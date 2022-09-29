#include <ctime>
#include "MultiGEP.h"
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	clock_t start, finish;
	start = clock();
	MultiGEP myMultiGEP;
	myMultiGEP
		.setClassifierCount(1000)
		.setTargetAccuracy(0.8)
		.setCalculator({'+', '-', '*', '/'})
		.setFeatures(10)
		.setThreshold(5.0)
		.setIndividualNum(100)
		.setGeneNum(5)
		.setConnetor('+')
		.allowSingleGene(true)
		.setMutationRate(0.044)
		.setEvaluationSetPath("E:\\VS Project\\GEP_Intergration\\data\\evaluation.txt")
		.setSaveModelPath("E:\\VS Project\\GEP_Intergration\\data\\save_model")
		.setTrainSetPath("E:\\VS Project\\GEP_Intergration\\data\\train.txt")
		.loadModel("E:\\VS Project\\GEP_Intergration\\data\\save_model")
		.train();
		myMultiGEP.evaluation();
		
	
	finish = clock();
	printf("运行时间为: %d \n", (int)((finish - start) / CLOCKS_PER_SEC));
	system("pause");
	return 0;
}
