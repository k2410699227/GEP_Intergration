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
    int paramNum(char ch){
        switch(ch){
            case '+':
            case '-':
            case '*':
            case '/':
                return 2;
                break;
        }
    }
    double caculation(char ch,double a){}
};