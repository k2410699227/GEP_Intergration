#ifndef MULTIGEP_H
#define MULTIGEP_H
#include "parameter.h"
#include "dataSource.h"
#include "population.h"
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <string>
#include "Gene.h"

class MultiGEP
{
private:
    unordered_set<string> qualifiedClassifier; //储存表达式的容器
    Parameter parameter;

    string save_model_path = "";

    bool singleSample(unordered_set<string> &classifiers, unordered_map<char, double> &indepen, double &depen);
    bool singleClassifier(string classifier, unordered_map<char, double> &indepen, double &depen);

public:
    void train();

    /// @brief 评估模型性能
    void evaluation();

    /// @brief 从已保存的文件的中加载模型，默认路径为"./data/model.save"
    /// @param path 已保存模型的路径
    MultiGEP &loadModel(const std::string path);

    /// @brief 设置模型保存路径，若未指定则不进行保存
    /// @param path 模型保存路径
    MultiGEP &setSaveModelPath(const std::string path);

    /// @brief 设置所需分类器的数量，默认为100
    /// @param num 分类器的数量
    MultiGEP &setClassifierCount(int num);

    /// @brief 设置单个分类器在训练集上的准确率，默认为0.7
    /// @param num 分类器的准确率
    MultiGEP &setTargetAccuracy(double rate);

    /// @brief 设置运算符号集，使用大括号列表初始化，默认为{'-', '+', '*', '/'}
    /// @param list 运算符号集
    MultiGEP &setCalculator(initializer_list<char> list);

    /// @brief 根据特征数目选取终结符号集，从[a-z]、[A-Z]中选择，优先选择小写，默认为{a-j}10个
    /// @param num 样本特征数量，最大为52
    MultiGEP &setFeatures(int num);

    /// @brief 设置分类阈值，大于阈值的为正类，默认值为0.5
    /// @param t 分类阈值
    MultiGEP &setThreshold(double t);

    /// @brief 设置基因个数，默认为5
    /// @param num 基因个数
    MultiGEP &setGeneNum(int num);

    /// @brief 设置种群个体数量，默认为100
    /// @param num 种群个体数量
    MultiGEP &setIndividualNum(int num);

    /// @brief 设置连接符号，可选：{+, -, *, /}, 默认为 +
    /// @param connetor 连接符号
    MultiGEP &setConnetor(char connetor);

    /// @brief 是否允许单基因表达式, 默认为TRUE，即允许
    /// @param flag true or false
    MultiGEP &allowSingleGene(bool flag);

    /// @brief 设置变异概率， 默认为0.044
    /// @param rate 变异概率
    MultiGEP &setMutationRate(double rate);

    /// @brief 设置验证集路径
    /// @param str 验证集路径
    MultiGEP &setEvaluationSetPath(const std::string &str);

    /// @brief 设置训练集文件路径
    /// @param str 训练集文件路径
    MultiGEP &setTrainSetPath(const std::string &str);

};

#endif