#include "Gene.h"
#include <string>
#include <vector>
class indivadual{
    public:
        indivadual();
        ~indivadual();
        indivadual& operator= (const indivadual& ind);
        void initialize();
        void mutation();
        void ISTransposition();
        void RISTransposition();
        void geneTransposition();
        std::string getGene();
        void recombanation();
        void caculate();
        void fit();
    private:
        inline int index_rand();
        Gene* gene;
        //length 
	    static int len;
        //适应度
        std::vector<double> result;
        //目标值 
        static std::vector<double> dependentVar;
        //error
        double error;
};