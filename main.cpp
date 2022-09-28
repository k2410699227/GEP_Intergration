#include <ctime>
#include "dataSource.h"
#include "parameter.h"
#include "population.h"
#include "MultiGEP.h"
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{
	double targetAccuracy = 0.7;
	for (int i = 1; i < argc; i++)
	{
		// cout << argv[i] << endl;
		if (strcmp(argv[i], "-a") == 0)
		{
			targetAccuracy = atof(argv[i + 1]);
			cout << targetAccuracy << endl;
		}
	}

	clock_t start, finish;
	start = clock();
	MultiGEP myMultiGEP;
	myMultiGEP
		.setClassifierCount(1000)
		.setTargetAccuracy(targetAccuracy)
		.run();

	finish = clock();
	printf("运行时间为: %d \n", (int)((finish - start) / CLOCKS_PER_SEC));

	return 0;
}
