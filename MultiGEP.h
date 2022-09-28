#ifndef MULTIGEP_H
#define MULTIGEP_H
#include "parameter.h"
#include "dataSource.h"
#include "population.h"
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>
#include "Gene.h"

class MultiGEP
{
private:
    unordered_set<string> qualifiedClassifier;
    Parameter parameter;

    bool singleSample(unordered_set<string> &classifiers, unordered_map<char, double> &indepen, double &depen);
    bool singleClassifier(string classifier, unordered_map<char, double> &indepen, double &depen);

public:
    void run();
    void evaluation(unordered_set<string> &classifiers, vector<unordered_map<char, double>> &indepen, vector<double> &depen);

    /// @brief 设置所需分类器的数量
    /// @param num 分类器的数量
    MultiGEP &setClassifierCount(int num);

    /// @brief 设置单个分类器在训练集上的准确率
    /// @param num 分类器的准确率
    MultiGEP &setTargetAccuracy(double rate);
};

#endif