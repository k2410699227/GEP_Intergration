#include "individual.h"
int Individual::len = 0;
Individual::Individual() : gene(nullptr)
{
	gene = new Gene[GENE_NUM];
	len = GENE_NUM * Gene::getLength();
};

Individual::Individual(const Individual &obj)
	: gene(new Gene[GENE_NUM])
{
	for (int i = 0; i < GENE_NUM; i++)
	{
		*(gene + i) = *(obj.gene + i);
	}
}

Individual::~Individual()
{
	if (!gene)
	{
		delete[] gene;
	}
}

void Individual::initialize()
{
	for (int i = 0; i < GENE_NUM; i++)
	{
		gene[i].initialize();
	}
	caculate();
	fit();
}

void Individual::caculate()
{
	std::vector<std::vector<double>> param = {};
	for (int i = 0; i < GENE_NUM; i++)
	{
		//判断是否有致死基因
		if (gene[i].isDeadly())
			this->deadly = true;
		param.push_back(gene[i].expressionValue());
	}

	int len = DataSource::sampleCount();
	for (int i = 0; i < len; ++i)
	{
		double value = 0.0;
		double a = 0.0;
		for (int j = 0; j < GENE_NUM; j++)
		{
			switch (CONNET)
			{
			case '+':
				value += param[j].at(i);
				break;
			case '-':
				value -= param[j].at(i);
				break;
			case '*':
				value *= param[j].at(i);
				break;
			case '/':
				value /= param[j].at(i);
				if (isnan(value) || isinf(value))
					value = 0.0;
				break;
			default:
				break;
			}
		}

		if (CLASSIFICATION)					   //分类任务
			value = value > THRESHOLD ? 1 : 0; //当表达式大于阈值，分类结果是1，否则为0；
		result.push_back(value);
	}
}

void Individual::fit()
{
	//致死个体适应度为0，在轮盘赌时不会被选择,
	if (this->isDeadly())
	{
		fitness = 0.0;
		return;
	}
	int num = DataSource::sampleCount();

	if (CLASSIFICATION)
	{
		int t = 0;
		for (int i = 0; i < num; i++)
		{
			t += (result[i] == DataSource::dependent()[i]) ? 1 : 0;
		}
		fitness = t > (num / 2) ? t : 1;
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			double temp = 0.0;
			if (AbsoluteError)
			{
				// 采用绝对误差：选择范围 - |适应度值 - 目标值|
				temp = RANGE - abs(result[i] - DataSource::dependent()[i]);
			}
			else
			{
				// 采用相对误差：选择范围 - |（适应度值 - 目标值）/ 目标值 * 100|
				temp = RANGE - abs(100 * (result[i] - DataSource::dependent()[i]) / DataSource::dependent()[i]);
			}
			if (temp <= 0)
				temp = 0.1; // 方便轮盘赌计算

			fitness += temp;
		}
	}
}

void Individual::modifyContent(string content)
{
	string text[GENE_NUM];
	for (int i = 0; i < GENE_NUM; i++)
	{
		text[i] = content.substr(i * Gene::getLength(), Gene::getLength());
		gene[i].setContent(text[i]);
		gene[i].setDeadly(false); //重置致死性
	}
}
void Individual::recalculate()
{
	for (int i = 0; i < GENE_NUM; i++)
	{
		gene[i].update();
	}

	result.clear();
	fitness = 0.0;
	// 计算表达式数值
	caculate();
	// 计算拟合误差
	fit();
}

int Individual::index_rand()
{
	return rand() % GENE_NUM;
}
void Individual::mutation()
{
	for(int i =0;i<GENE_NUM;i++){
		gene[i].mutation();
	}
}
void Individual::ISTransposition()
{
	// 随机选取转座长度
	int len_array = sizeof(IS_ELEM_LEN) / sizeof(int);
	int index = rand() % len_array;
	int len_is = IS_ELEM_LEN[index];
	// 从整段染色体中随机选取len_is个长度的片段
	index = rand() % (len - len_is);				   // 随机起始索引
	std::string str = content().substr(index, len_is); // 截取字符串
	// 随机选择基因
	index = index_rand();
	gene[index].transposition(str);
}
void Individual::RISTransposition()
{
	// 随机选择基因
	int index = index_rand();
	// 在头中随机选取一点
	int pos = rand() % HEAD_LEN;
	// 沿该点向后查找直到发现一个函数 该点即为RIS转座的起始点
	int a;
	if ((a = gene[index].findFunc(pos)) == -1)
		return;
	pos = a + Gene::getLength() * index;
	// 随机选取RIS转座长度
	int len_array = sizeof(RIS_ELEM_LEN) / sizeof(int);
	index = rand() % len_array;
	int len_ris = RIS_ELEM_LEN[index];
	// 从起始点向后取len_ris个长度的片段
	std::string str = content().substr(index, len_ris);
	index = index_rand();
	gene[index].transposition(str);
}

std::string Individual::content()
{
	std::string content;
	for (int i = 0; i < GENE_NUM; i++)
	{
		content += gene[i].getContent();
	}
	return content;
}

void Individual::geneTransposition()
{
	if (GENE_NUM > 1)
	{
		int index1 = 0, index2 = 0;
		while (index1 == index2)
		{
			index1 = index_rand();
			index2 = index_rand();
		}
		Gene temp(gene[index1]);
		gene[index1] = gene[index2];
		gene[index2] = temp;
	}
}
void Individual::recombanation(const int pos, const int length, const std::string &str)
{
	std::string text = content();
	text.replace(pos, length, str);
	for (int i = 0; i < GENE_NUM; i++)
	{
		Gene temp(text.substr(i * Gene::getLength(), Gene::getLength()));
		gene[i] = temp;
	}
}

std::string Individual::showContent() const
{
	std::string str;
	for (int i = 0; i < GENE_NUM; i++)
	{
		str = str + gene[i].getContent() + " ";
	}
	if(this->deadly)
		str = str + "  dead";
	else
		str = str + " [" + std::to_string(fitness) + "]";
	return str;
}

string Individual::infixExpressionWithDc()
{
	string expression = "";
	for (int i = 0; i < GENE_NUM; i++)
	{
		if (i < GENE_NUM - 1)
			expression = expression + gene[i].decodeWithDc() + ' ' + CONN + ' ';
		else
			expression = expression + gene[i].decodeWithDc();
	}
	return expression;
}