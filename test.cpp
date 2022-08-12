#include<iostream>
#include"test.h"
#include <vector>
using namespace std;
#include "Gene.h"

int main(){
	Gene test("+a*babba");
	vector<char> ve = test.caculateValidLength();
	for(vector<char>::iterator it = ve.begin();it != ve.end();++it){
		cout << *it;
	}
}
