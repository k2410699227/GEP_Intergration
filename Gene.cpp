#include <iostream>
#include <math.h>
#include <stack>
#include <vector>
#include <map>
#include "Gene.h"
#include "parameter.h"
#include "CalculateOperation.h"
using namespace std;
using namespace CaculatiOperation;
double *Gene::dc_array = nullptr;
int Gene::tail_len = 0; // tail length
int Gene::gene_len = 0;

Gene::Gene(const string &str)
	: text(str)
{
	int m = maxParameter();
	tail_len = HEAD_LEN * (m - 1) + 1;
	gene_len = tail_len + HEAD_LEN;
}

Gene &Gene::operator=(const Gene &obj)
{
	if (this != &obj)
	{
		this->text = obj.text;
		dc_value = obj.dc_value;
	}
	return *this;
}
Gene::Gene(const Gene &obj) : text(obj.text), dc_value(obj.dc_value)
{
}

void Gene::initialize()
{
	if (Allow_Single_Gene) //判断是否支持单符号基因
		text += getRandomElement();
	else
		text += getFunction();

	for (int i = 1; i < HEAD_LEN; i++)
	{
		text += getRandomElement();
	}
	for (int i = 0; i < tail_len; i++)
	{
		text += getTerminator();
	}
	// dc next
	if (IS_OPEN_DC)
		saveDcValue();

	// // 解码表达式为中缀表达式
	// string expression = decode();
	// // 中缀转后缀
	// queue<char> postfix_str = infix2postfix(expression);
	// 计算训练样本
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
	int length = sizeof(Terminator) / sizeof(char);
	if (!IS_OPEN_DC)
	{

		int ran = rand() % length;
		return Terminator[ran];
	}
	else
	{
		// 如果开启了Dc域 长度加1
		vector<char> dc_term;
		for (int i = 0; i < length; i++)
		{
			dc_term.push_back(Terminator[i]);
		}
		dc_term.push_back('?');
		int index = rand() % (length + 1);
		return dc_term[index];
	}
}

char Gene::getFunction()
{
	int length = sizeof(Function) / sizeof(char);
	int ran = rand() % length;
	return Function[ran];
}

