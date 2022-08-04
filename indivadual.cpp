#include "indivadual.h"
#include "parallel.h"

indivadual::indivadual():gene(nullptr)
{
    gene = new Gene[GENE_NUM];
    len = GENE_NUM * Gene::getLength();
};

indivadual::indivadual(const indivadual& obj){
    
}

void indivadual::initialize(){
    for(int i = 0;i<GENE_NUM;i++){
        gene[i].initialize();
    }
    caculate();
    fit();
}

void indivadual::caculate(){}
void indivadual::fit(){
    //if n > Cp,then Fn = n;else Fi = 0;
}
int indivadual::index_rand()
{
	return rand() % GENE_NUM;
}
void indivadual::mutation(){
    int index = index_rand();
	gene[index].mutation();
}
void indivadual::ISTransposition(){
    
}
void indivadual::RISTransposition(){}
