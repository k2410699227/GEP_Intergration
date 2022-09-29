#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "parameter.h"
#include <assert.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;

/** 分割字符串*/
void split(const std::string &input_str, std::vector<std::string> &output, const char *delim);

/** 将字符串数组转为浮点数*/
vector<double> toDouble(vector<string> &data);

/** 从参数中指定的路径获取文件内容*/
vector<string> getFileText(const string path);
class DataSource
{
private:
    /** 自变量数据 */
    static vector<unordered_map<char, double>> independentVar;
    /** 因变量数据 */
    static vector<double> dependentVar;

    /** 验证用自变量数据 */
    static vector<unordered_map<char, double>> indepenEvaluation;
    /** 验证用因变量数据 */
    static vector<double> depenEvaluation;
    Parameter parameter;

public:
    /**构造函数内初始化输入文件和输出文件*/
    DataSource(Parameter &p);
    ~DataSource()
    {
        independentVar.clear();
        dependentVar.clear();
        depenEvaluation.clear();
        indepenEvaluation.clear();
    }

    /** 解析自变量数据*/
    void setTrainData();

    /** 解析因变量数据*/
    void setEvaluationData();

    /** 返回自变量数据 */
    static vector<unordered_map<char, double>> &independent() { return independentVar; }

    /** 返回因变量数据 */
    static vector<double> &dependent() { return dependentVar; }

    /** 返回验证自变量数据 */
    static vector<unordered_map<char, double>> &getIndepenEvaluation() { return indepenEvaluation; }

    /** 返回验证因变量数据 */
    static vector<double> &getDepenEvaluation() { return depenEvaluation; }

    static int sampleCount() { return dependentVar.size(); }
};

#endif