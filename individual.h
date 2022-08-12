#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "Gene.h"
#include <string>
#include <vector>
#include<cmath>
#include "parameter.h"
#include "dataSource.h"
class Individual{
    public:
        Individual();
        Individual(const Individual& obj);
        ~Individual();
        Individual& operator= (const Individual& ind);
        void initialize();
        void mutation();
        void ISTransposition();
        void RISTransposition();
        void geneTransposition();
        void recombanation(const int pos, const int length, const std::string& str);
        std::string getGene();
        std::string content();
        void recombanation();
        void modifyContent(string content);
        void caculate();
        void recalculate();
        void fit();
        static inline int getLength() {return len;}
        double getFitness() {return fitness;}
        std::string showContent() const;
        string infixExpressionWithDc();     //返回含常数真实值的表达式
        std::vector<double> getResult(){return result;}
    private:
        inline int index_rand();
        Gene* gene;
        //length 
	    static int len;
        //个体数量
        std::vector<double> result;
        //目标值 
        static std::vector<double> dependentVar;
        //适应度
        double fitness;
};

#endif