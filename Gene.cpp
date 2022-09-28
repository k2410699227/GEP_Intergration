#include <iostream>
#include <math.h>
#include <stack>
#include <vector>
#include <map>
#include "Gene.h"
#include "parameter.h"
#include "CalculateOperation.h"
using namespace std;
using namespace CalculateOperation;
int Gene::tail_len = 0; // tail length
int Gene::gene_len = 0;

Gene::Gene(const string &str, Parameter &p)
	: text(str), parameter(p)
{
	int m = maxParameter();
	tail_len = parameter.HEAD_LEN * (m - 1) + 1;
	gene_len = tail_len + parameter.HEAD_LEN;
}

Gene::Gene(Parameter &p)
{
	this->parameter = p;
	int m = maxParameter();
	tail_len = parameter.HEAD_LEN * (m - 1) + 1;
	gene_len = tail_len + parameter.HEAD_LEN;
}

Gene &Gene::operator=(const Gene &obj)
{
	if (this != &obj)
	{
		this->text = obj.text;
		this->tail_len = obj.tail_len;
		this->gene_len = obj.gene_len;
		this->parameter = obj.parameter;
	}
	return *this;
}
Gene::Gene(const Gene &obj) : text(obj.text)
{
}

void Gene::initialize()
{
	if (parameter.Allow_Single_Gene) //判断是否支持单符号基因
		text += getRandomElement();
	else
		text += getFunction();

	for (int i = 1; i < parameter.HEAD_LEN; i++)
	{
		text += getRandomElement();
	}
	for (int i = 0; i < tail_len; i++)
	{
		text += getTerminator();
	}

	int len_inden = DataSource::sampleCount();
	for (int i = 0; i < len_inden; i++)
	{
		double value_practise = geneExpressing(i, this->validGene(), DataSource::independent()[i]);
		result.push_back(value_practise);
	}
}

Gene::~Gene() {}

char Gene::getRandomElement()
{
	int index = rand() % 2;
	if (index)
		return getTerminator();
	else
		return getFunction();
}

char Gene::getTerminator()
{
	int length = parameter.Terminator.size();

	int ran = rand() % length;
	return parameter.Terminator[ran];
}

char Gene::getFunction()
{
	int length = parameter.Function.size();
	int ran = rand() % length;
	return parameter.Function[ran];
}

void Gene::mutation(double evo)
{
	for (int i = 0; i < gene_len; i++)
	{
		char ch;
		double prob = rand() % 100 / 100;
		if (prob < evo * parameter.MUTATION_RATE)
		{
			if ((i == 0) && !parameter.Allow_Single_Gene) //不支持单符号基因
				ch = getFunction();
			else if (i < parameter.HEAD_LEN)
				ch = getRandomElement();
			else
				ch = getTerminator();
			text[i] = ch;
		}
	}
}

void Gene::transposition(const string &str)
{
	int len = str.length();
	// 先删除头部结尾处len个长度的字符
	text.erase(parameter.HEAD_LEN - len, len);
	// 获取目前基因的头部长度
	int curr_head_len = text.length() - tail_len;
	// 产生除过头部首元素的随机位置
	int index = (rand() % curr_head_len) + 1;
	text.insert(index, str);
}

int Gene::findFunc(const int pos)
{
	for (int i = pos; i < parameter.HEAD_LEN; i++)
	{
		if (isFunc(text.at(i)))
			return i;
	}
	return -1;
}

bool Gene::isFunc(char elem)
{
	for (int i = 0; i < parameter.Function.size(); i++)
	{
		if (elem == parameter.Function[i])
			return true;
	}
	return false;
}
bool Gene::isTerm(char elem)
{
	for (int i = 0; i < parameter.Terminator.size(); i++)
	{
		if (elem == parameter.Terminator[i])
			return true;
	}

	return false;
}

