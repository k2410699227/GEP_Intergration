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
	
	/* =======================设置训练样本====================== */
	
	srand((int)time(0));
	// // 设置自变量
	// Gene::setData(file.independent());
	// // 设置因变量
	// Individual::setDependentVar(file.dependent());

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
		
		// cout << "------------------------第" << i+1 << "代------------------------" << endl;
		// cout << endl;
		// parent.context();
		// cout << endl;
		parent.evolution();
	}
	cout << "------------------------第" << GENERATION << "代------------------------" << endl;
	// parent.getContext();
	if (parent.excellentIndiv(maxFitness, num_index, excellGene, excellInfix))
		num_generaton = 1000;
	cout << endl;
	cout << endl << "迭代完成..." << endl;
	cout << "当前代最优秀个体基因如下: " << endl;
	cout << "第" << num_generaton << "代的第" << num_index << "个个体：" << endl;
	cout << "基因：" << excellGene << endl;
	cout << "中缀表达式: " << excellInfix << endl;

	Gene::destroyDc();

	return 0;
}


