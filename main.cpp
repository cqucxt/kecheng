#include <windows.h>
#include <winioctl.h> //DDK�������������
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
using namespace std;

typedef unsigned char uint8_t;

uint32_t transtoint(unsigned char a[])
{
	uint32_t sum = 0UL;
	uint32_t temp = 0UL;

	for (int i = 3; i >= 0; i--)
	{
		temp = a[i];//�ȸ�ֵ				
		temp = temp << (8 * (3 - i));
		sum = sum | temp;

	}
	return sum;
}

struct MBR_disk_entry
{
	uint8_t bootflag;//������־
	uint8_t citouhao;//��ͷ��
	uint8_t shanquhao;//������
	uint8_t zhumianhao;//�����
	uint8_t disk_flag;//�������ͱ�־
	uint8_t someinfo[3];//id��������ͷ�š���������š�����������
	uint8_t relative[4];//�����ʼ����
	uint8_t sectors[4];//��������
};
struct MBR
{
	uint8_t boot_code[446];//��������
	//4��������ÿ��16�ֽ�,ֻ��һ�������������ݣ���Ӧ�ı�־��0xEE
	MBR_disk_entry pation_table_entry[4];
	uint8_t endflag[2];//55AA
};

bool HexOutput(MBR the_mbr, size_t len, bool ismbr, ULONGLONG* baseaddr, ULONGLONG* nextaddr, int EBRnum)
{
	BOOL mbrflag = 1;
	if (ismbr) {
		printf("=====================================================");
		printf("        ��һ���֣�MBR��:\n\n");
	}
	else {
		printf("===================��%d��EBR========================", EBRnum);
		printf("       EBR:\n\n");
	}
	printf("\n<===================��������Ϣ����======================>\n\n");
	printf("\n\n������ʼ�����ʹ�С�ֱ�Ϊ: \n\n");
	int limit = ismbr ? 4 : 2;//����Ѿ�֪������չ������EBR�����ö�ȡ��ô��
	for (int i = 0, rank = 1; i < limit; i++, rank++) {
		uint8_t sysFlag = the_mbr.pation_table_entry[i].disk_flag;
		printf("��%d�������־λΪ: 0%x\n", rank, sysFlag);
		if (sysFlag == 0x00) {
			printf("������Ϊ��\n\n");
		}
		//������������������Ͳ����ټ�������ȥ��
		else {
			uint8_t offsetadd[4] = { 0 };
			if (sysFlag == 0xF || sysFlag == 0x5) {
				printf("This is an extend patition!\n");
				mbrflag = 0;
			}else{
				mbrflag = 1;
			}
			for (int j = 0, k = 3; j < 4; j++, k--) {
				offsetadd[j] = the_mbr.pation_table_entry[i].relative[k];
			}
			uint32_t tempadd = transtoint(offsetadd);

			if (sysFlag == 0x7)
				if(ismbr){
					cout << "��ʼ��������" << tempadd << endl;
				}else
					cout << "��ʼ��������" << tempadd + *nextaddr << endl;
			else
				if(ismbr){
					cout << "��ʼ��������" << tempadd << endl;
				}else
					cout << "��ʼ��������" << tempadd + *baseaddr <<endl;
			
			if (!mbrflag) {
				*nextaddr = tempadd + *baseaddr;//��չ�����ĵ�ַ����ı�
				cout<<"test next "<<*nextaddr<<endl;
			}
			if (ismbr && !mbrflag) {
				*baseaddr = tempadd;//�����������չ�����ĵ�ַ��ֻ�����ֵ��0ֵ
				cout<<"test base"<<*baseaddr<<endl;
			}
			printf("��С��");
			for (int j = 3; j >= 0; j--) {
				if (the_mbr.pation_table_entry[i].sectors[j] < 16)
					printf(" 0%x ", the_mbr.pation_table_entry[i].sectors[j]);
				else
					printf(" %x ", the_mbr.pation_table_entry[i].sectors[j]);
			}
			printf("\n\n");
		}
	}
	printf("\n\n");
	if(ismbr){
		return 0;
	}else{
		return mbrflag;
	}
}

