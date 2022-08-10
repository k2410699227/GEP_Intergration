#include <ctime>
#include "dataSource.h"
#include"parameter.h"
#include "population.h"
#include "Gene.h"
#include <iostream>
using namespace std;

/* 迭代过程中最优秀个体 */
int num_generaton = 0;	// 代数
int num_index = 0;		// 个体索引
string excellGene = "";	// 基因
string excellInfix = "";	// 中缀表达式
double maxFitness = 0.0;	// 最大适宜度

void clearOutputFile();

int main()
{
	//获取样本数据
	DataSource file;
	srand((int)time(0));

	/* =======================设置Dc域====================== */
	Gene::DcInit();
	
	/* =======================迭代开始====================== */
	Population parent(INDIVIDUAL_NUM);
	// 初始化种群
	parent.initialize();

	for (int i = 0; i < GENERATION - 1; i++)
	{
		if (parent.excellentIndiv(maxFitness, num_index, excellGene, excellInfix))
			num_generaton = i + 1;
		
		cout << "------------------------NO." << i+1 << "generation------------------------" << endl;
		cout << endl;
		parent.display();
		cout << endl;
		parent.evolution();
	}
	cout << "------------------------No." << GENERATION << "generation------------------------" << endl;
	parent.display();
	if (parent.excellentIndiv(maxFitness, num_index, excellGene, excellInfix))
		num_generaton = GENERATION;
	cout << endl;
	cout << endl << "iterate finished..." << endl;
	cout << "best gene: " << endl;
	cout << "No." << num_generaton << "generation, No." << num_index << "individual:" << endl;
	cout << "gene:" << excellGene << endl;
	cout << "infix expression" << excellInfix << endl;

	Gene::destroyDc();

	return 0;
}


