#include<iostream>
#include<fstream>
#include<cstring>
#include<windows.h>  
#include<commdlg.h>
#include<stdio.h>
#include<io.h>
#include<Windows.h>
using namespace std;

int num, char_num, word_num, line_num, code_line, blank_line, comment_line, id_num;
bool start;
char token[20], rwtab[10][20];

bool isLetter(char c) {
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')) {
		return true;
	}
	else
		return false;
}

bool isDigit(char c) {
	if (c >= '0'&&c <= '9') {
		return true;
	}
	else
		return false;
}

void test_c(char *str) {
	char c;
	char_num = 0;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		char_num++;
	}
	cout << str << "," << "字符数 = " << char_num << endl;
	infile.close();
}

void test_w(char *str) {
	char c;
	word_num = 0;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	c = infile.get();
	while (c != EOF) {
		while (c == ' ' || c == '\n' || c == '\t') {
			c = infile.get();
		}

		if (isLetter(c)) {
			do {
				c = infile.get();
			} while (c != '\n'&&c != '\t'&&c != ' '&&c != ';'&&c != ','&&c != '('&&c != ')'&&c != EOF);   //标识符
			word_num++;
			continue;
		}
		c = infile.get();
	}
	cout << str << "," << "单词数 = " << word_num << endl;
	infile.close();
}

void test_l(char *str) {
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++;
		}
	}
	cout << str << "," << "行数 = " << line_num << endl;
	infile.close();
}

void test_ll(char *str) {
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++;
		}
	}
	infile.close();
}

void test_a(char *str) {
	test_ll(str);
	num = 0; code_line = 1; comment_line = 0;
	char c;
	int sum = 0, sum1 = 0;
	ifstream infile(str);
	ifstream infile2(str);
	if (!infile || !infile2) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c != '\t'&&c != ' ') {
			num++;
		}
		if (c == '\n') {
			num--;
			if (num <= 1) {
				blank_line++;
			}
			num = 0;
		}
	}
	while ((c = infile2.get()) != EOF) {
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
				if (sum1 > 1) {
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
								comment_line--;
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
	code_line = line_num - comment_line - blank_line + sum;
	cout << str << "," << "代码行/空行/注释行 = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	infile.close();
	infile2.close();
}

void test_e(char *str) {
	char c;
	int m, n, sum;
	id_num = 0;
	ifstream infile(str);
	ifstream infile2("stoplist.txt");
	if (!infile || !infile2) {
		cout << "error" << endl;
	}

	m = n = sum = 0;
	c = infile2.get();
	while (c != EOF) {
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
			token[m] = '\0';
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
				if (strcmp(token, rwtab[n]) == 0) {
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

void test_o(char *str) {
	ofstream outfile(str);
	outfile << "字符数 = " << char_num << endl;
	outfile << "单词数 = " << word_num << endl;
	outfile << "行数 = " << line_num << endl;
	outfile << "代码行/空行/注释行 = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	outfile << "标识符 = " << id_num << endl;
	outfile.close();
}

void main() {
	char ch;
	char name[10][1000];
	bool s1, s2, s3, s4, s5, s6, s7, s8, s9;
	s1 = s2 = s3 = s4 = s5 = s6 = s7 = s8 = s9 = false;
	int sum = 0, i;
	num = 0;
	while (1) {
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
			else if (ch == 'z') {
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

	if (s9 == true) {
		const char *to_search = name[0];        //欲查找的文件，支持通配符
		char *search;
		start = false;
		if (sum > 1) {
			start = true;
			search = name[1];
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
			strcpy(name[sum++], search);
		}
		_findclose(handle);                                      //关闭句柄
	}

	if (s8 == false) {
		if (s6 == false && s1 == true && s9 == false) {
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

		if (s6 == true || s9 == true) {
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

					ofstream outfile(name[sum - 1]);
					outfile << name[i] << ":" << endl;
					outfile.close();
					test_o(name[sum - 1]);
				}
			}
		}

		if (s7 == true && s6 == false) {
			ofstream outfile(name[sum - 1]);
			outfile << name[0] << ":" << endl;
			outfile.close();
			test_o(name[sum - 1]);
		}
	}
	else {
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