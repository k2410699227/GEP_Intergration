#include <string>
#include <vector>
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
        int findFunc(const int pos);
        static inline int getLength() {return gene_len;}
        std::string getContent() {return text;}
        void DcInit();
        int maxParameter();
        double randDcValue();
        void saveDcValue();
        vector<double> expressionValue(){return result;};
    private:
        std::string text;           //gene content
        const int head_len = 5;     //head length
        static int tail_len;     //tail length
        static int gene_len;
        std::vector<double> dc_area;
        static double* dc_array;
        vector<double> result;
};