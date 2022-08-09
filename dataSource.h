#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "parameter.h"
#include<assert.h>
#include <vector>
#include <string>
#include<unordered_map>
#include <fstream>
using namespace std;
class DataSource
{
private:
    /** 自变量数据 */
    static vector<unordered_map<char,double>> independentVar;
    /** 因变量数据 */
    static vector<double> dependentVar;

    /** 用空格分割字符串*/
    void split(const std::string &input_str, std::vector<std::string> &output, const char *delim);

    /** 将字符串数组转为浮点数*/
    vector<double> toDouble(vector<string> &data);

    /** 从参数中指定的路径获取文件内容*/
    vector<string> getFileText(const string path);

    /** 解析自变量数据*/
    void setIndependentData();

    /** 解析因变量数据*/
    void setDependentData();

public:
    /**构造函数内初始化输入文件和输出文件*/
    DataSource();
    ~DataSource()
    {
        independentVar.clear();
        dependentVar.clear();
    }

    /** 返回自变量数据 */
    static vector<unordered_map<char,double>> independent() { return independentVar; }

    /** 返回因变量数据 */
    static vector<double> dependent() { return dependentVar; }

    static int sampleCount(){return dependentVar.size();}
};

#endif