#include <vector>
#include "indivadual.h"
#include "population.h"
#include "parallel.h"

Population::Population(const int num):num(num)
{
    indivadual = new Indivadual[num];
}
Population::~Population(){
    if (!indivadual)
		delete[] indivadual;
}
void Population::initialize(){
    for (int i = 0;i<num;i++){
        indivadual[i].initialize();
    }
}
void Population::mutation(){
    for(int i =0;i<num;i++){
        double prob = rand() % 100 / 100;
        if(prob < MUTATION_RATE){
            indivadual[i].mutation();
        }
    }
}
void Population::ISTransposition(){
    for(int i =0;i<num;i++){
        double prob = rand() % 100 / 100;
        if(prob < MUTATION_RATE){
            indivadual[i].ISTransposition();
        }
    }
}
void Population::RISTrasposition(){
    for(int i =0;i<num;i++){
        double prob = rand() % 100 / 100;
        if(prob < MUTATION_RATE){
            indivadual[i].RISTransposition();
        }
    }
}
void Population::geneTransposition(){
    for(int i = 0;i<num;i++){
        double prob = rand() / double(RAND_MAX);
        if(prob < GENE_TRANS_RATE){
            indivadual[i].geneTransposition();
        }
    }
}
void Population::geneRecombination(){
    double prob = rand() / double(RAND_MAX);
    if (prob < GENE_COMBA_RATE){
        int index1 = 0;
        int index2 = 0;
        while(index1 == index2){
            index1 = rand() % num;
            index2 = rand() % num;
        }
        int geneIndex = rand() % GENE_NUM;
        std::string subStr1 = indivadual[index1].content().substr(geneIndex * Gene::getLength(),Gene::getLength());
        std::string subStr2 = indivadual[index2].content().substr(geneIndex * Gene::getLength(),Gene::getLength());
        indivadual[index1].recombanation(geneIndex * Gene::getLength(),Gene::getLength(),subStr2);
        indivadual[index2].recombanation(geneIndex * Gene::getLength(),Gene::getLength(),subStr1);
    }
}
void Population::onePointRecombination(){
    double prob = rand() / double(RAND_MAX);
	if (prob < ONE_POINT_COMBA_RATE)
	{
		// 随机选取两个不同个体
		int index1 = 0;
		int index2 = 0;
		while (index1 == index2)
		{
			index1 = rand() % num;
			index2 = rand() % num;
        }
		// 随机选取重组点
		int pos = rand() % Indivadual::getLength();
		std::string subStr1 = indivadual[index1].content().substr(0,pos);
		std::string subStr2 = indivadual[index2].content().substr(0,pos);
		// 交换两个染色体的片段
		indivadual[index1].recombanation(0, pos, subStr2);
		indivadual[index2].recombanation(0, pos, subStr1);
	}
}
void Population::twoPointRecombination(){
    double prob = rand() / double(RAND_MAX);
    if(prob < TWO_POINT_COMBA_RATE){
        int index1 = 0;
        int index2 = 0;
        while(index1 == index2){
            index1 = rand() % num;
            index2 = rand() % num;
        }
        int pos1 = rand() % Indivadual::getLength();
        int pos2 = rand() % (Indivadual::getLength() - pos1) + pos1;
        int len = pos2-pos1+1;
        std::string subStr1 = indivadual[index1].content().substr(pos1,len);
        std::string subStr2 = indivadual[index2].content().substr(pos1,len);
        indivadual[index1].recombanation(pos1,len,subStr2);
        indivadual[index2].recombanation(pos1,len,subStr1);
    }
}
void Population::evolution(){
    double sumOfFit = 0.0;
    for(int i = 0;i < INDIVADUAL_NUM;i++){
        sumOfFit += indivadual[i].getError();
    }
    std::vector<double> gambleRate;
    for(int i =0;i < INDIVADUAL_NUM;i++){
        gambleRate.push_back(indivadual[i].getError() / sumOfFit);
    }
    //选择个体
    // 左值为左区间 右值为右区间
	std::vector<std::pair<double,double> > section;
	// 将概率展开为长度为1的线段 划分每个个体所占区间
	double temp = 0.0;
	for (int i = 0; i < INDIVADUAL_NUM - 1; i++)
	{
		section.push_back(std::pair<double, double>(temp, temp + gambleRate[i]));
		if(i < INDIVADUAL_NUM -2)
			temp += gambleRate[i];
	}
	section.push_back(std::pair<double, double>(temp, 1.0)); 
	std::vector<std::string> context = {};
	for (int i = 0; i < INDIVADUAL_NUM; i++)
	{
		// 生成0~1间的随机算子
		double prob = rand() / double(RAND_MAX);
		for (int j = 0; j < INDIVADUAL_NUM; j++)
		{
			// 如果落在该区间内 则加入该个体
			if (prob >= section[j].first && prob <= section[j].second)
			{
				context.push_back(indivadual[j].content());
				break;
			}
		}
    }
    //补齐中间步骤
    this->mutation();
    this->ISTransposition();
    this->RISTrasposition();
    this->geneTransposition();
    this->onePointRecombination();
    this->twoPointRecombination();
    this->geneRecombination();

}