void Gene::mutation(double evo)
{
	for (int i = 0; i < gene_len; i++)
	{
		char ch;
		double prob = rand() % 100 / 100;
		if (prob < evo * MUTATION_RATE)
		{
			if ((i == 0) && !Allow_Single_Gene) //不支持单符号基因
				ch = getFunction();
			else if (i < HEAD_LEN)
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
	text.erase(HEAD_LEN - len, len);
	// 获取目前基因的头部长度
	int curr_head_len = text.length() - tail_len;
	// 产生除过头部首元素的随机位置
	int index = (rand() % curr_head_len) + 1;
	text.insert(index, str);
}

int Gene::findFunc(const int pos)
{
	for (int i = pos; i < HEAD_LEN; i++)
	{
		if (isFunc(text.at(i)))
			return i;
	}
	return -1;
}

bool Gene::isFunc(char elem)
{
	for (int i = 0; i < sizeof(Function) / sizeof(char); i++)
	{
		if (elem == Function[i])
			return true;
	}
	return false;
}
bool Gene::isTerm(char elem)
{
	for (int i = 0; i < sizeof(Terminator) / sizeof(char); i++)
	{
		if (elem == Terminator[i])
			return true;
	}
	if (IS_OPEN_DC && elem == '?')
		return true;

	return false;
}
void Gene::DcInit()
{
	if (IS_OPEN_DC)
	{
		// 分配Dc域内存
		dc_array = new double[DC_LEN];
		// 初始化Dc域元素数值大小
		int minValue = int(DC_MIN_VALUE * 1000);
		int maxValue = int(DC_MAX_VALUE * 1000);
		for (int i = 0; i < DC_LEN; i++)
		{
			int value = (rand() % (maxValue - minValue + 1)) + minValue;
			dc_array[i] = value / 1000.0;
		}
	}
}

int Gene::maxParameter()
{
	//获取最大参数个数
	int count = 0;

	for (int i = 0; i < HEAD_LEN; i++)
	{
		switch (Function[i])
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

int Gene::parameterCount(char c)
{
	switch (c)
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
		return 2;
		break;
	case '^': // false
	case 'S': // sin
	case 'C': // cos
	case 'T': // tan
	case 'Q': // sqrt
	case 'E': // exp
	case 'L': // log10
		return 1;
		break;
	}
	return 1;
}

double Gene::randDcValue()
{
	int index = rand() % DC_LEN;
	return dc_array[index];
}

void Gene::saveDcValue()
{
	for (string::iterator it = text.begin(); it != text.end(); ++it)
	{
		if (*it == '?')
		{
			double temp = randDcValue();
			dc_value.push_back(temp);
		}
	}
}

void Gene::destroyDc()
{
	if (dc_array != nullptr)
		delete[] dc_array;
}

//单参数函数值计算
double Gene::mathExpression(char symbol, double value_r)
{
	double value = 0.0;
	switch (symbol)
	{
	case '^':
		value = value_r > 0.0 ? 0.0 : 1.0;
	case 'S': // sin
		value = sin(value_r);
		break;
	case 'C': // cos
		value = cos(value_r);
		break;
	case 'T': // tan
		value = tan(value_r);
		break;
	case 'Q': // sqrt
		value = sqrt(fabs(value_r));
		break;
	case 'E': // exp
		value = exp(value_r);
		break;
	case 'L': // log10
		value = log10(fabs(value_r));
		break;
	default:
		break;
	}
	if (isfinite(value) || isinf(value) || isnan(value))
		value = 0.0;
	return value;
}

//双参数函数输出值计算
double Gene::mathExpression(double value_l, char symbol, double value_r)
{
	double value = 0.0;
	switch (symbol)
	{
	case '+':
		value = value_l + value_r;
		break;
	case '-':
		value = value_l - value_r;
		break;
	case '*':
		value = value_l * value_r;
		break;
	case '/':
		if (value_l == 0.0)
		{
			value = 0.0;
			break;
		}
		value = value_r / value_l;
		break;
	case '>':
		value = value_l > value_r ? 1.0 : 0.0;
		break;
	case '<':
		value = value_l < value_r ? 1.0 : 0.0;
		break;
	case '&':
		value = (value_l > 0.0 && value_r > 0.0) ? 1.0 : 0.0;
		break;
	case '|':
		value = (value_l == 0.0 && value_r == 0.0) ? 0.0 : 1.0;
		break;
	case 'X':
		value = ((value_l > 0.0 && value_r > 0.0) || (value_l == 0.0 && value_r == 0.0)) ? 0.0 : 1.0;
		break;
	default:
		break;
	}
	return value;
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
	while (distance(p, text.begin()) <= HEAD_LEN)
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

std::queue<char> Gene::infix2postfix(string expression)
{
	// 先将字符串表达式依次入队
	queue<char> temp;
	for (string::iterator it = expression.begin(); it != expression.end(); ++it)
	{
		temp.push(*it);
	}

	queue<char> postfix;
	stack<char> charStack;
	while (!temp.empty())
	{
		// 弹出字符串队列的队头元素
		char ch = temp.front();
		temp.pop();
		if (isTerm(ch))
		{
			// 如果是运算数 直接入队
			postfix.push(ch);
		}
		else if (ch == '(')
		{
			// 如果是左括号 压入堆栈
			charStack.push(ch);
		}
		else if (ch == ')')
		{
			// 如果是右括号 弹出栈顶运算符并输出 直到遇到左括号(出栈 不输出)
			char elem;
			while (!charStack.empty() && (elem = charStack.top()) != '(')
			{
				postfix.push(elem);
				charStack.pop();
			}
			// 弹出左括号
			if (!charStack.empty())
				charStack.pop();
		}
		else if (isFunc(ch))
		{
			// 如果是运算符
			// 如果堆栈为空，或栈顶运算符为左括号“(”，则直接将此运算符入栈
			// 当优先级大于栈顶运算符时，则把它压栈；
			// 当优先级小于或等于栈顶运算符时，将栈顶运算符弹出并输出；
			// 再比较新的栈顶运算符，直到该运算符大于栈顶运算符优先级为止，然后将该运算符压栈
			if (charStack.empty() || charStack.top() == '(')
			{
				charStack.push(ch);
			}
			else
			{
				int curPriority = priority(ch);
				int topPriority = -1;
				;
				while (!charStack.empty() && curPriority <= (topPriority = priority(charStack.top())))
				{
					char c = charStack.top();
					charStack.pop();
					postfix.push(c);
				}
				charStack.push(ch);
			}
		}
	}
	// 最后弹出栈内剩余元素
	while (!charStack.empty())
	{
		postfix.push(charStack.top());
		charStack.pop();
	}
	return postfix;
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
			temp.pop_back();
			break;
		case 2:
			// if(nonTerm->first=='/')
			res = calculation(nonTerm->first, (term--)->second, (term--)->second);
			temp.pop_back();
			temp.pop_back();
			break;
		case 3:
			res = calculation(nonTerm->first, (term--)->second, (term--)->second, (term--)->second);
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
		nonTerm->first = Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
		nonTerm->second = res;

		nonTerm = --(--temp.end());
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

		nonTerm->first = Terminator[0]; //已计算出真实值的非终结符替换为第一位终结符，对计算结果无影响
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

	// 生成并存储Dc域数据
	if (IS_OPEN_DC)
		saveDcValue();

	// // 解码表达式为中缀表达式
	// string expression = decode();
	// // 中缀转后缀
	// queue<char> postfix_str = infix2postfix(expression);
	// 后缀计算
	int len_inden = DataSource::sampleCount();
	for (int i = 0; i < len_inden; i++)
	{
		double value_practise = geneExpressing(i, this->validGene(), DataSource::independent()[i]);
		result.push_back(value_practise);
	}
}