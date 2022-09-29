#ifndef _PARAMETER_H_
#define _PARAMETER_H_
#include <string>
#include <vector>

class Parameter
{
public:
    Parameter()
    {
        IS_ELEM_LEN = {1, 2, 3};
        RIS_ELEM_LEN = {1, 2, 3};
        Function = {'-', '+', '*', '/'};
        Terminator = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
    }

    // gene parameter
    int HEAD_LEN = 5;
    /* 默认采用绝对误差，flase即为相对误差 */
    bool AbsoluteError = true;
    /* 选择范围 */
    double RANGE = 100.0;
    // 可选函数集: +、-、*、/、<、 >、&(AND)、|(OR)、 X(XOR)、S(sin)、C(cos)、T(tan)、E(exp)、L(log10)、Q(sqrt)
    std::vector<char> Function;    // {'-', '+', '*', '/'},'>','<','&','|','X','^'};
    std::vector<char> Terminator;  //终结符号集合
    bool Allow_Single_Gene = false; //是否支持单符号基因
    int Invalid_Sample_Ratio = 2;  //允许的最大无效样本比例，如2表示当无效样本比例超过1/2时，个体致死

    /* 分类阈值*/
    bool CLASSIFICATION = true;
    double THRESHOLD = 0.5;

    bool IS_OPEN_DC = 0;
    int DC_LEN = 10;
    double DC_MAX_VALUE = 10.0;
    double DC_MIN_VALUE = 0.0;

    // individual parameter
    int GENE_NUM = 5;
    char CONNET = '+'; //连接符号

    // population parameter
    int INDIVIDUAL_NUM = 100;
    int GENERATION = 1000;
    double MUTATION_RATE = 0.044;
    double ONE_POINT_COMBA_RATE = 0.3;
    double TWO_POINT_COMBA_RATE = 0.3;
    double GENE_COMBA_RATE = 0.1;
    double GENE_TRANS_RATE = 0.1;
    double IS_TRANS_RATE = 0.1;
    std::vector<int> IS_ELEM_LEN; // random length set for IS transposition
    double RIS_TRANS_RATE = 0.1;
    std::vector<int> RIS_ELEM_LEN; // random length set for RIS transposition

    /* 训练集存储文件 */
    std::string PATH_TRAIN = "";
    /* 测试集存储文件 */
    std::string PATH_EVALUATION = "";
    double targetAccuracy = 0.7; //每个分类器在训练集上的准确率
    int classifierCount = 100;   //挑选分类器的数量
    int leastGeneration = 50;    //判断是否需要重新开始的最小代数
};

#endif