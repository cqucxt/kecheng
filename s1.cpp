/*#include <iostream>
#include<string>
using namespace std;

int PC1[56] = {57,49,41,33,25,17,9,
                1,58,50,42,34,26,18,
                10,2,59,51,43,35,27,
                19,11,3,60,52,44,36,
                63,55,47,39,31,23,15,
                7,62,54,46,38,30,22,
                14,6,61,53,45,37,29,
                21,13,5,28,20,12,4};
int PC2[48] = { 14,17,11,24,1,5,
3,28,15,6,21,10,
23,19,12,4,26,8,
16,7,27,20,13,2,
41,52,31,37,47,55,
30,40,51,45,33,48,
44,49,39,56,34,53,
46,42,50,36,29,32 };
int lshift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

//十六进制转二进制
string trans16to2(string hexDigit) {
	string binaryDigit;
    for (int i = 0; i < hexDigit.length(); ++i) {
        char e = hexDigit[i];
        if (e >= 'A' && e <= 'F') {
            int a = static_cast<int>(e - 'A' + 10);
            switch (a) {
            case 10:
                binaryDigit += "1010";
                break;
            case 11:
                binaryDigit += "1011";
                break;
            case 12:
                binaryDigit += "1100";
                break;
            case 13:
                binaryDigit += "1101";
                break;
            case 14:
                binaryDigit += "1110";
                break;
            case 15:
                binaryDigit += "1111";
                break;
            }
        }
        else if (isdigit(e)) {
            int b = static_cast<int>(e - '0');
            switch (b) {
            case 0:
                binaryDigit += "0000";
                break;
            case 1:
                binaryDigit += "0001";
                break;
            case 2:
                binaryDigit += "0010";
                break;
            case 3:
                binaryDigit += "0011";
                break;
            case 4:
                binaryDigit += "0100";
                break;
            case 5:
                binaryDigit += "0101";
                break;
            case 6:
                binaryDigit += "0110";
                break;
            case 7:
                binaryDigit += "0111";
                break;
            case 8:
                binaryDigit += "1000";
                break;
            case 9:
                binaryDigit += "1001";
                break;
            }
        }
    }
    return binaryDigit;
};

//循环左移一位
string strol(string ss) {
    int len = ss.length();
    string ss1;
    char temp = ss[0];
    for (int i = 1; i < len; i++) {
        ss1 += ss[i];
    }
    ss1 += temp;
    return ss1;
};

int main() {
	string hexDigit;
	cin >> hexDigit;
	string binaryDigit = trans16to2(hexDigit);
    cout << binaryDigit;
    cout << binaryDigit.length();
    string sPC1;
    string K[16];
    for (int i = 0; i < 56; i++) {
        sPC1 += binaryDigit[PC1[i]-1];
        //cout << sPC1 << endl;
    }
    cout << sPC1 << endl;
    string temp = sPC1;
    string c = temp.substr(0, 28);
    string d = temp.substr(28, 55);
   for (int i = 0; i < 16; i++) {
        int numlshift = lshift[i];
        cout << c<<endl;
        cout << d << endl;
        if (numlshift == 1) {
            c = strol(c);
            d = strol(d);
        }
        if (numlshift == 2) {
            c = strol(c);
            d = strol(d);
            c = strol(c);
            d = strol(d);
        }
        
        string ss;
        ss += c;
        ss += d;
        cout << ss.length() << endl;
        cout << ss<<endl;
        for (int j = 0; j < 48; j++) {
            K[i] += ss[PC2[j]-1];
        }
    }
   for (int k = 0; k < 16; k++) {
       //cout << K[k].length() << endl;
       cout << K[k] << endl;
   }
}*/