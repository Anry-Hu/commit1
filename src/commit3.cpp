#include<iostream>
#include<fstream>
#include<cstring>
#include<windows.h>  
#include<commdlg.h>
#include<stdio.h>
#include<io.h>
#include<Windows.h>
using namespace std;

int num, char_num, word_num, line_num, code_line, blank_line, comment_line, id_num; //储存相应的数目
bool start; //一个开始状态，一些情况需要使用
char token[20], rwtab[10][20]; //token是临时字符组，rwtab来保存停用词表里面的词

bool isLetter(char c) { //判断是否为字母
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')) {
		return true;
	}
	else
		return false;
}

bool isDigit(char c) { //判断是否为数字
	if (c >= '0'&&c <= '9') {
		return true;
	}
	else
		return false;
}

void test_c(char *str) { //字符函数
	char c;
	char_num = 0; //初始化字符的数量
	ifstream infile(str); //打开文件
	if (!infile) { //检测文件是否打开
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) { //从文件中读取数据
		char_num++; //每读取一个字符就加一
	}
	cout << str << "," << "字符数 = " << char_num << endl;
	infile.close(); //关闭文件
}

void test_w(char *str) { //单词函数
	char c;
	word_num = 0;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	c = infile.get();
	while (c != EOF) {
		while (c == ' ' || c == '\n' || c == '\t') { //如果是这三个符号就跳过
			c = infile.get();
		}

		if (isLetter(c)) { //检测为单词时的处理情况
			do {
				c = infile.get(); //到下面的状态时单词截止
			} while (c != '\n'&&c != '\t'&&c != ' '&&c != ';'&&c != ','&&c != '('&&c != ')'&&c != EOF);   //标识符
			word_num++; //单词数加一
			continue;
		}
		c = infile.get();
	}
	cout << str << "," << "单词数 = " << word_num << endl;
	infile.close();
}

void test_l(char *str) { //行数函数
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++; //出现换行变加一
		}
	}
	cout << str << "," << "行数 = " << line_num << endl;
	infile.close();
}

void test_ll(char *str) { //行数函数的拷贝函数
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++; //这里少了输出
		}
	}
	infile.close();
}

void test_a(char *str) { //代码行/空行/注释行的计算
	test_ll(str); //总行数计算
	num = 0; code_line = 1; comment_line = 0; //初始化
	char c;
	int sum = 0, sum1 = 0;
	ifstream infile(str);
	ifstream infile2(str);
	if (!infile || !infile2) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) { //计算空行
		if (c != '\t'&&c != ' ') {
			num++;
		}
		if (c == '\n') {
			num--;
			if (num <= 1) { //字符少于等于一便为空行
				blank_line++;
			}
			num = 0;
		}
	}
	while ((c = infile2.get()) != EOF) { //计算注释行
		if (c != '\t'&&c != ' '&&c != '/'&&c != '*') {
			sum1++;
		}
		if (c == '\n') {
			sum1 = 0;
		}
		
		if (c == '/') {
			c = infile2.get();
			if (c == '/') {
				comment_line++;
				if (sum1 > 1) { //本人的划分原则里，有种情况是某行既是注释行也是代码行，这里通过sum1来进行区分，详细的行数划分原则上面已写
					sum++;
				}
			}
			else if (c == '*') {
				comment_line++;
				if (sum1 > 1) {
					sum++;
				}
				while ((c = infile2.get()) != EOF) {
					if (c == '\n') {
						comment_line++;
						continue;
					}
					else if (c == '*') {
						c = infile2.get();
						if (c == '/') {
							c = infile2.get();
							if (c == EOF) {
								break;
							}
							if (c != '\n') {
								comment_line--; //因为*/codeline是属于代码行，所以出现这种情况时，需要去掉
								break;
							}
							else {
								break;
							}
						}
						else {
							cout << "注释符出错" << endl;
						}
					}
				}
			}
		}	
	}
	code_line = line_num - comment_line - blank_line + sum; //计算代码行的公式
	cout << str << "," << "代码行/空行/注释行 = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	infile.close();
	infile2.close();
}

void test_e(char *str) { //停用词表的使用，与单词函数大同小异
	char c;
	int m, n, sum;
	id_num = 0;
	ifstream infile(str);
	ifstream infile2("stoplist.txt"); //遍历停用词表
	if (!infile || !infile2) {
		cout << "error" << endl;
	}

	m = n = sum = 0;
	c = infile2.get();
	while (c != EOF) { //获取停用词表里面的单词
		if (isLetter(c)) {
			do {
				rwtab[sum][n++] = c;
				c = infile2.get();
			} while (isLetter(c));
			rwtab[sum][n] = '\0';
			sum++;
			n = 0;
			continue;
		}
		c = infile2.get();
	}

	c = infile.get();
	while (c != EOF) {
		for (m = 0; m < 20; m++) {
			token[m] = '\0'; //临时单词
		}
		m = 0;

		while (c == ' ' || c == '\n' || c == '\t') {
			c = infile.get();
		}

		if (isLetter(c)) {
			do {
				token[m++] = c;
				c = infile.get();
			} while (isLetter(c) || isDigit(c)); //标识符
			token[m] = '\0';
			for (n = 0; n < sum; n++) {
				if (strcmp(token, rwtab[n]) == 0) { //进行比较，如果属于停用词表，则不进行计算
					id_num--;
					break;
				}
			}
			id_num++;
			continue;
		}
		c = infile.get();
	}
	cout << str << "," << "标识符 = " << id_num << endl;
	infile.close();
	infile2.close();
}

