#include<iostream>
#include<fstream>
#include<cstring>
#include<windows.h>  
#include<commdlg.h>
#include<stdio.h>
#include<io.h>
#include<Windows.h>
using namespace std;

int num, char_num, word_num, line_num, code_line, blank_line, comment_line, id_num; //������Ӧ����Ŀ
bool start; //һ����ʼ״̬��һЩ�����Ҫʹ��
char token[20], rwtab[10][20]; //token����ʱ�ַ��飬rwtab������ͣ�ôʱ�����Ĵ�

bool isLetter(char c) { //�ж��Ƿ�Ϊ��ĸ
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')) {
		return true;
	}
	else
		return false;
}

bool isDigit(char c) { //�ж��Ƿ�Ϊ����
	if (c >= '0'&&c <= '9') {
		return true;
	}
	else
		return false;
}

void test_c(char *str) { //�ַ�����
	char c;
	char_num = 0; //��ʼ���ַ�������
	ifstream infile(str); //���ļ�
	if (!infile) { //����ļ��Ƿ��
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) { //���ļ��ж�ȡ����
		char_num++; //ÿ��ȡһ���ַ��ͼ�һ
	}
	cout << str << "," << "�ַ��� = " << char_num << endl;
	infile.close(); //�ر��ļ�
}

void test_w(char *str) { //���ʺ���
	char c;
	word_num = 0;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	c = infile.get();
	while (c != EOF) {
		while (c == ' ' || c == '\n' || c == '\t') { //��������������ž�����
			c = infile.get();
		}

		if (isLetter(c)) { //���Ϊ����ʱ�Ĵ������
			do {
				c = infile.get(); //�������״̬ʱ���ʽ�ֹ
			} while (c != '\n'&&c != '\t'&&c != ' '&&c != ';'&&c != ','&&c != '('&&c != ')'&&c != EOF);   //��ʶ��
			word_num++; //��������һ
			continue;
		}
		c = infile.get();
	}
	cout << str << "," << "������ = " << word_num << endl;
	infile.close();
}

void test_l(char *str) { //��������
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++; //���ֻ��б��һ
		}
	}
	cout << str << "," << "���� = " << line_num << endl;
	infile.close();
}

void test_ll(char *str) { //���������Ŀ�������
	char c;
	line_num = 1;
	ifstream infile(str);
	if (!infile) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) {
		if (c == '\n') {
			line_num++; //�����������
		}
	}
	infile.close();
}

void test_a(char *str) { //������/����/ע���еļ���
	test_ll(str); //����������
	num = 0; code_line = 1; comment_line = 0; //��ʼ��
	char c;
	int sum = 0, sum1 = 0;
	ifstream infile(str);
	ifstream infile2(str);
	if (!infile || !infile2) {
		cout << "error" << endl;
	}
	while ((c = infile.get()) != EOF) { //�������
		if (c != '\t'&&c != ' ') {
			num++;
		}
		if (c == '\n') {
			num--;
			if (num <= 1) { //�ַ����ڵ���һ��Ϊ����
				blank_line++;
			}
			num = 0;
		}
	}
	while ((c = infile2.get()) != EOF) { //����ע����
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
				if (sum1 > 1) { //���˵Ļ���ԭ������������ĳ�м���ע����Ҳ�Ǵ����У�����ͨ��sum1���������֣���ϸ����������ԭ��������д
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
								comment_line--; //��Ϊ*/codeline�����ڴ����У����Գ����������ʱ����Ҫȥ��
								break;
							}
							else {
								break;
							}
						}
						else {
							cout << "ע�ͷ�����" << endl;
						}
					}
				}
			}
		}	
	}
	code_line = line_num - comment_line - blank_line + sum; //��������еĹ�ʽ
	cout << str << "," << "������/����/ע���� = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	infile.close();
	infile2.close();
}

