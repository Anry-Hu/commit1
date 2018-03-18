  编码环境是C++。
  第一次实现了基本功能，从文件中读取数据，求出字符数、单词数、行数。
  这里的单词指的并不是标识符，只要通过','和几个控制符相隔开的，都算作单词，标识符以后会做。
  难点：单词的统计较难，其它两个直接计算便可。在统计单词的时候，必须把每种情况都考虑进去，比如文件最开始是空格，最开始是字符这些不同的情况，包括结尾的不同也有可能导致不同。
  代码如下：
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void test-c() {
	char c;
	int char_num, word_num, line_num;
	char_num = word_num = line_num = 0;
	bool state, start;
	state = start = true;
	ifstream infile("file.txt");
	ofstream outfile("outfile.txt");
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		char_num++;
		if (c == '\n') {
			line_num++;
		}
		if (c == '\n' || c == '\t' || c == ' ' || c == ',') {
			state = false;
			start = false;
			continue;
		}
		else if (state == false) {
			state = true;
			word_num++;
		}
		else if (start == true && state == true) {
			start = false;
			word_num++;
		}
	}
	outfile << "line_num = " << line_num << endl;
	outfile << "word_num = " << word_num << endl;
	outfile << "char_num = " << char_num << endl;
	infile.close();
}
