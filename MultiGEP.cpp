#include "MultiGEP.h"
double targetAccuracy = 0.7;
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
        switch (CONNET)
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

    double res = value > THRESHOLD ? 1.0 : 0.0;

    return res == depen ? true : false;
}