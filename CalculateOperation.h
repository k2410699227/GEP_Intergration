#ifndef CALCULATEOPERATION_H
#define CALCULATEOPERATION_H
#include <string>
namespace CaculatiOperation{
    double twoAdd(double a,double b){
        return a+b;
    }
    int twoAdd(int a,int b){
        return a+b;
    }
    float twoAdd(float a,float b){
        return a+b;
    }
    int twoSubtract(int a,int b){
        return a-b;
    }
    double twoSubtract(double a,double b){
        return a-b;
    }
    float twoSubtract(float a,float b){
        return a-b;
    }
    double twoMultiply(double a,double b){
        return a*b;
    }
    float twoMultuply(float a,float b){
        return a*b;
    }
    double twoDivide(double a,double b){
        return a/b;
    }
    float twoDIvide(float a,float b){
        return a/b;
    }
    bool bigger(double a,double b){
        return a>b;
    }
    bool smaller(double a,double b){
        return a<b;
    }
    double twoAnd(double a,double b){
        if(a>0&&b>0)
            return 1;
        return 0;
    }
    double twoOR(double a,double b){
        if(a>0 || b > 0)
            return 1;
        return 0;
    }
    double twoXor(double a,double b){
        if(a>0 && b>0)
            return 0;
        else if(a<0 && b<0)
            return 0;
        else
            return 1;
    }
    double twoBigger(double a,double b){
        if(a > b)
            return 1;
        else
            return 0;
    }
    double twoSmaller(double a,double b){
        if(a < b)
            return 1;
        else
            return 0;
    }
    /* bool twoAnd(double a,double b){
        if(a > 0)
    } */
    //返回操作符所需的参数个数
    int paramNum(char ch){
        switch(ch){
            case '+':
            case '-':
            case '*':
            case '/':
            case '>':
            case '<':
            case '=':
            case '&':
            case '|':
            case 'X':
                return 2;
                break;
            default:
                return 0;
        }
    }
    //单目运算
    double calculation(char ch,double a){}
    //双目运算
    double calculation(char ch,double a,double b){
        switch (ch)
        {
        case '+':
            return twoAdd(a,b);
            break;
        case '-':
            return twoSubtract(a,b);
            break;
        case '*':
            return twoMultiply(a,b);
            break; 
        case '/':
            return twoDivide(a,b);
        case '<':
            return twoBigger(a,b);
        case '>':
            return twoSmaller(a,b);
        case '&':
            return twoAnd(a,b);
        case '|':
            return twoOR(a,b);
        case 'X':
            return twoXor(a,b);
        default:
            break;
        }
    }
    //三目运算
    double calculation(char ch,double a,double b,double c){}
    //返回用于校验的表达式字符串
    std::string getExpression(char ch,std::string a,std::string b){
        return '('+a+ch+b+')';
    }
    std::string getExpression(char ch,std::string a){
        std::string res = "";
        return "("+ch+a+")";
    }
    std::string getExpression(char ch,std::string a,std::string b,std::string c){};
    
};

#endif
