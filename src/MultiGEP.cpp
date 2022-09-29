#include "MultiGEP.h"
double targetAccuracy = 0.7;

void MultiGEP::train()
{
    if (this->parameter.PATH_TRAIN.empty())
    {
        printf("未设置训练集路径\n");
        return;
    }
    //获取样本数据
    DataSource data(parameter);
    data.setTrainData();
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
            if (accumulate(statistic.end() - 10, statistic.end(), 0) < parameter.INDIVIDUAL_NUM / 10) // 10代选取个体数之和<种群个体数十分之一
            {
                statistic.clear();
                parent->reInitialize(); //重新初始化种群
                newGeneration = 1;
            }
        cout << "-----------------------Generation " << i + 1 << "------------------------ " << qualifiedClassifier.size() << endl;

        parent->evolution();
    }

    delete parent;
    // evaluation();

    if (!this->save_model_path.empty())
    {
        ofstream file;
        file.open(this->save_model_path);
        for (auto cls : qualifiedClassifier)
        {
            file << cls << endl;
        }
        file.close();
    }
}

void MultiGEP::evaluation()
{
    if (parameter.PATH_EVALUATION.empty())
    {
        printf("未设置验证集路径\n");
        return;
    }
    DataSource data(parameter);
    data.setEvaluationData();
    vector<unordered_map<char, double>> &indepen = DataSource::getIndepenEvaluation();
    vector<double> &depen = DataSource::getDepenEvaluation();
    int accurate = 0;
    int allCount = indepen.size();
    for (int i = 0; i < allCount; i++)
    {
        if (singleSample(qualifiedClassifier, indepen[i], depen[i]))
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

    if (parameter.CONNET == '*' || parameter.CONNET == '/')
        value = 1.0;
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

MultiGEP &MultiGEP::loadModel(const std::string path)
{
    qualifiedClassifier.clear();
    vector<string> temp = getFileText(path);
    for (auto t : temp)
    {
        qualifiedClassifier.insert(t);
    }
    return *this;
}

MultiGEP &MultiGEP::setSaveModelPath(const std::string path)
{

    this->save_model_path = path;
    return *this;
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

MultiGEP &MultiGEP::setCalculator(initializer_list<char> list)
{
    this->parameter.Function = list;
    return *this;
}

MultiGEP &MultiGEP::setFeatures(int featureNum)
{
    assert(featureNum <= 52 && featureNum > 0);
    this->parameter.Terminator.clear();
    char c = 'a';
    for (int i = 0; i < featureNum; i++)
    {
        this->parameter.Terminator.emplace_back(c);
        c++;
        if (c > 'z')
            c = 'A';
    }
    return *this;
}

MultiGEP &MultiGEP::setThreshold(double t)
{
    this->parameter.THRESHOLD = t;
    return *this;
}

MultiGEP &MultiGEP::setGeneNum(int num)
{
    this->parameter.GENE_NUM = num;
    return *this;
}

MultiGEP &MultiGEP::setIndividualNum(int num)
{
    this->parameter.INDIVIDUAL_NUM = num;
    return *this;
}

MultiGEP &MultiGEP::setConnetor(char conn)
{
    this->parameter.CONNET = conn;
    return *this;
}

MultiGEP &MultiGEP::allowSingleGene(bool flag)
{
    this->parameter.Allow_Single_Gene = flag;
    return *this;
}

MultiGEP &MultiGEP::setMutationRate(double rate)
{
    this->parameter.MUTATION_RATE = rate;
    return *this;
}

MultiGEP &MultiGEP::setEvaluationSetPath(const string &path)
{
    this->parameter.PATH_EVALUATION = path;
    return *this;
}

MultiGEP &MultiGEP::setTrainSetPath(const string &path)
{
    this->parameter.PATH_TRAIN = path;
    return *this;
}
