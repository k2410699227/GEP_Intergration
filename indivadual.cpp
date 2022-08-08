#include "indivadual.h"
#include "parameter.h"

Indivadual::Indivadual():gene(nullptr)
{
    gene = new Gene[GENE_NUM];
    len = GENE_NUM * Gene::getLength();
};

Indivadual::Indivadual(const Indivadual& obj)
: gene(new Gene[GENE_NUM])
{
    for (int i = 0; i < GENE_NUM; i++)
	{
		*(gene + i) = *(obj.gene + i);
	}
}

Indivadual::~Indivadual(){
    if(!gene){
        delete[] gene;
    }
}

void Indivadual::initialize(){
    for(int i = 0;i<GENE_NUM;i++){
        gene[i].initialize();
    }
    caculate();
    fit();
}

void Indivadual::caculate(){}
void Indivadual::fit(){
    //if n > Cp,then Fn = n;else Fi = 0;
}
int Indivadual::index_rand()
{
	return rand() % GENE_NUM;
}
void Indivadual::mutation(){
    int index = index_rand();
	gene[index].mutation();
}
void Indivadual::ISTransposition(){
    // 随机选取转座长度
	int len_array = sizeof(IS_ELEM_LEN) / sizeof(int);
	int index = rand() % len_array;
	int len_is = IS_ELEM_LEN[index];
	// 从整段染色体中随机选取len_is个长度的片段
	index = rand() % (len - len_is);	// 随机起始索引
	std::string str = content().substr(index, len_is);	// 截取字符串
	// 随机选择基因
	index = index_rand();
	gene[index].transposition(str);
}
void Indivadual::RISTransposition(){
    // 随机选择基因
	int index = index_rand();
	// 在头中随机选取一点
	int pos = rand() % HEAD_LEN;
	// 沿该点向后查找直到发现一个函数 该点即为RIS转座的起始点
	int a;
	if ((a = gene[index].findFunc(pos)) == -1)
		return;
	pos = a + Gene::getLength() * index;
	// 随机选取RIS转座长度
	int len_array = sizeof(RIS_ELEM_LEN) / sizeof(int);
	index = rand() % len_array;
	int len_ris = RIS_ELEM_LEN[index];
	// 从起始点向后取len_ris个长度的片段
	std::string str = content().substr(index, len_ris);
	index = index_rand();
	gene[index].transposition(str);
}

std::string Indivadual::content(){
    std::string content;
    for(int i = 0;i<GENE_NUM;i++){
        content += gene[i].getContent();
    }
    return content;
}

void Indivadual::geneTransposition(){
    if(GENE_NUM > 1){
        int index1=0,index2 = 0;
        while(index1 == index2){
            index1 = index_rand();
            index2 = index_rand();
        }
        Gene temp(gene[index1]);
        gene[index1] = gene[index2];
        gene[index2] = temp;
    }
}
void Indivadual::recombanation(const int pos, const int length, const std::string& str){
    std::string text = content();
	text.replace(pos, length, str);
	for (int i = 0; i < GENE_NUM; i++)
	{
		Gene temp(text.substr(i * Gene::getLength(), Gene::getLength()));
		gene[i] = temp;
	}
}