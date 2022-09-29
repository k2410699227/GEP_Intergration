#include "individual.h"
int Individual::len = 0;
Individual::Individual(Parameter &p) : gene(nullptr)
{
	this->parameter = p;
	gene = new Gene[parameter.GENE_NUM];
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		gene[i] = Gene(this->parameter);
	}
	len = parameter.GENE_NUM * Gene::getLength();
};

Individual::Individual(const Individual &obj)
	: gene(new Gene[obj.parameter.GENE_NUM]), parameter(obj.parameter)
{
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		*(gene + i) = *(obj.gene + i);
	}
}

Individual &Individual::operator=(const Individual &ind)
{
	this->parameter = ind.parameter;
	gene = new Gene[ind.parameter.GENE_NUM];
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		*(gene + i) = *(ind.gene + i);
	}
	return *this;
}

Individual::~Individual()
{
	if (gene != nullptr)
	{
		delete[] gene;
	}
}

void Individual::initialize()
{
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		gene[i].initialize();
	}
	calculate();
	fit();
}

void Individual::calculate()
{
	std::vector<std::vector<double>> param = {};
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		//合并各基因无效样本的索引值
		allInvalidSamples.insert(gene[i].getInvalidSamples().begin(), gene[i].getInvalidSamples().end());
		param.push_back(gene[i].expressionValue());
	}
	//当无效样本数大于样本总数的一半时，标记为致死个体
	if (allInvalidSamples.size() > DataSource::sampleCount() / parameter.Invalid_Sample_Ratio)
	{
		this->setDeadly(true);
		return;
	}

	int len = DataSource::sampleCount();
	for (int i = 0; i < len; ++i)
	{
		double value = 0.0;
		if (allInvalidSamples.find(i) != allInvalidSamples.end())
		{
			result.push_back(0.0);
			continue;
		}
		if (parameter.CONNET == '*' || parameter.CONNET == '/')
			value = 1.0;
		for (int j = 0; j < parameter.GENE_NUM; j++)
		{
			switch (parameter.CONNET)
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

		if (parameter.CLASSIFICATION)					 //分类任务
			value = value > parameter.THRESHOLD ? 1 : 0; //当表达式大于阈值，分类结果是1，否则为0；
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

	if (parameter.CLASSIFICATION)
	{
		int t = 0;
		for (int i = 0; i < num; i++)
		{
			if (allInvalidSamples.find(i) != allInvalidSamples.end())
				continue;
			t += (result[i] == DataSource::dependent()[i]) ? 1 : 0;
		}
		//分类适应度计算
		if (t > (num / 2))
		{
			int f = t / (1.0 - (double)allInvalidSamples.size() / (double)DataSource::sampleCount());
			fitness = f;
		}
		else
			fitness = 1;
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			double temp = 0.0;
			if (allInvalidSamples.find(i) != allInvalidSamples.end()) //无效样本，不需要计算
				continue;
			if (parameter.AbsoluteError)
			{

				// 采用绝对误差：选择范围 - |适应度值 - 目标值|
				temp = parameter.RANGE - abs(result[i] - DataSource::dependent()[i]);
			}
			else
			{
				// 采用相对误差：选择范围 - |（适应度值 - 目标值）/ 目标值 * 100|
				temp = parameter.RANGE - abs(100 * (result[i] - DataSource::dependent()[i]) / DataSource::dependent()[i]);
			}
			if (temp <= 0)
				temp = 0.1; // 方便轮盘赌计算

			fitness += temp;
		}
		fitness = fitness / (1 - (double)allInvalidSamples.size() / (double)DataSource::sampleCount());
	}
}

void Individual::modifyContent(pair<std::string, double> content)
{
	evolutionRatio = content.second;
	string text[parameter.GENE_NUM];
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		text[i] = content.first.substr(i * Gene::getLength(), Gene::getLength());
		gene[i].setContent(text[i]);
	}
}
void Individual::recalculate()
{
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		gene[i].update();
	}

	result.clear();
	this->allInvalidSamples.clear(); //清除无效样本记录
	fitness = 0.0;
	// 计算表达式数值
	calculate();
	// 计算拟合误差
	fit();
}

int Individual::index_rand()
{
	return rand() % parameter.GENE_NUM;
}
void Individual::mutation()
{
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		gene[i].mutation(evolutionRatio);
	}
}
void Individual::ISTransposition()
{
	// 随机选取转座长度
	int len_array = parameter.IS_ELEM_LEN.size();
	int index = rand() % len_array;
	int len_is = parameter.IS_ELEM_LEN[index];
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
	int pos = rand() % parameter.HEAD_LEN;
	// 沿该点向后查找直到发现一个函数 该点即为RIS转座的起始点
	int a;
	if ((a = gene[index].findFunc(pos)) == -1)
		return;
	pos = a + Gene::getLength() * index;
	// 随机选取RIS转座长度
	int len_array = parameter.RIS_ELEM_LEN.size();
	index = rand() % len_array;
	int len_ris = parameter.RIS_ELEM_LEN[index];
	// 从起始点向后取len_ris个长度的片段
	std::string str = content().substr(index, len_ris);
	index = index_rand();
	gene[index].transposition(str);
}

std::string Individual::content()
{
	std::string content;
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		content += gene[i].getContent();
	}
	return content;
}

std::string Individual::getValidGenes() const
{
	std::string res;
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		res += gene[i].validGene();
		if (i < parameter.GENE_NUM - 1)
			res += ' ';
	}
	return res;
}
void Individual::geneTransposition()
{
	if (parameter.GENE_NUM > 1)
	{
		int index1 = 0, index2 = 0;
		while (index1 == index2)
		{
			index1 = index_rand(); // gene choose
			index2 = index_rand(); // position choose
		}
		if (index1 < index2)
		{
			Gene temp(gene[index1]);
			for (int i = index1; i < index2; i++)
			{
				gene[i] = gene[i + 1];
			}
			gene[index2] = temp;
		}
		else
		{
			Gene temp(gene[index1]);
			for (int i = index2; i < index1; i++)
			{
				gene[i + 1] = gene[i];
			}
			gene[index2] = temp;
		}
	}
}
void Individual::recombanation(const int pos, const int length, const std::string &str)
{
	std::string text = content();
	text.replace(pos, length, str);
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		Gene temp(text.substr(i * Gene::getLength(), Gene::getLength()), parameter);
		gene[i] = temp;
	}
}

std::string Individual::showContent() const
{
	std::string str;
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		str = str + gene[i].getContent() + " ";
	}
	if (this->deadly)
		str = str + "  dead";
	else
		str = str + " [" + std::to_string(fitness) + "]";
	return str + ' ' + std::to_string(this->allInvalidSamples.size());
}

string Individual::infixExpressionWithDc()
{
	string expression = "";
	for (int i = 0; i < parameter.GENE_NUM; i++)
	{
		if (i < parameter.GENE_NUM - 1)
			expression = expression + gene[i].toExpression() + ' ' + parameter.CONNET + ' ';
		else
			expression = expression + gene[i].toExpression();
	}
	return expression;
}