void test_o(char *str) { //-o输出功能
	ofstream outfile(str,ios::app);
	outfile << "字符数 = " << char_num << endl;
	outfile << "单词数 = " << word_num << endl;
	outfile << "行数 = " << line_num << endl;
	outfile << "代码行/空行/注释行 = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	outfile << "标识符 = " << id_num << endl;
	outfile.close();
}

void main() {
	char ch;
	char name[10][1000]; //储存文件名
	bool s1, s2, s3, s4, s5, s6, s7, s8, s9; //保存每个功能的状态
	s1 = s2 = s3 = s4 = s5 = s6 = s7 = s8 = s9 = false; //初始化
	int sum = 0, i;
	num = 0;
	while (1) { //对输入进行检测，并且通过回车结束输入
		ch = getchar();
		if (ch == ' ' || ch == '\t') {
			continue;
		}
		else if (ch == '-') {
			ch = getchar();
			if (ch == 'c') {
				s1 = true;
			}
			else if (ch == 'w') {
				s2 = true;
			}
			else if (ch == 'l') {
				s3 = true;
			}
			else if (ch == 'a') {
				s4 = true;
			}
			else if (ch == 'e') {
				s5 = true;
			}
			else if (ch == 's') {
				s6 = true;
			}
			else if (ch == 'o') {
				s7 = true;
			}
			else if (ch == 'x') {
				s8 = true;
			} 
			else if (ch == 'z') { //调用exe程序
				WinExec("MFCtesting.exe", SW_SHOWNORMAL);
				return;
			}
		}
		else if (ch == '\n') {
			break;
		}
		else {
			if (ch == '*') {
				s9 = true;
			}
			name[sum][num++] = ch;
			while (1) {
				ch = getchar();
				if (ch == ' ' || ch == '\t' || ch == '\n') {
					break;
				}
				else {
					name[sum][num++] = ch;
				}
			}
			name[sum][num] = '\0';
			sum++;
			num = 0;
		}
	}

	if (s9 == true) { //*.***功能的使用，遍历所有后缀名为***的文件
		const char *to_search = name[0];        //欲查找的文件，支持通配符
		start = false;
		if (sum > 1) {
			start = true;
			strcpy(name[9], name[sum - 1]);
		}
		long handle;                                                //用于查找的句柄
		struct _finddata_t fileinfo;                          //文件信息的结构体
		handle = _findfirst(to_search, &fileinfo);         //第一次查找
		if (-1 == handle) {
			return;
		}
		sum = 0;
		strcpy(name[sum++], fileinfo.name);              //找到的文件名进行赋值
		while (!_findnext(handle, &fileinfo))               //循环查找其他符合的文件，直到找不到其它的为止
		{
			strcpy(name[sum++], fileinfo.name);
		}
		if (start == true) {
			strcpy(name[sum++], name[9]);
		}
		_findclose(handle);                                      //关闭句柄
	}

	if (s8 == false) { //进行判断，并且决定调用哪个功能
		if (s6 == false && s1 == true && s9 == false) { //如果没有-s，默认只使用第一个文件
			test_c(name[0]);
		}

		if (s6 == false && s2 == true && s9 == false) {
			test_w(name[0]);
		}

		if (s6 == false && s3 == true && s9 == false) {
			test_l(name[0]);
		}

		if (s6 == false && s4 == true && s9 == false) {
			test_a(name[0]);
		}

		if (s6 == false && s5 == true && s9 == false) {
			test_e(name[0]);
		}

		if (s6 == true || s9 == true) { //有-s，则递归调用文件
			if (s7 == false) {
				for (i = 0; i < sum; i++) {
					if (s1 == true) {
						test_c(name[i]);
					}

					if (s2 == true) {
						test_w(name[i]);
					}

					if (s3 == true) {
						test_l(name[i]);
					}

					if (s4 == true) {
						test_a(name[i]);
					}

					if (s5 == true) {
						test_e(name[i]);
					}
				}
			}
			else if (s7 == true) {
				for (i = 0; i < sum - 1; i++) {
					if (s1 == true) {
						test_c(name[i]);
					}

					if (s2 == true) {
						test_w(name[i]);
					}

					if (s3 == true) {
						test_l(name[i]);
					}

					if (s4 == true) {
						test_a(name[i]);
					}

					if (s5 == true) {
						test_e(name[i]);
					}

					ofstream outfile(name[sum - 1],ios::app);
					outfile << name[i] << ":" << endl;
					outfile.close();
					test_o(name[sum - 1]);
				}
			}
		}

		if (s7 == true && s6 == false) {
			ofstream outfile(name[sum - 1],ios::app);
			outfile << name[0] << ":" << endl;
			outfile.close();
			test_o(name[sum - 1]);
		}
	}
	else { //最开始是对-x的判断，因为如果有-x，那么就是自己来选择文件，所以这个功能的判断需要放第一，下面就是调用打开文件程序的代码
		OPENFILENAME ofn;      // 公共对话框结构。     
		TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。               
								// 初始化选择文件对话框。     
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);    
		// 显示打开选择文件对话框。     

		if (GetOpenFileName(&ofn))
		{
			//显示选择的文件。     
			cout << "成功打开文件：" << szFile << endl;
		}
		if (s1 == true) {
			test_c(szFile);
		}
		if (s2 == true) {
			test_w(szFile);
		}

		if (s3 == true) {
			test_l(szFile);
		}

		if (s4 == true) {
			test_a(szFile);
		}

		if (s5 == true) {
			test_e(szFile);
		}
		if (s7 == true) {
			ofstream outfile(name[0]);
			outfile << szFile << ":" << endl;
			outfile.close();
			test_o(name[0]);
		}
	}

	system("pause");
}