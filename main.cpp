#include <windows.h>
#include <winioctl.h> //DDK驱动开发与控制
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
		temp = a[i];//先赋值				
		temp = temp << (8 * (3 - i));
		sum = sum | temp;

	}
	return sum;
}

struct MBR_disk_entry
{
	uint8_t bootflag;//引导标志
	uint8_t citouhao;//磁头号
	uint8_t shanquhao;//扇区号
	uint8_t zhumianhao;//柱面号
	uint8_t disk_flag;//分区类型标志
	uint8_t someinfo[3];//id、结束磁头号、结束柱面号、结束扇区号
	uint8_t relative[4];//相对起始扇区
	uint8_t sectors[4];//总扇区数
};
struct MBR
{
	uint8_t boot_code[446];//引导代码
	//4个分区表，每个16字节,只有一个分区表有内容，对应的标志是0xEE
	MBR_disk_entry pation_table_entry[4];
	uint8_t endflag[2];//55AA
};

bool HexOutput(MBR the_mbr, size_t len, bool ismbr, ULONGLONG* baseaddr, ULONGLONG* nextaddr, int EBRnum)
{
	BOOL mbrflag = 1;
	if (ismbr) {
		printf("=====================================================");
		printf("        第一部分（MBR）:\n\n");
	}
	else {
		printf("===================第%d个EBR========================", EBRnum);
		printf("       EBR:\n\n");
	}
	printf("\n<===================分区表信息解析======================>\n\n");
	printf("\n\n分区起始扇区和大小分别为: \n\n");
	int limit = ismbr ? 4 : 2;//如果已经知道是扩展分区的EBR，不用读取那么多
	for (int i = 0, rank = 1; i < limit; i++, rank++) {
		uint8_t sysFlag = the_mbr.pation_table_entry[i].disk_flag;
		printf("第%d分区表标志位为: 0%x\n", rank, sysFlag);
		if (sysFlag == 0x00) {
			printf("分区表为空\n\n");
		}
		//以上遇到这两种情况就不用再继续读下去了
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
					cout << "起始扇区数：" << tempadd << endl;
				}else
					cout << "起始扇区数：" << tempadd + *nextaddr << endl;
			else
				if(ismbr){
					cout << "起始扇区数：" << tempadd << endl;
				}else
					cout << "起始扇区数：" << tempadd + *baseaddr <<endl;
			
			if (!mbrflag) {
				*nextaddr = tempadd + *baseaddr;//扩展分区的地址，会改变
				cout<<"test next "<<*nextaddr<<endl;
			}
			if (ismbr && !mbrflag) {
				*baseaddr = tempadd;//主分区里的扩展分区的地址，只有这个值和0值
				cout<<"test base"<<*baseaddr<<endl;
			}
			printf("大小：");
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

	printf("<==================欢迎使用分区读取程序==============================>\n\n");
	/*
	HANDLE CreateFile(
		LPCTSTR lpFileName, // file name对象路径名
		DWORD dwDesiredAccess, // access mode控制模式
		DWORD dwShareMode, // share mode共享模式
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD 安全属性(也即销毁方式)
		DWORD dwCreationDisposition, // how to create，通常设置为OPEN_EXISTING
		DWORD dwFlagsAndAttributes, // file attributes
		HANDLE hTemplateFile // handle to template file
	);*/
	//通过CreateFile来获得设备的句柄
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		printf("Creatfile error!May be no permission!ERROR_ACCESS_DENIED！\n");
		return (FALSE);
	}
	/*
BOOL DeviceIoControl(
	HANDLE hDevice, // 用CreateFile获得的设备句柄
	DWORD dwIoControlCode, // 指定操作码
	LPVOID lpInBuffer, // 指向存放操作所需数据的缓冲区的指针
	DWORD nInBufferSize, // 以字节为单位，指定输入缓冲区的大小
	LPVOID lpOutBuffer, // 指向用来存放输出数据的缓冲区的指针
	DWORD nOutBufferSize, // 以字节为单位，指定输出缓冲区的大小
	LPDWORD lpBytesReturned, // 指向一个存放输出数据长度的变量的指针
	LPOVERLAPPED lpOverlapped // 指向OVERLAPPED结构体的指针
);
*/
//通过DeviceIoControl函数与设备进行IO
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
		HANDLE hFile, //文件句柄
		LONG lDistanceToMove, //指定移动文件指针的字节数
		PLONG lpDistanceToMoveHigh, //指向要移动的有符号64位距离的高32位
		DWORD dwMoveMethod //文件指针移动的起点
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
//		printf("错误类型代号：%ld\n\n", GetLastError());//如果出错了

	/*
	BOOL ReadFile(
		HANDLE hFile, // 文件句柄
		LPVOID lpBuffer, // 用来接收从文件中读出的数据的缓冲区指针
		DWORD nNumberOfBytesToRead, // 要读的字节总数
		LPDWORD lpNumberOfBytesRead, // 用来存储实际传输的字节总数的指针
		LPOVERLAPPED lpOverlapped // 一个指向OVERLAPPED结构体的指针
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
	//扩展分区的起始地址和EBR地址
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
//				printf("错误类型代号：%ld\n\n", GetLastError());//如果出错了
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

