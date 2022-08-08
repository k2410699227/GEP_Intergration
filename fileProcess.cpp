#include "fileProcess.h"
#include <cstring>
#include <cstdlib>

FileProcess::FileProcess()
{
    setDependentData();
    setIndependentData();
}

vector<string> FileProcess::getFileText(const string path)
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

void FileProcess::setIndependentData()
{
    vector<string> text = getFileText(PATH_INDEPENDENT);
    for (auto line : text)
    {
        vector<string> result;
        //空格分割
        split(line, result, " ");
        vector<double> temp = toDouble(result);
        independentVar.push_back(temp);
    }
}

void FileProcess::setDependentData()
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

void FileProcess::split(const std::string &input_str, std::vector<std::string> &output, const char *delim)
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

vector<double> FileProcess::toDouble(vector<string> &data)
{
    vector<double> temp;
    for (auto it : data)
    {
        temp.push_back(atof(it.c_str()));
    }
    return temp;
}
