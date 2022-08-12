#ifndef GENE_H
#define GENE_H
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include "dataSource.h"
#include "BinaryTree.h"
using namespace std;
class Gene{
    public:
        Gene(const Gene& obj);
        Gene(const std::string& str = "");
        Gene& operator=(const Gene& obj);
        ~Gene();
        void initialize();
        void mutation();
        void transposition(const std::string& str);
        char getRandomElement();
        char getTerminator();
        char getFunction();
        bool isFunc(char);
        bool isTerm(char);
        int findFunc(const int pos);
        void setContent(const string& content) { text = content; }
        static inline int getLength() {return gene_len;}
        std::string getContent() {return text;}
        static void DcInit();
        int maxParameter();
        //函数参数数量
        int parameterCount(char);
        double randDcValue();
        void saveDcValue();
        vector<double> expressionValue(){return result;};
        static void destroyDc();
        double mathExpression(char symbol, double value_r);
        double mathExpression(double value_l, char symbol, double value_r);
        int sampleCount();
        int priority(char ch);
        void update();

        vector<char> validGene();
        
        std::queue<char> infix2postfix(string expression);  //中序转为后序表达式
        string decode();    //将进化出的层序表达式解码为中序表达
        string decodeWithDc();
        double calculate(queue<char> postfix, unordered_map<char, double> value);     //计算基因的数值  
    private:
        std::string text;           //gene content
        static int tail_len;     //tail length
        static int gene_len;
        BinaryTree* tree;   //以二叉树形式储存生成的表达式
        std::vector<double> dc_value;
        static double* dc_array;
        vector<double> result;
        vector<int> validLength; //有效长度
};

#endif