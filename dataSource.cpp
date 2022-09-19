#include "dataSource.h"
#include <cstring>
#include <cstdlib>

vector<unordered_map<char, double>> DataSource::independentVar = {};
vector<double> DataSource::dependentVar = {};
vector<unordered_map<char, double>>DataSource::indepenEvaluation = {};
vector<double>DataSource::depenEvaluation = {};

DataSource::DataSource()
{
    setDependentData();
    setIndependentData();
    assert(independentVar.size() == dependentVar.size());
}

vector<string> DataSource::getFileText(const string path)
{
    ifstream inFile;
    vector<string> res;
    inFile.open(path);
    string s;
    while (getline(inFile, s))
    {
        res.push_back(s);
    }
    inFile.close();
    return res;
}

void DataSource::setIndependentData()
{
    vector<string> text = getFileText(PATH_INDEPENDENT);
    for (auto line : text)
    {
        vector<string> result;
        //空格分割
        split(line, result, " ");
        assert(result.size() >= (sizeof(Terminator) / sizeof(char)));

        vector<double> temp = toDouble(result);
        unordered_map<char, double> single;
        for (int i = 0; i < temp.size(); i++)
            single[Terminator[i]] = temp[i];
        independentVar.push_back(single);
    }

    text = getFileText(PATH_VALIDATION);
    for (auto line : text)
    {
        vector<string> result;
        //空格分割
        split(line, result, " ");
        assert(result.size() >= (sizeof(Terminator) / sizeof(char) + 1));

        vector<double> temp = toDouble(result);
        unordered_map<char, double> single;
        for (int i = 0; i < temp.size() - 1; i++)
            single[Terminator[i]] = temp[i];
        indepenEvaluation.push_back(single);
        depenEvaluation.emplace_back(temp.back());
    }
}

void DataSource::setDependentData()
{
    vector<string> text = getFileText(PATH_DEPENDENT);
    for (auto line : text)
    {
        vector<string> result;
        //空格分割
        split(line, result, " ");
        dependentVar.push_back(atof(result[0].c_str()));
    }
}

void split(const std::string &input_str, std::vector<std::string> &output, const char *delim)
{
    int pos = 0;
    int npos = 0;
    int regexlen = strlen(delim);
    while ((npos = input_str.find(delim, pos)) != -1)
    {
        std::string tmp = input_str.substr(pos, npos - pos);
        output.push_back(tmp);
        pos = npos + regexlen;
    }
    output.push_back(input_str.substr(pos, input_str.length() - pos));
}

vector<double> DataSource::toDouble(vector<string> &data)
{
    vector<double> temp;
    for (auto it : data)
    {
        temp.push_back(atof(it.c_str()));
    }
    return temp;
}
