//1.替代密码，凯撒密码
#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;
void tidai(string mess, int Key, int n, char* c) {
	transform(mess.begin(), mess.end(), mess.begin(), ::toupper);
	//cout << mess << endl;
	map<int, char> mmp1;
	mmp1[0] = 'A'; mmp1[1] = 'B'; mmp1[2] = 'C'; mmp1[3] = 'D'; mmp1[4] = 'E'; mmp1[5] = 'F'; mmp1[6] = 'G'; mmp1[7] = 'H';
	mmp1[8] = 'I'; mmp1[9] = 'J'; mmp1[10] = 'K'; mmp1[11] = 'L'; mmp1[12] = 'M'; mmp1[13] = 'N'; mmp1[14] = 'O'; mmp1[15] = 'P';
	mmp1[16] = 'Q'; mmp1[17] = 'R'; mmp1[18] = 'S'; mmp1[19] = 'T'; mmp1[20] = 'U'; mmp1[21] = 'V'; mmp1[22] = 'W'; mmp1[23] = 'X';
	mmp1[24] = 'Y'; mmp1[25] = 'Z';
	map<char, int> mmp2;
	mmp2['A'] = 0; mmp2['B'] = 1; mmp2['C'] = 2; mmp2['D'] = 3; mmp2['E'] = 4; mmp2['F'] = 5;
	mmp2['G'] = 6; mmp2['H'] = 7; mmp2['I'] = 8; mmp2['J'] = 9; mmp2['K'] = 10; mmp2['L'] = 11;
	mmp2['M'] = 12; mmp2['N'] = 13; mmp2['O'] = 14; mmp2['P'] = 15; mmp2['Q'] = 16; mmp2['R'] = 17;
	mmp2['S'] = 18; mmp2['T'] = 19; mmp2['U'] = 20; mmp2['V'] = 21; mmp2['W'] = 22; mmp2['X'] = 23;
	mmp2['Y'] = 24; mmp2['Z'] = 25;

	map<int, char>::iterator iter1;
	map<char, int>::iterator iter2;
	//cout<<"test\n"<<endl; 

	int i = 0;
	while (mess[i] != '\0') {
		//cout<<"yes";
		if (mess[i] == ' ') {
			i++;
			continue;
		}

		iter2 = mmp2.find(mess[i]);
		if (iter2 == mmp2.end()) {
			//cout << "error1 input" << endl;
			break;
		}
		int temp = (iter2->second + Key) % n;
		//cout << temp << endl;
		iter1 = mmp1.find(temp);
		if (iter1 == mmp1.end()) {
			cout << "error2 input" << endl;
			break;
		}
		c[i] = iter1->second;
		//cout << c[i] << endl;
		i++;
	}
};

void KKey(string key1, vector<int>& NumArray) {
	transform(key1.begin(), key1.end(), key1.begin(), ::toupper);//大小写一致
	//cout << key1 << endl;
	int len = key1.length();//密钥的长度
	set<char> s;
	int i = 0;
	//将密钥逐字符放入set中，进行自动排序
	while (key1[i] != '\0') {
		s.insert(key1[i]);
		i++;
	}
	string sschar = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//cout << sschar << endl;
	set<char> ss;//存放二十六字母
	i = 0;
	while (sschar[i] != '\0')
	{
		ss.insert(sschar[i]);
		i++;
	}
	//存放每个字符在密钥中的字母序
	//vector<int> NumArray;//可能要换一种结构，可以直接得到一共插入多少个数字
	i = 0;
	set<char>::iterator iter;

	while (key1[i] != '\0') {
		//如果密钥有重复的字母，字母仅使用一次
		bool present = ss.count(key1[i]) != 0;
		if (!present) {
			i++;
			continue;
		}
		int j = 0;
		for (iter = s.begin(); iter != s.end(); iter++) {
			if (*iter == key1[i]) {
				NumArray.push_back(j);
				break;
			}
			j++;
		}
		ss.erase(key1[i]);
		i++;
	}
	//cout << "test<" << endl;
	//for (int j = 0; j < NumArray.size(); j++) {
		//cout << NumArray[j];
	//}
	//cout << endl;
}
void zhihuan(char* mess1, int len2, char* c2, vector<int> NumArray) {

	//得到NumArray实际的大小
	int TheRealLen = NumArray.size();
	//cout << "the real len:" << TheRealLen << endl;
	//len(mess1)%TheRealLen!=0,则该部分不做置换
	//int len2 = mess1.length();
	char* c1 = new char[len2];
	//cout << "len1:" << len2 << endl;
	int cishu = len2 / TheRealLen;//一共做多少次
	//cout << "cishu" << cishu << endl;
	int sheyu = len2 % TheRealLen;
	vector<vector<char>> ccc, ccc1, ccc2;
	for (int i = 0; i < cishu; ++i) {
		vector<char> tmp(TheRealLen, ' '); //定义int型一维数组tmp，该数组有n个int型元素，且每个元素的初始值为value
		ccc.push_back(tmp); //将一维数组tmp（小容器）加入matrix（大容器）中，使之成为matrix的元素，令matrix成为二维数组
		ccc1.push_back(tmp);
		ccc2.push_back(tmp);
	}

	//	char** ccc; //如何声明一个未知大小的二维数组
	//	char** ccc1; //如何声明一个未知大小的二维数组
	//	char** ccc2; //如何声明一个未知大小的二维数组
	//
	//	ccc = new char* [cishu];
	//	ccc1 = new char* [cishu];
	//	ccc2 = new char* [cishu];

//	for (int i = 0; i < TheRealLen; i++) {
//		ccc[i] = new char[TheRealLen];
//		ccc1[i] = new char[TheRealLen];
//		ccc2[i] = new char[TheRealLen];
//	}
	int i = 0;
	for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			ccc[j][k] = mess1[i];
			//cout << ccc[j][k];
			i++;
		}
	}
	/*for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			//ccc[j][k] = mess1[i];
			cout << ccc[j][k];
			//i++;
		}
	}*/

	for (int k = 0; k < TheRealLen; k++) {
		for (int j = 0; j < cishu; j++) {
			ccc1[j][k] = ccc[j][NumArray[k]];
		}
	}
	/*for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			//ccc[j][k] = mess1[i];
			cout << ccc1[j][k];
			//i++;
		}
	}*/
	for (int k = 0; k < TheRealLen; k++) {
		for (int j = 0; j < cishu; j++) {
			ccc2[j][k] = ccc1[j][NumArray[k]];
		}
	}
	/*for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			//ccc[j][k] = mess1[i];
			cout << ccc2[j][k];
			//i++;
		}
	}*/
	i = 0;
	for (int k = 0; k < TheRealLen; k++) {
		for (int j = 0; j < cishu; j++) {
			c2[i++] = ccc2[j][k];
		}
	}

	//cout << c2;

