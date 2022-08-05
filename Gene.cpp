#include <iostream>
#include <math.h>
#include "Gene.h"
#include "parallel.h"
using namespace std;

Gene::Gene(const string& str)
:text(str)
{
    gene_len = tail_len + head_len;
}

void Gene::initialize(){
    for (int i = 0;i<head_len;i++){
        text += getRandomElement();
    }
    for (int i= 0;i<tail_len;i++){
        text += getTerminator();
    }
    //dc next
}

Gene::Gene(const Gene& obj):text(obj.text)
{}

char Gene::getRandomElement(){
    int index = rand() % 2;
	if (index)
		return getTerminator();
	else
		return getNTerminator();
}

char Gene::getTerminator(){
    int length = sizeof(Terminator)/sizeof(char);
    int ran = rand() % length;
    return Terminator[ran];
}

char Gene::getNTerminator(){
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