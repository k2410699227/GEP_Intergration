//gene parallel
const int HEAD_LEN = 7;
char NTerminator[] = {'^','+','|'};
char Terminator[] = {'a'};
//individual parallel
int GENE_NUM = 3;
//population parallel
/* 变异概率 */
const double PROB_MUTATION = 1;
/* IS转座率 */
const double PROB_IS_TRANS = 1;
/* IS元素长度 */
const int IS_LEN[] = { 1,2,3 };
/* RIS转座率 */
const double PROB_RIS_TRANS = 1;
/* RIS元素长度 */
const int RIS_LEN[] = { 1,2,3 };
/* 基因转座率 */
const double PROB_GENE_TRANS = 1;