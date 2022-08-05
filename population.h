#include "indivadual.h"
class Population{
    public:
        Population(const int num);
        ~Population();
        void initialize();
        void evolution();
    private:
        void mutation();
        void ISTransposition();
        void RISTrasposition();
        void geneTransposition();
        void onePointRecombination();
        void twoPointRecombination();
        void geneRecombination();

        Indivadual* indivadual;
        int num;
};