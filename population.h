#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"
#include "parameter.h"
#include <iostream>
class Population{
    public:
        Population(const int num);
        ~Population();
        void initialize();
        void evolution();
        bool excellentIndiv(double&, int&, string&, string&);   //判断是否出现适应度更佳个体，并更新传入变量的个体信息
        void display() const;
        Individual* getIndividual(){return individual;}
    private:
        void mutation();
        void ISTransposition();
        void RISTrasposition();
        void geneTransposition();
        void onePointRecombination();
        void twoPointRecombination();
        void geneRecombination();
        
        Individual* individual;
        int num;
};

#endif