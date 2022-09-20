#ifndef GENE_H
#define GENE_H
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include "dataSource.h"
using namespace std;
class Gene
{
public:
    Gene(const Gene &obj);
    Gene(const std::string &str = "");
    Gene &operator=(const Gene &obj);
    ~Gene();
    void initialize();
    void mutation(double rate);
    void transposition(const std::string &str);
    char getRandomElement();
    char getTerminator();
    char getFunction();
    bool isFunc(char);
    bool isTerm(char);
    int findFunc(const int pos);
    void setContent(const string &content) { text = content; }
    static inline int getLength() { return gene_len; }
    std::string getContent() { return text; }
    static void DcInit();
    int maxParameter();
    double randDcValue();
    void saveDcValue();
    vector<double> expressionValue() { return result; };
    static void destroyDc();
    int sampleCount();
    int priority(char ch);
    void update();

    std::string validGene(); //返回有效基因片段
    double geneExpressing(int i, string validSegment, unordered_map<char, double> &termToValue); // i为样本索引
    std::string toExpression();
    unordered_set<int> getInvalidSamples() { return this->invalidSamples; }

private:
    std::string text;    // gene content
    static int tail_len; // tail length
    static int gene_len;
    std::vector<double> dc_value;
    static double *dc_array;
    vector<double> result;
    unordered_set<int> invalidSamples;
};

#endif