#include <string>
#include <vector>
#include <queue>
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
        static inline int getLength() {return gene_len;}
        std::string getContent() {return text;}
        void DcInit();
        int maxParameter();
        double randDcValue();
        void saveDcValue();
        vector<double> expressionValue(){return result;};
        void destroyDc();
        double mathExpression(double value_l, char symbol, double value_r);
        int sampleCount();
        int priority(char ch);
        std::queue<char> infix2postfix(string expression);
    private:
        std::string text;           //gene content
        const int head_len = 5;     //head length
        static int tail_len;     //tail length
        static int gene_len;
        std::vector<double> dc_value;
        static double* dc_array;
        vector<double> result;
};