int Gene::maxParameter()
{
	//获取最大参数个数
	int count = 0;

	for (int i = 0; i < parameter.HEAD_LEN; i++)
	{
		switch (parameter.Function[i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '<':
		case '>':
		case '&':
		case '|':
		case 'X':
			count = max(count, 2);
			break;
		case '^': // false
		case 'S': // sin
		case 'C': // cos
		case 'T': // tan
		case 'Q': // sqrt
		case 'E': // exp
		case 'L': // log10
			count = max(count, 1);
			break;
		}
	}
	return count;
}

int Gene::priority(char ch)
{
	if (ch == '+' || ch == '-')
		return 0;
	else if (ch == '*' || ch == '/')
		return 1;
	else if (ch == '(')
		return 3;
	else
		return 2;
}

string Gene::validGene()
{
	string validGene;
	string::iterator e = text.begin();
	string::iterator p = text.begin();
	while (distance(p, text.begin()) <= parameter.HEAD_LEN)
	{
		e = e + (paramNum(*p));
		p++;
		if (p > e)
		{
			break;
		}
	}
	for (string::iterator it = text.begin(); it != e; ++it)
	{
		validGene.push_back(*it);
	}
	validGene.push_back(*e);
	return validGene;
}

double Gene::geneExpressing(int index, string validSegment, unordered_map<char, double> &termToValue)
{

	vector<pair<char, double>> temp; //以(表达式，数值)的格式储存当前各节点的信息
	for (auto v : validSegment)		 //初始化各节点信息
	{
		if (isTerm(v))
			temp.push_back(pair<char, double>(v, termToValue[v])); //终结符的值为真实值
		else
			temp.push_back(pair<char, double>(v, 0)); //非终结符的值为0;
	}

	auto nonTerm = --temp.end(), term = --temp.end();

	while (temp.size() != 1)
	{
		while (isTerm(nonTerm->first))
			--nonTerm;
		double res = 0.0;
		switch (paramNum(nonTerm->first))
		{
		case 1:
			res = calculation(nonTerm->first, (term--)->second);
			nonTerm->first = parameter.Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
			nonTerm->second = res;
			temp.pop_back();
			break;
		case 2:
			// if(nonTerm->first=='/')
			res = calculation(nonTerm->first, (term--)->second, (term--)->second);
			nonTerm->first = parameter.Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
			nonTerm->second = res;
			temp.pop_back();
			temp.pop_back();
			break;
		case 3:
			res = calculation(nonTerm->first, (term--)->second, (term--)->second, (term--)->second);
			nonTerm->first = parameter.Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
			nonTerm->second = res;
			temp.pop_back();
			temp.pop_back();
			temp.pop_back();
			break;
		default:
			break;
		}
		if (isinf(res) || isnan(res)) //除数为零，表达式无意义，记录无效样本的索引
		{
			this->invalidSamples.insert(index);
			return 0.0;
		}

		nonTerm = --temp.end();
		term = --temp.end();
	}

	return temp.begin()->second;
}

std::string Gene::toExpression()
{
	vector<pair<char, std::string>> temp; //以(符号，表达式字符串)的格式储存当前各节点的信息
	for (auto v : this->validGene())	  //初始化各节点信息
	{
		std::string s;
		s.append(1, v);
		temp.push_back(pair<char, std::string>(v, s));
	}

	auto nonTerm = --temp.end(), term = --temp.end();

	while (temp.size() != 1)
	{
		while (isTerm(nonTerm->first))
			--nonTerm;
		std::string res = "";
		switch (paramNum(nonTerm->first))
		{
		case 1:
			res = getExpression(nonTerm->first, (term--)->second);
			temp.pop_back();
			break;
		case 2:
			// if(nonTerm->first=='/')
			res = getExpression(nonTerm->first, (term--)->second, (term--)->second);
			temp.pop_back();
			temp.pop_back();
			break;
		case 3:
			res = getExpression(nonTerm->first, (term--)->second, (term--)->second, (term--)->second);
			temp.pop_back();
			temp.pop_back();
			temp.pop_back();
			break;
		default:
			break;
		}

		nonTerm->first = parameter.Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
		nonTerm->second = res;

		nonTerm = --(--temp.end());
		term = --temp.end();
	}

	return temp.begin()->second;
}

void Gene::update()
{
	result.clear();
	this->invalidSamples.clear(); //清除无效样本记录

	int len_inden = DataSource::sampleCount();
	for (int i = 0; i < len_inden; i++)
	{
		double value_practise = geneExpressing(i, this->validGene(), DataSource::independent()[i]);
		result.push_back(value_practise);
	}
}