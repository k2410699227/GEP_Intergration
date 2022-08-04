#include "indivadual.h"
#include "parallel.h"

indivadual::indivadual():gene(nullptr)
{
    gene = new Gene[GENE_NUM];
    len = GENE_NUM * Gene::getLength();
};

indivadual::indivadual(const indivadual& obj)
: gene(new Gene[GENE_NUM])
{
    for (int i = 0; i < GENE_NUM; i++)
	{
		*(gene + i) = *(obj.gene + i);
	}
}

indivadual::~indivadual(){
    if(!gene){
        delete[] gene;
    }
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
    // 随机选取转座长度
	int len_array = sizeof(IS_LEN) / sizeof(int);
	int index = rand() % len_array;
	int len_is = IS_LEN[index];
	// 从整段染色体中随机选取len_is个长度的片段
	index = rand() % (len - len_is);	// 随机起始索引
	std::string str = content().substr(index, len_is);	// 截取字符串
	// 随机选择基因
	index = index_rand();
	gene[index].transposition(str);
}
void indivadual::RISTransposition(){
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
	int len_array = sizeof(RIS_LEN) / sizeof(int);
	index = rand() % len_array;
	int len_ris = RIS_LEN[index];
	// 从起始点向后取len_ris个长度的片段
	std::string str = content().substr(index, len_ris);
	index = index_rand();
	gene[index].transposition(str);
}

std::string indivadual::content(){
    std::string content;
    for(int i = 0;i<GENE_NUM;i++){
        content += gene[i].getContent();
    }
    return content;
}

void indivadual::geneTransposition(){
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