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
public:
    void run();
    void evaluation(unordered_set<string> &classifiers, vector<unordered_map<char, double>> &indepen, vector<double> &depen);
    bool singleSample(unordered_set<string> &classifiers, unordered_map<char, double> &indepen, double &depen);
    bool singleClassifier(string classifier, unordered_map<char, double> &indepen, double &depen);
    
};




#endif