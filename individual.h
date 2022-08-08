#include "Gene.h"
#include <string>
#include <vector>
#include<cmath>
#include "parameter.h"
#include "dataSource.h"
class Individual{
    public:
        Individual();
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
        void caculate();
        void fit();
        static inline int getLength() {return len;}
        double getFitness() {return fitness;}
        double getError() {return error;}
        std::string showContent() const;
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