//	for (i = 0; i < TheRealLen; i++) {
//		free(ccc[i]);
//		free(ccc1[i]);
//		free (ccc2[i]);
//	}
//	delete ccc[i];
//	delete ccc1[i];
//	delete ccc2[i];

};
int main() {
	//tidai
	int Key = 8;
	int n = 26;
	string mess = "It is KaiSa";
	cout<<"替代密码，密钥为："<<Key << endl;
	//int len = mess.length();
	int len = 0;
	int sl = 0;
	while (mess[sl] != '\0') {
		if (mess[sl] == ' ') {
			sl++;
			continue;
		}
		len++;
		sl++;
	}
	//cout << len;
	char* c = new char[len];//声明堆，len个单位
	char* mes = new char[len];
	for (int j = 0; j < len; j++)
		c[j] = ' ';
	for (int j = 0; j < len; j++)
		mes[j] = ' ';
	//加密
	tidai(mess, Key, n, c);
	len = mess.length();
	cout << "加密："  << endl;
	for (int j = 0; j < len; j++) {
		cout << c[j];
	}
	cout << endl;
	//解密
	tidai(c, 26 - Key, n, mes);
	cout << "解密：" <<endl;
	for (int j = 0; j < len; j++) {
		cout << mes[j];
	}
	cout << endl;
	
	

	//zhihua
	//加密 
	string key1 = "cipher";
	cout << "置换密码，密钥为：" << key1 << endl;
	vector<int> NumArray;
	KKey(key1, NumArray);
	string mess1 = "attack begins at five";
	//将空格去掉
	int len1 = 0;
	int slll = 0;
	while (mess1[slll] != '\0') {
		if (mess1[slll] == ' ') {
			slll++;
			continue;
		}
		len1++;
		slll++;
	}
	char* mm = new char[len1];
	slll = 0; len1 = 0;
	while (mess1[slll] != '\0') {
		if (mess1[slll] == ' ') {
			slll++;
			continue;
		}
		mm[len1] = mess1[slll];
		len1++;
		slll++;
	}

	//cout << len1 << endl;//mm的长度
	char* c1 = new char[len1];
	char* c2 = new char[len1];

	for (int j = 0; j < len1; j++) {
		c1[j] = ' ';
		c2[j] = ' ';
	}
	zhihuan(mm, len1, c1, NumArray);
	cout << "加密："<<c1 << endl;


	//解密
	char* m1 = new char[len1];
	char* m2 = new char[len1];
	//cout << len1 << endl;
	for (int j = 0; j < len1; j++) {
		m1[j] = ' ';
	}
	for (int j = 0; j < len1; j++) {
		m2[j] = ' ';
	}
	key1 = "rehpic";
	int TheRealLen = NumArray.size();
	int cishu = len1 / TheRealLen;
	int i = 0;
	for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			c2[i++]=c1[j + k * cishu];
		}
	}
	//for (int j = 0; j < len1; j++) {
	//	cout << c2[j] ;
	//}
	//cout << endl;
	zhihuan(c2, len1, m1, NumArray);
	//cout << m1<<endl;

	i = 0;
	for (int j = 0; j < cishu; j++) {
		for (int k = 0; k < TheRealLen; k++) {
			m2[i++] = m1[j + k * cishu];
		}
	}
	cout << "解密：";
	cout << m2 << endl;
	delete c;
	delete mes;
	delete c1; delete c2;
	delete m1; delete m2;
}
