#include <vector>
#include<iomanip>
#include "individual.h"
#include "population.h"
#include "parameter.h"

Population::Population(const int num) : num(num)
{
    individual = new Individual[num];
}
Population::~Population()
{
    if (!individual)
        delete[] individual;
}
void Population::initialize()
{
    for (int i = 0; i < num; i++)
    {
        individual[i].initialize();
    }
}
void Population::mutation()
{
    double sumOfFit = 0.0,rate = 0.0;
    for (int i = 0; i < INDIVIDUAL_NUM; i++)
    {
        sumOfFit += individual[i].getFitness();
    }
    for (int i = 0; i < num; i++)
    {
        rate = 1 - individual[i].getFitness()/sumOfFit;
        individual[i].mutation(rate);
    }
}
void Population::ISTransposition()
{
    for (int i = 0; i < num; i++)
    {
        double prob = rand() % 100 / 100;
        if (prob < IS_TRANS_RATE)
        {
            individual[i].ISTransposition();
        }
    }
}
void Population::RISTrasposition()
{
    for (int i = 0; i < num; i++)
    {
        double prob = rand() % 100 / 100;
        if (prob < RIS_TRANS_RATE)
        {
            individual[i].RISTransposition();
        }
    }
}
void Population::geneTransposition()
{
    for (int i = 0; i < num; i++)
    {
        double prob = rand() / double(RAND_MAX);
        if (prob < GENE_TRANS_RATE)
        {
            individual[i].geneTransposition();
        }
    }
}
void Population::geneRecombination()
{
    double prob = rand() / double(RAND_MAX);
    if (prob < GENE_COMBA_RATE)
    {
        int index1 = 0;
        int index2 = 0;
        while (index1 == index2)
        {
            index1 = rand() % num;
            index2 = rand() % num;
        }
        int geneIndex = rand() % GENE_NUM;
        std::string subStr1 = individual[index1].content().substr(geneIndex * Gene::getLength(), Gene::getLength());
        std::string subStr2 = individual[index2].content().substr(geneIndex * Gene::getLength(), Gene::getLength());
        individual[index1].recombanation(geneIndex * Gene::getLength(), Gene::getLength(), subStr2);
        individual[index2].recombanation(geneIndex * Gene::getLength(), Gene::getLength(), subStr1);
    }
}
void Population::onePointRecombination()
{
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
        int pos = rand() % Individual::getLength();
        std::string subStr1 = individual[index1].content().substr(0, pos);
        std::string subStr2 = individual[index2].content().substr(0, pos);
        // 交换两个染色体的片段
        individual[index1].recombanation(0, pos, subStr2);
        individual[index2].recombanation(0, pos, subStr1);
    }
}
void Population::twoPointRecombination()
{
    double prob = rand() / double(RAND_MAX);
    if (prob < TWO_POINT_COMBA_RATE)
    {
        int index1 = 0;
        int index2 = 0;
        while (index1 == index2)
        {
            index1 = rand() % num;
            index2 = rand() % num;
        }
        int pos1 = rand() % Individual::getLength();
        int pos2 = rand() % (Individual::getLength() - pos1) + pos1;
        int len = pos2 - pos1 + 1;
        std::string subStr1 = individual[index1].content().substr(pos1, len);
        std::string subStr2 = individual[index2].content().substr(pos1, len);
        individual[index1].recombanation(pos1, len, subStr2);
        individual[index2].recombanation(pos1, len, subStr1);
    }
}
void Population::evolution()
{
    double sumOfFit = 0.0;
    for (int i = 0; i < INDIVIDUAL_NUM; i++)
    {
        sumOfFit += individual[i].getFitness();
    }
    std::vector<double> gambleRate;
    for (int i = 0; i < INDIVIDUAL_NUM; i++)
    {
        gambleRate.push_back(individual[i].getFitness() / sumOfFit);
    }
    //选择个体
    // 左值为左区间 右值为右区间
    std::vector<std::pair<double, double>> section;
    // 将概率展开为长度为1的线段 划分每个个体所占区间
    double temp = 0.0;
    for (int i = 0; i < INDIVIDUAL_NUM - 1; i++)
    {
        section.push_back(std::pair<double, double>(temp, temp + gambleRate[i]));
        if (i < INDIVIDUAL_NUM - 2)
            temp += gambleRate[i];
    }
    section.push_back(std::pair<double, double>(temp, 1.0));
    std::vector<std::string> context = {};
    // 保留最优个体，其余进行轮盘赌（包括最优个体）
    //string bestOne = bestIndiv();
    context.push_back(bestIndiv());
    for (int i = 0; i < INDIVIDUAL_NUM - 1; i++)
    {
        // 生成0~1间的随机算子
        double prob = rand() / double(RAND_MAX);
        for (int j = 0; j < INDIVIDUAL_NUM; j++)
        {
            // 如果落在该区间内 则加入该个体
            if (prob >= section[j].first && prob <= section[j].second)
            {
                context.push_back(individual[j].content());
                break;
            }
        }
    }
    for (int i = 0; i < num; i++)
    {
        individual[i].modifyContent(context[i]);
        individual[i].setDeadly(false); //重置致死性
    }
    //补齐中间步骤
    this->mutation();
    this->ISTransposition();
    this->RISTrasposition();
    this->geneTransposition();
    this->onePointRecombination();
    this->twoPointRecombination();
    this->geneRecombination();
    //context.push_back(bestOne);
    for (int i = 0; i < num; i++)
    {
        individual[i].recalculate();
    }
}

bool Population::excellentIndiv(double &maxValue, int &index, string &content,
                                string &contentWithDc)
{
    double temp = individual[0].getFitness();
    int idx = 0;
    // 找寻适宜度最高的个体
    for (int i = 1; i < INDIVIDUAL_NUM; i++)
    {
        if ((individual[i].getFitness() >= temp)&&(!individual[i].isDeadly()))
        {
            temp = individual[i].getFitness();
            idx = i;
        }
    }

    if (temp > maxValue)
    {
        index = idx + 1;
        content = individual[idx].showContent();
        contentWithDc = individual[idx].infixExpressionWithDc();
        maxValue = individual[idx].getFitness();
        return true;
    }
    return false;
}

string Population::bestIndiv()
{
    double temp = individual[0].getFitness();
    int idx = 0;
    for (int i = 1; i < INDIVIDUAL_NUM; i++)
    {
        if ((individual[i].getFitness() >= temp)&&(!individual[i].isDeadly()))
        {
            temp = individual[i].getFitness();
            idx = i;
        }
    }
    string ans = individual[idx].content();
    return ans;
}

void Population::display() const
{
    for (int i = 0; i < num; ++i)
    {
        std::cout << "[" <<setiosflags(ios::left)<<setw(2);
        cout.fill('0');
        cout<< i + 1 << "] " << individual[i].showContent() << endl;
    }
}