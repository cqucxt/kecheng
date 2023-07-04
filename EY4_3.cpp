#include <windows.h>
#include <winioctl.h> //DDK驱动开发与控制
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
#include <string>
#include <sstream>
using namespace std;

//节表免疫，方法一 

string decimal_to_hex_str(int decimal) {
    stringstream ss;
    ss << hex << decimal;
    return ss.str();
}

int main() {
	//	LPCWSTR path = "P:\\exp1.exe";

	HANDLE hFile;
	HANDLE hMapping;
	LPVOID pMapping;
	hFile = CreateFile("P:\\exp1.exe", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return 0;
	}
	//将PE文件映射到内存
	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
	if (!hMapping) {
		return 0;
	}
	pMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);//返回的是map的开始地址
	if (!pMapping) {
		return 0;
	}

	PIMAGE_DOS_HEADER dosheader;
	dosheader = (PIMAGE_DOS_HEADER)pMapping;
	if (dosheader->e_magic != IMAGE_DOS_SIGNATURE) {
		cout << "无效的PE文件" << endl;
		return 0;
	}

	PIMAGE_NT_HEADERS nt_header;
	nt_header = (PIMAGE_NT_HEADERS)((BYTE*)pMapping + dosheader->e_lfanew);
	if (nt_header->Signature != IMAGE_NT_SIGNATURE) {
		cout << "无效的PE文件" << endl;
		return 0;
	}

	PIMAGE_SECTION_HEADER section_header;
	section_header = IMAGE_FIRST_SECTION(nt_header);
	for (int i = 0; i < nt_header->FileHeader.NumberOfSections;
		i++, section_header++) {
		cout << section_header->Name << "\t" << section_header->Misc.VirtualSize
			<< "\t\t" << section_header->VirtualAddress << "\t\t" <<
			section_header->SizeOfRawData << "\t\t"
			<< section_header->PointerToRawData << "\t\t\t" << section_header->Characteristics << endl;
		
	}
	
	
	for(int i=0;i<nt_header->FileHeader.NumberOfSections;i++){
		DWORD el_anew =424+40*i; 
//		DWORD el_anew = 40+24 + nt_header->FileHeader.SizeOfOptionalHeader+40*i;
		DWORD byteofthis=4;
		UINT8* Content = new UINT8[byteofthis];
		memcpy(Content, (UINT8*)pMapping + el_anew+16, byteofthis);
		int flag = 0;
		for (size_t i = 0; i < byteofthis; ++i)
		{
			unsigned char c = Content[i]; // must use unsigned char to print >128 value 
			flag++;
			if (c < 16)
				printf("0%x ", c);
			else
				printf("%x ", c);
			if ((flag) % 16 == 0)
				printf("\n");
		}
		cout<<endl;
		
		OVERLAPPED overLap = { 0 };
		overLap.Offset = 424+40*i+8;
		DWORD writeByte;
		DWORD readsize = 0;
		LARGE_INTEGER offset;//long long signed
		offset.QuadPart = (ULONGLONG)0 * (ULONGLONG)512;//0
		SetFilePointer(hFile, offset.LowPart, &offset.HighPart, FILE_BEGIN);
		if (!WriteFile(hFile, Content, byteofthis, &writeByte, &overLap)) {
			cout << dec << "error code" << GetLastError() << endl;
		}	
		
		
	}
	
//	DWORD el_anew = dosheader->e_lfanew+24 + nt_header->FileHeader.SizeOfOptionalHeader;
//	DWORD byteofthis = 40 * nt_header->FileHeader.NumberOfSections;
////	cout<<"SizeOfOptionalHeader:"<<nt_header->FileHeader.SizeOfOptionalHeader<<endl;
////	cout<<"sizeof(section_header):"<<sizeof(section_header)<<endl;
////	cout<<"nt_header->FileHeader.NumberOfSections:"<<nt_header->FileHeader.NumberOfSections<<endl;
//	cout<<"byteofthis:"<<byteofthis<<endl;
//	UINT8* Content = new UINT8[byteofthis];
//	memcpy(Content, (UINT8*)pMapping + el_anew, byteofthis);
//	int flag = 0;
//	for (size_t i = 0; i < byteofthis; ++i)
//	{
//		unsigned char c = Content[i]; // must use unsigned char to print >128 value 
//		flag++;
//		if (c < 16)
//			printf("0%x ", c);
//		else
//			printf("%x ", c);
//		if ((flag) % 16 == 0)
//			printf("\n");
//	}


	//el_anew的位置0x3C,大小为uint32_t

//NT_header+optionalHeader+sectionHeader的位置
//	uint32_t el_anew = dosheader->e_lfanew;
//	cout << "el_fanew:" << el_anew << endl;
	//大小=18H+fileHeader.sizeofoptionalheader+sizeof(section_header)* nt_header->FileHeader.NumberOfSections
//	uint32_t byteofthis=24+nt_header->fileHeader.sizeofoptionalheader+sizeof(section_header)* nt_header->FileHeader.NumberOfSections;
	//转移的位置：1023，大小相同，

//59336704
//	OVERLAPPED overLap = { 0 };
//	overLap.Offset = 1024-byteofthis;
//	DWORD writeByte;
//	DWORD readsize = 0;
//	LARGE_INTEGER offset;//long long signed
//	offset.QuadPart = (ULONGLONG)0 * (ULONGLONG)512;//0
//	SetFilePointer(hFile, offset.LowPart, &offset.HighPart, FILE_BEGIN);
//	if (!WriteFile(hFile, Content, byteofthis, &writeByte, &overLap)) {
//		cout << dec << "error code" << GetLastError() << endl;
//	}	
//
//	DWORD sizeofoptionalheader=1024-el_anew-byteofthis;
//	sizeofoptionalheader=sizeofoptionalheader+nt_header->FileHeader.SizeOfOptionalHeader;
//	cout<<sizeofoptionalheader<<endl;//2c0
//	
//	
//
////修改sizeofoptionalheader
//
//	UINT8 data[] = { 0xc0, 0x02};
//
//	DWORD el_anewweizhi = 0xb0+0x14;
//	DWORD byteofthis1 = 2;
//
//	OVERLAPPED overLap1 = { 0 };
//	overLap1.Offset = 0xb0+0x14;
////	DWORD writeByte;
//	readsize = 0;
////	LARGE_INTEGER offset;//long long signed
//	offset.QuadPart = (ULONGLONG)0 * (ULONGLONG)512;//0
//	SetFilePointer(hFile, offset.LowPart, &offset.HighPart, FILE_BEGIN);
//	if (!WriteFile(hFile, data, byteofthis1, &writeByte, &overLap1)) {
//		cout << dec << "error code" << GetLastError() << endl;
//	}	 


	CloseHandle(hFile);



}

