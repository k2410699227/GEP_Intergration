#include <iostream>
#include <math.h>
#include <stack>
#include <map>
#include "Gene.h"
#include "parameter.h"
#include "CaculateOperation.h"
using namespace std;
using namespace CaculatiOperation;
double *Gene::dc_array = nullptr;
int Gene::tail_len = 0; // tail length
int Gene::gene_len = 0;

Gene::Gene(const string &str)
	: text(str),
	  tree(NULL)
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
	for (int i = 0; i < HEAD_LEN; i++)
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

	// 解码表达式为中缀表达式
	string expression = decode();
	// 中缀转后缀
	queue<char> postfix_str = infix2postfix(expression);
	// 后缀计算 计算训练样本
	int len_inden = DataSource::independent().size();
	for (int i = 0; i < len_inden; i++)
	{
		double value_practise = calculate(postfix_str, DataSource::independent()[i]);
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

void Gene::mutation()
{
	int index = rand() % gene_len;
	char ch;
	if (index < HEAD_LEN)
		ch = getRandomElement();
	else
		ch = getTerminator();
	text[index] = ch;
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

vector<char> Gene::validGene(){
	vector<char> validGene;
	cout << "caclulating valid length:" <<endl;
	string::iterator e = text.begin();
	cout << "e = " << *e << endl;
	string::iterator p = text.begin();
	cout << "p = " << *p << endl;
	while (distance(p,text.begin()) <= HEAD_LEN){
		e = e+(paramNum(*p));
		p++;
		if(p>e){
			break;
		}
		
	}
	for(string::iterator it = text.begin();it != e;++it){
		validGene.push_back(*it);
		cout << "push back:" << *it << endl;
	}
	validGene.push_back(*e);
	cout << "push back:" << *e << endl;
	//return e;
	//return distance(e,text.begin());
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

string Gene::decode()
{
	queue<char> queue = {};
	int index = 0; // 基因片段字符串的下标索引
	tree = new BinaryTree(text.at(index));
	queue.push(text.at(index));
	index++;
	// 构造二叉树
	while (!queue.empty())
	{
		char ch = queue.front();
		queue.pop();
		if (isTerm(ch))
			continue;
		else
		{
			int count = parameterCount(ch);
			BinaryTreeNode *parent = tree->Find(ch);
			for (int i = 0; i < count; i++)
			{
				char value = text.at(index);
				if (count == 1)
					tree->Insert(parent, value, false);
				else
					tree->Insert(parent, value);
				index++;
				queue.push(value);
			}
		}
	}
	// 中序遍历二叉树
	string result = tree->Output();
	delete tree;
	return result;
}

string Gene::decodeWithDc()
{
	string expression = decode();
	int flag = 0;
	string::size_type index;
	while ((index = expression.find('?')) != string::npos)
	{
		string temp = std::to_string(dc_value[flag++]);
		expression.replace(expression.find('?'), 1, temp);
	}
	return expression;
}
void Gene::update()
{
	result.clear();

	// 生成并存储Dc域数据
	if (IS_OPEN_DC)
		saveDcValue();

	// 解码表达式为中缀表达式
	string expression = decode();
	// 中缀转后缀
	queue<char> postfix_str = infix2postfix(expression);
	// 后缀计算
	int len_inden = DataSource::independent().size();
	for (int i = 0; i < len_inden; i++)
	{
		double value_practise = calculate(postfix_str, DataSource::independent()[i]);
		result.push_back(value_practise);
	}
}
double Gene::calculate(queue<char> postfix, unordered_map<char, double> value)
{
	stack<double> temp;
	double result = 0.0;
	int flag = 0;
	char ch = postfix.front();
	if(postfix.size()==1)	//单元素基因
		return postfix.front();
	while (!postfix.empty())
	{
		char ch = postfix.front();
		if (isTerm(ch))
		{
			// 如果是“?”则从存储的Dc域数据中选择其值
			if (IS_OPEN_DC && ch == '?')
			{
				temp.push(dc_value[flag++]);
			}
			else
			{
				// 若是数字则进栈
				temp.push(value.at(ch));
			}
		}
		else if (isFunc(ch))
		{
			// 如果是运算符 则弹出n个其需要的数字
			int num = parameterCount(ch);
			if (num == 1)
			{
				double value = temp.top();
				temp.pop();
				result = mathExpression(ch, value);
			}
			else if (num == 2)
			{
				double valuel = temp.top();
				temp.pop();
				double valuer = temp.top();
				temp.pop();
				result = mathExpression(valuel, ch, valuer);
			}
			// 运算结果进栈
			temp.push(result);
		}
		postfix.pop();
	}
	return result;
}