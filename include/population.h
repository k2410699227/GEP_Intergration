#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"
#include "parameter.h"
#include <iostream>
class Population
{
public:
    Population(const int num, Parameter &p);
    ~Population();
    void initialize();
    void reInitialize();
    void evolution();
    bool excellentIndiv(double &, int &, string &, string &); //判断是否出现适应度更佳个体，并更新传入变量的个体信息
    void display() const;
    string bestIndiv();
    Individual *getIndividual() { return individual; }
    int pickTargetIndiv(unordered_set<string> &targetIndividual, int num, int &invidualCount); //挑选num个符合预期个体，已达预期个体数返回-1，否则返回选取个体数

private:
    void mutation();
    void ISTransposition();
    void RISTrasposition();
    void geneTransposition();
    void onePointRecombination();
    void twoPointRecombination();
    void geneRecombination();

    Parameter parameter;
    Individual *individual;
    int num;
};

#endif