#include <iostream>
#include <math.h>
#include "Gene.h"
#include "parameter.h"
using namespace std;
double* Gene::dc_array = nullptr;

Gene::Gene(const string& str)
:text(str)
{
	int m = maxParameter();
	tail_len = HEAD_LEN * (m - 1) + 1;
    gene_len = tail_len + head_len;
}

Gene& Gene::operator=(const Gene& obj)
{
	if (this != &obj)
	{
		this->text = obj.text;
		dc_area = obj.dc_area;
	}
	return *this;
}
Gene::Gene(const Gene& obj):text(obj.text),dc_area(obj.dc_area)
{}

void Gene::initialize(){
    for (int i = 0;i<head_len;i++){
        text += getRandomElement();
    }
    for (int i= 0;i<tail_len;i++){
        text += getTerminator();
    }
    //dc next
}

Gene::~Gene(){}

char Gene::getRandomElement(){
    int index = rand() % 2;
	if (index)
		return getTerminator();
	else
		return getFunction();
}

char Gene::getTerminator(){
    int length = sizeof(Terminator)/sizeof(char);
    int ran = rand() % length;
    return Terminator[ran];
}

char Gene::getFunction(){
    int length = sizeof(Function)/sizeof(char);
    int ran = rand() % length;
    return Function[ran];
}

void Gene::mutation(){
    int index = rand() % gene_len;
	char ch;
	if (index < HEAD_LEN)
		ch = getRandomElement();
	else
		ch = getTerminator();
	text[index] = ch;
}

void Gene::transposition(const string& str){
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

void Gene::DcInit(){
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

int Gene::maxParameter(){
	int max = 0;
	for(int i =0;i<(sizeof(Function)/sizeof(char));i++){
		switch(Function[i]){
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
				if(max < 2){
					max = 2;
				}
				break;
		}
	}
	return max;
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
			dc_area.push_back(temp);
		}
	}
}

void Gene::destroyDc()
{
	if (dc_array != nullptr)
		delete[] dc_array;
}

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
		if(value_l == 0.0)
		{
			value = 0.0;
			break;
		}
		value = value_r / value_l;
		break;
	case '>':
		value = value_l > value_r ? 1 : 0;
		break;
	case '<':
		value = value_l < value_r ? 1 : 0;
		break;
	default:
		break;
	}
	return value;
}