int main() {
	HANDLE hDevice;
	DISK_GEOMETRY* pdg;
	BOOL bResult, bRet;
	DWORD junk, dwCB;
	MBR the_mbr = { 0 };

	printf("<==================��ӭʹ�÷�����ȡ����==============================>\n\n");
	/*
	HANDLE CreateFile(
		LPCTSTR lpFileName, // file name����·����
		DWORD dwDesiredAccess, // access mode����ģʽ
		DWORD dwShareMode, // share mode����ģʽ
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD ��ȫ����(Ҳ�����ٷ�ʽ)
		DWORD dwCreationDisposition, // how to create��ͨ������ΪOPEN_EXISTING
		DWORD dwFlagsAndAttributes, // file attributes
		HANDLE hTemplateFile // handle to template file
	);*/
	//ͨ��CreateFile������豸�ľ��
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		printf("Creatfile error!May be no permission!ERROR_ACCESS_DENIED��\n");
		return (FALSE);
	}
	/*
BOOL DeviceIoControl(
	HANDLE hDevice, // ��CreateFile��õ��豸���
	DWORD dwIoControlCode, // ָ��������
	LPVOID lpInBuffer, // ָ���Ų����������ݵĻ�������ָ��
	DWORD nInBufferSize, // ���ֽ�Ϊ��λ��ָ�����뻺�����Ĵ�С
	LPVOID lpOutBuffer, // ָ���������������ݵĻ�������ָ��
	DWORD nOutBufferSize, // ���ֽ�Ϊ��λ��ָ������������Ĵ�С
	LPDWORD lpBytesReturned, // ָ��һ�����������ݳ��ȵı�����ָ��
	LPOVERLAPPED lpOverlapped // ָ��OVERLAPPED�ṹ���ָ��
);
*/
//ͨ��DeviceIoControl�������豸����IO
	bResult = DeviceIoControl(
		hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		pdg,
		sizeof(*pdg),
		&junk,
		(LPOVERLAPPED)NULL
	);

	/*
	DWORD SetFilePointer(
		HANDLE hFile, //�ļ����
		LONG lDistanceToMove, //ָ���ƶ��ļ�ָ����ֽ���
		PLONG lpDistanceToMoveHigh, //ָ��Ҫ�ƶ����з���64λ����ĸ�32λ
		DWORD dwMoveMethod //�ļ�ָ���ƶ������
	);
*/
	HANDLE hdevice;
	LARGE_INTEGER offset;
	int addr=0;
	offset.QuadPart = (ULONGLONG)addr * (ULONGLONG)512;
	SetFilePointer(
		hDevice,
		offset.LowPart,
		&offset.HighPart,
		FILE_BEGIN
	);
	if (GetLastError())
		printf("/n");
//		printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������

	/*
	BOOL ReadFile(
		HANDLE hFile, // �ļ����
		LPVOID lpBuffer, // �������մ��ļ��ж��������ݵĻ�����ָ��
		DWORD nNumberOfBytesToRead, // Ҫ�����ֽ�����
		LPDWORD lpNumberOfBytesRead, // �����洢ʵ�ʴ�����ֽ�������ָ��
		LPOVERLAPPED lpOverlapped // һ��ָ��OVERLAPPED�ṹ���ָ��
	);
	*/
	bRet = ReadFile(
		hDevice,
		&the_mbr,
		512,
		&dwCB,
		NULL
	);

	BOOL finished = 0;
	int EBRnum = 0;
	//��չ��������ʼ��ַ��EBR��ַ
	ULONGLONG * baseaddr = new ULONGLONG[4], * nextaddr = new ULONGLONG[4];
	*baseaddr = (ULONGLONG)0;
	*nextaddr = (ULONGLONG)0;
	finished = HexOutput(the_mbr, 512, true, baseaddr, nextaddr, EBRnum);

	if (finished)
		CloseHandle(hDevice);
	else {
		do {
			EBRnum++;
			the_mbr = { 0 };
			offset.QuadPart = (ULONGLONG)*nextaddr * (ULONGLONG)512;//xuyao gai 
			cout<<"\ntest "<<*nextaddr * 512<<"  \n";
			SetFilePointer(hDevice,
				offset.LowPart,
				&offset.HighPart,
				FILE_BEGIN);
			if (GetLastError())
				printf("/n");
//				printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������
			ReadFile(hDevice, &the_mbr, 512, &dwCB, NULL);
		} while (!HexOutput(the_mbr, 512, false, baseaddr, nextaddr, EBRnum));
		CloseHandle(hDevice);
	}
	delete baseaddr;
	delete nextaddr;

	if (bResult) {
		printf("<=======================success=================================>\n\n");
	}
	else {
		printf("/n");
//		printf("GetDriveGeometry failed. Error %ld.\n", GetLastError());
	}
	return ((int)bResult);
}

