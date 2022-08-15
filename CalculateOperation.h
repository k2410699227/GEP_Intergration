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
            twoAdd(a,b);
            break;
        case '-':
            twoSubtract(a,b);
            break;
        case '*':
            twoMultiply(a,b);
            break;;
        case '/':
            twoDivide(a,b);
        default:
            break;
        }
    }
    //三目运算
    double calculation(char ch,double a,double b,double c){}
    //返回用于校验的表达式字符串
    std::string showString(char ch,std::string a,std::string b){
        switch (ch)
        {
        case '+':
        case '-':
        case '*':
        case '/':
            return a+ch+b;
            break;
        default:
            break;
        }
    }
};