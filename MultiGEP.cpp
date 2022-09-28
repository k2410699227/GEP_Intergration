#include "MultiGEP.h"
double targetAccuracy = 0.7;

void MultiGEP::run()
{
    //获取样本数据
    DataSource file(parameter);
    srand((int)time(0));
    /* =======================迭代开始====================== */
    Population *parent = new Population(parameter.INDIVIDUAL_NUM, parameter);

    // 初始化种群
    int individualCount = 0;
    int newGeneration = 1;
    parent->initialize();
    vector<int> statistic;
    for (int i = 0;; i++)
    {
        int numOfPick = parent->pickTargetIndiv(qualifiedClassifier, parameter.classifierCount, individualCount);
        if (numOfPick == -1) //挑选出足够个体后结束迭代
            break;
        statistic.push_back(numOfPick);
        newGeneration++;
        if (newGeneration > parameter.leastGeneration)
        {
            parent->reInitialize();
            newGeneration = 1;
        }
        // if (accumulate(statistic.end() - 10, statistic.end(), 0) < INDIVIDUAL_NUM / 10) // 10代选取个体数之和<种群个体数十分之一
        // {
        // 	parent->reInitialize(); //重新初始化种群
        // 	newGeneration = 1;
        // }
        cout << "-----------------------Generation " << i + 1 << "------------------------ " << qualifiedClassifier.size() << endl;

        parent->evolution();
    }

    delete parent;
    evaluation(qualifiedClassifier, DataSource::getIndepenEvaluation(), DataSource::getDepenEvaluation());
}

void MultiGEP::evaluation(unordered_set<string> &classifiers, vector<unordered_map<char, double>> &indepen, vector<double> &depen)
{
    int accurate = 0;
    int allCount = indepen.size();
    for (int i = 0; i < allCount; i++)
    {
        if (singleSample(classifiers, indepen[i], depen[i]))
            accurate += 1;
    }

    printf("准确率为: %.2f %c\n", (double)accurate / allCount * 100, '%');
}

bool MultiGEP::singleSample(unordered_set<string> &classifiers, unordered_map<char, double> &indepen, double &depen)
{
    int trueClassifier = 0;

    for (auto c : classifiers)
    {
        if (singleClassifier(c, indepen, depen))
            trueClassifier += 1;
    }

    if (trueClassifier > classifiers.size() / 2)
        return true;

    return false;
}

bool MultiGEP::singleClassifier(string classifier, unordered_map<char, double> &indepen, double &depen)
{
    double value = 0;
    vector<string> genes;
    Gene temp; //临时对象，用于调用基因表达式值的计算函数
    split(classifier, genes, " ");

    for (auto g : genes)
    {
        switch (parameter.CONNET)
        {
        case '+':
            value += temp.geneExpressing(-1, g, indepen);
            break;
        case '-':
            value -= temp.geneExpressing(-1, g, indepen);
            break;
        case '*':
            value *= temp.geneExpressing(-1, g, indepen);
            break;
        case '/':
            value /= temp.geneExpressing(-1, g, indepen);
            if (isnan(value) || isinf(value))
                value = 0.0;
            break;
        default:
            break;
        }
    }

    double res = value > parameter.THRESHOLD ? 1.0 : 0.0;

    return res == depen ? true : false;
}

MultiGEP &MultiGEP::setClassifierCount(int num)
{
    this->parameter.classifierCount = num;
    return *this;
}

MultiGEP &MultiGEP::setTargetAccuracy(double rate)
{
    this->parameter.targetAccuracy = rate;
    return *this;
}