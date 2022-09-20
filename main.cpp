#include <ctime>
#include "dataSource.h"
#include "parameter.h"
#include "population.h"
#include "MultiGEP.h"
#include <iostream>
#include <unordered_map>
#include <string.h>
using namespace std;

/* 迭代过程中最优秀个体 */
int num_generaton = 0;	 // 代数
int num_index = 0;		 // 个体索引
string excellGene = "";	 // 基因
string excellInfix = ""; // 中缀表达式
double maxFitness = 0.0; // 最大适宜度

int main(int argc, char *argv[])
{//cout<<argc<<endl;
	
	for(int i = 0; i< argc; i++)
	{
		cout << argv[i] << endl;
		if (strcmp(argv[i], "-a")==0)
			targetAccuracy = atof(argv[i + 1]);
	}
	cout << targetAccuracy << endl;

	clock_t start, finish;
	start = clock();
	MultiGEP myMultiGEP;
	//获取样本数据
	DataSource file;
	srand((int)time(0));

	/* =======================设置Dc域====================== */
	Gene::DcInit();
	unordered_set<string> qualifiedClassifier;
	/* =======================迭代开始====================== */
	Population *parent = new Population(INDIVIDUAL_NUM);

	// 初始化种群
	parent->initialize();

	for (int i = 0;; i++)
	{
		if (!parent->pickTargetIndiv(qualifiedClassifier, classifierCount)) //挑选出足够个体后结束迭代
			break;
		cout << "-----------------------Generation " << i + 1 << "------------------------" << qualifiedClassifier.size() << endl;
		cout << endl;
		// parent.display();
		cout << endl;
		if (parent->excellentIndiv(maxFitness, num_index, excellGene, excellInfix))
		{
			num_generaton = i + 1;
			if (maxFitness == RANGE * DataSource::dependent().size() || (maxFitness == DataSource::sampleCount() && CLASSIFICATION))
			{
				// for (auto i : parent.getIndividual()[num_index - 1].getResult())
				// 	cout << i << endl;
				// cout << parent.getIndividual()[num_index - 1].showContent();
				break;
			}
		}

		parent->evolution();
	}

	// cout << "------------------------Generation " << GENERATION << "------------------------" << endl;
	// parent.display();
	// if (parent.excellentIndiv(maxFitness, num_index, excellGene, excellInfix))
	// 	num_generaton = GENERATION;
	// cout << endl;

	// cout << endl
	// 	 << "iteration finished..." << endl;
	// cout << "best gene: " << endl;
	// cout << "No." << num_generaton << " generation, No." << num_index << " individual:" << endl;
	// cout << "gene: " << excellGene << endl;
	// cout << "infix expression: " << excellInfix << endl;
	delete parent;
	Gene::destroyDc();
	finish = clock();
	printf("运行时间为: %d \n筛选出 %d 个个体\n", (finish - start) / CLOCKS_PER_SEC, qualifiedClassifier.size());
	myMultiGEP.evaluation(qualifiedClassifier, DataSource::getIndepenEvaluation(), DataSource::getDepenEvaluation());
	return 0;
}