void test_e(char *str) { //ͣ�ôʱ��ʹ�ã��뵥�ʺ�����ͬС��
	char c;
	int m, n, sum;
	id_num = 0;
	ifstream infile(str);
	ifstream infile2("stoplist.txt"); //����ͣ�ôʱ�
	if (!infile || !infile2) {
		cout << "error" << endl;
	}

	m = n = sum = 0;
	c = infile2.get();
	while (c != EOF) { //��ȡͣ�ôʱ�����ĵ���
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
			token[m] = '\0'; //��ʱ����
		}
		m = 0;

		while (c == ' ' || c == '\n' || c == '\t') {
			c = infile.get();
		}

		if (isLetter(c)) {
			do {
				token[m++] = c;
				c = infile.get();
			} while (isLetter(c) || isDigit(c)); //��ʶ��
			token[m] = '\0';
			for (n = 0; n < sum; n++) {
				if (strcmp(token, rwtab[n]) == 0) { //���бȽϣ��������ͣ�ôʱ��򲻽��м���
					id_num--;
					break;
				}
			}
			id_num++;
			continue;
		}
		c = infile.get();
	}
	cout << str << "," << "��ʶ�� = " << id_num << endl;
	infile.close();
	infile2.close();
}

void test_o(char *str) { //-o�������
	ofstream outfile(str,ios::app);
	outfile << "�ַ��� = " << char_num << endl;
	outfile << "������ = " << word_num << endl;
	outfile << "���� = " << line_num << endl;
	outfile << "������/����/ע���� = " << code_line << "/" << blank_line << "/" << comment_line << endl;
	outfile << "��ʶ�� = " << id_num << endl;
	outfile.close();
}

void main() {
	char ch;
	char name[10][1000]; //�����ļ���
	bool s1, s2, s3, s4, s5, s6, s7, s8, s9; //����ÿ�����ܵ�״̬
	s1 = s2 = s3 = s4 = s5 = s6 = s7 = s8 = s9 = false; //��ʼ��
	int sum = 0, i;
	num = 0;
	while (1) { //��������м�⣬����ͨ���س���������
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
			else if (ch == 'z') { //����exe����
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

	if (s9 == true) { //*.***���ܵ�ʹ�ã��������к�׺��Ϊ***���ļ�
		const char *to_search = name[0];        //�����ҵ��ļ���֧��ͨ���
		start = false;
		if (sum > 1) {
			start = true;
			strcpy(name[9], name[sum - 1]);
		}
		long handle;                                                //���ڲ��ҵľ��
		struct _finddata_t fileinfo;                          //�ļ���Ϣ�Ľṹ��
		handle = _findfirst(to_search, &fileinfo);         //��һ�β���
		if (-1 == handle) {
			return;
		}
		sum = 0;
		strcpy(name[sum++], fileinfo.name);              //�ҵ����ļ������и�ֵ
		while (!_findnext(handle, &fileinfo))               //ѭ�������������ϵ��ļ���ֱ���Ҳ���������Ϊֹ
		{
			strcpy(name[sum++], fileinfo.name);
		}
		if (start == true) {
			strcpy(name[sum++], name[9]);
		}
		_findclose(handle);                                      //�رվ��
	}

	if (s8 == false) { //�����жϣ����Ҿ��������ĸ�����
		if (s6 == false && s1 == true && s9 == false) { //���û��-s��Ĭ��ֻʹ�õ�һ���ļ�
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

		if (s6 == true || s9 == true) { //��-s����ݹ�����ļ�
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
	else { //�ʼ�Ƕ�-x���жϣ���Ϊ�����-x����ô�����Լ���ѡ���ļ�������������ܵ��ж���Ҫ�ŵ�һ��������ǵ��ô��ļ�����Ĵ���
		OPENFILENAME ofn;      // �����Ի���ṹ��     
		TCHAR szFile[MAX_PATH]; // �����ȡ�ļ����ƵĻ�������               
								// ��ʼ��ѡ���ļ��Ի���     
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
		// ��ʾ��ѡ���ļ��Ի���     

		if (GetOpenFileName(&ofn))
		{
			//��ʾѡ����ļ���     
			cout << "�ɹ����ļ���" << szFile << endl;
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