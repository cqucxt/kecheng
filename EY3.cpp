#include <windows.h>
#include <winioctl.h> //DDK驱动开发与控制
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
using namespace std;


int main() {
	//	LPCWSTR path = "P:\\exp1.exe";

	HANDLE hFile;
	HANDLE hMapping;
	LPVOID pMapping;
	hFile = CreateFile("P:\\exp1.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
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

	IMAGE_OPTIONAL_HEADER optional_header;
	optional_header = nt_header->OptionalHeader;

	cout << "AddressOfEntryPoint:\t" << optional_header.AddressOfEntryPoint << endl;
	cout << "ImageBase:\t" << optional_header.ImageBase << endl;
	cout << "SectionAlignment:\t" << optional_header.SectionAlignment << endl;
	cout << "FileAlignment:\t" << optional_header.FileAlignment << endl;
	cout << "NumberOfSections:\t" << nt_header->FileHeader.NumberOfSections << endl;

	//AddressOfEntryPoint、ImageBase、
	//SectionAlignment、FileAlignment、NumberOfSections


	DWORD PointerToRawData = IMAGE_FIRST_SECTION(nt_header)->PointerToRawData;
	DWORD SizeOfRawData = IMAGE_FIRST_SECTION(nt_header)->SizeOfRawData;
	UINT8* textContent = new UINT8[SizeOfRawData];
	memcpy(textContent, (UINT8*)pMapping + PointerToRawData, SizeOfRawData);
	int flag = 0;
	for (size_t i = 0; i < 1024; ++i)
	{
		unsigned char c = textContent[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if ((flag) % 16 == 0)
			printf("\n");
	}



	CloseHandle(hFile);

	HANDLE hdrive;
	hdrive = CreateFile(
		TEXT("\\\\.\\PhysicalDrive1"), // 物理磁盘
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 允许其他应用程序读写磁盘
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (CreateFile == INVALID_HANDLE_VALUE) {
		// 处理打开磁盘失败的情况
		cout << "test" << endl;
		return -1;
	}
	OVERLAPPED overLap = { 0 };
	overLap.Offset = 512;
	DWORD writeByte;
	DWORD readsize = 0;
	LARGE_INTEGER offset;//long long signed
	offset.QuadPart = (ULONGLONG)59336704 * (ULONGLONG)512;//0
	SetFilePointer(hdrive, offset.LowPart, &offset.HighPart, FILE_BEGIN);
	if (!WriteFile(hdrive, textContent, SizeOfRawData, &writeByte, &overLap)) {
		cout << dec << "error code" << GetLastError() << endl;
	}

	//	CloseHandle(hdrive);

}

