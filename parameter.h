#ifndef _PARAMETER_H_
#define _PARAMETER_H_
#include <string>

// gene parameter
const int HEAD_LEN = 7;
/* 默认采用绝对误差，flase即为相对误差 */
const bool AbsoluteError = true;
/* 选择范围 */
const double RANGE = 1000.0;
// 可选函数集: +、-、*、/、<、 >、&(AND)、|(OR)、 X(XOR)、S(sin)、C(cos)、T(tan)、E(exp)、L(log10)、Q(sqrt)
const char Function[] = {'-', '+', '*', '/'}; //,'>','<','&','|','X','^'};
const char Terminator[] = {'a','b','c','d','e','f','g','h','i','j'};
const bool Allow_Single_Gene = false;   //是否支持单符号基因
/* 分类阈值*/
const bool CLASSIFICATION = false;
const double THRESHOLD = 0.5;
//连接符号
const char CONN = '+';
const bool IS_OPEN_DC = 0;
const int DC_LEN = 10;
const double DC_MAX_VALUE = 10.0;
const double DC_MIN_VALUE = 0.0;
// individual parameter
const int GENE_NUM = 3;
const char CONNET = '+';
// population parameter
const int INDIVIDUAL_NUM = 100;
const int GENERATION = 50;
const double MUTATION_RATE = 0.044;
const double ONE_POINT_COMBA_RATE = 0.3;
const double TWO_POINT_COMBA_RATE = 0.3;
const double GENE_COMBA_RATE = 0.1;
const double GENE_TRANS_RATE = 0.1;
const double IS_TRANS_RATE = 0.1;
const int IS_ELEM_LEN[] = {1, 2, 3}; // random length set for IS transposition
const double RIS_TRANS_RATE = 0.1;
const int RIS_ELEM_LEN[] = {1, 2, 3}; // random length set for RIS transposition

/* 自变量存储文件 */
const std::string PATH_INDEPENDENT = "./data/input.txt";
/* 因变量存储文件 */
const std::string PATH_DEPENDENT = "./data/output.txt";

#endif