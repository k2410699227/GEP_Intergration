#include "Gene.h"
#include <string>
#include <vector>
class Indivadual{
    public:
        Indivadual();
        ~Indivadual();
        Indivadual& operator= (const Indivadual& ind);
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
        double getError() {return error;}
    private:
        inline int index_rand();
        Gene* gene;
        //length 
	    static int len;
        //个体数量
        std::vector<double> result;
        //目标值 
        static std::vector<double> dependentVar;
        //error
        double error;
};