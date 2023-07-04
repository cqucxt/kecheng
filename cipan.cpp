#include <windows.h>
#include <winioctl.h> //DDK驱动开发与控制
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
#define BufferLength 1024
using namespace std;

//将四个连续字节存放的值转为int型
uint32_t transtoint(unsigned char a[])
{
	uint32_t sum = 0UL;
	uint32_t temp = 0UL;
	
	for (int i = 3; i>=0; i--)
	{
		temp = a[i];//先赋值				
		temp = temp <<( 8 * (3-i));		
		sum = sum | temp;
				
	}
	return sum;
}
//十六进制输出
bool HexOutput(char* buf, size_t len,bool ismbr, ULONGLONG * baseaddr,ULONGLONG * nextaddr,int EBRnum)
{
	bool mbrflag=1;//在读取MBR的时候判断条目是主分区还是扩展分区条目 
	unsigned char a = buf[0];
	if(ismbr)
		printf("-----------------------------------------------------");
	else
		printf("--------------------------第%d个EBR------------------",EBRnum);
	//printf("第一字节是：%x\n\n", a);
	if(ismbr)
		printf("          第一部分(MBR)：\n\n");
	else
		printf("          第一部分(EBR)：\n\n");
	int flag = 0;
	for (size_t i = 0; i < len; ++i)
	{
		unsigned char c = buf[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if (i == 445)
		{
			flag = 0;
			printf("\n\n          第二部分(分区表)：\n");
		}
		if (i == 509)
		{
			flag = 0;
			printf("\n\n          第三部分(结束标志)：\n");
		}
		if ((flag) % 16 == 0)
			printf("\n");
	}
	printf("\n<-------------------分区表信息解析------------------->\n\n");
	printf("\n\n分区地址和大小分别为: \n\n");	
	int limit=ismbr?509:477;//如果已经知道是扩展分区的EBR，不用读取那么多,rank=1是分区表，rank=2是指向下一个的
	for (int m = 445, rank = 1; m < limit&&rank<=4; m += 16, rank++)
	{
		unsigned char fifth = buf[m + 5];//取得第五位文件系统标志位，十六进制为05H或者0FH是扩展分区 		
		if(fifth==0x5)//是扩展分区条目，不用往后读了 
		{
			printf("This is an extend patition!\n");
			mbrflag=0;	
			rank = 4;
		}//不是mbr是扩展分区EBR 
		if (fifth < 16) //调整输出格式
			printf("第%d分区表标志位为: 0%x\n", rank, fifth);
		else
			printf("第%d分区表标志位为: %x\n", rank, fifth);
		if (fifth == 0x00)//当第五位（标志位）是00时，代表分区表信息为空，无分区
		{
			printf(" 分区表为空\n\n");//也不用往后读了 
		}
		else {//分区项 
			if(fifth==0xf||fifth==0x5)//是扩展分区条目，不用往后读了 
			{
				printf("This is an extend patition!\n");
				mbrflag=0;	
//				rank = 4;
			}else{
				mbrflag=1; 
			} 
			unsigned char offsetadd[4] = { 0 };
			//倒着读，高字节在高地址处
			for (int n = m + 12, t = 0; n > m + 8, t < 4; n--, t++)
			{
				unsigned char temp = buf[n];
				if (temp < 16)
					printf("  0%x  ", temp);
				else
					printf("  %x  ", temp);
				offsetadd[t] = buf[n];
			}
			//计算地址，转换为十进制扇区数LBA
			printf("\n");
			uint32_t tempadd = transtoint(offsetadd);
			printf("\n开始地址为： %I64x", (ULONGLONG)tempadd * (ULONGLONG)512 + *baseaddr);
			
			if(ismbr&&!mbrflag)// if in mbr and got a extend entry,the EBR at relsecor+nowbase(0)
			{
				*baseaddr=(ULONGLONG)tempadd* (ULONGLONG)512 + *baseaddr;//only change once
				*nextaddr = (ULONGLONG)0UL;
			}				
			else if (!mbrflag)//if it's the extend entry
				{
					*nextaddr = (ULONGLONG)tempadd * (ULONGLONG)512;
				}
			printf("\n\n");
			printf("大小：");
			for (int p = m + 16, w = 0; p > m + 12, w < 4; p--, w++)
			{
				unsigned char temp1 = buf[p];
				if (temp1 < 16)
					printf("  0%x  ", temp1);
				else
					printf("  %x  ", temp1);
				offsetadd[w] = buf[p];
			}
			//计算大小，转化为GB单位
			printf("\n");
			uint32_t tempsize = transtoint(offsetadd);
			
			if(ismbr && !mbrflag)
				printf("\n扩展盘总大小为： %lu 扇区 = %lf GB \n", tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
			else if(!ismbr)
			{
				printf("\n第%d逻辑盘大小为： %lu 扇区 = %lf GB \n",EBRnum, tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
			}
			else
				printf("\n该盘大小为： %lu 扇区 = %lf GB \n", tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
		}
	}
	printf("\n\n");
	if(ismbr){
		return 0;
	}else{
		return mbrflag;
	}
}

//函数：对主分区表进行解析，分别得到每个分区的偏移地址以及分区大小
BOOL GetDriveGeometry(DISK_GEOMETRY *pdg, int addr)
{
	HANDLE hDevice;               // 设备句柄
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard resultscc
	char lpBuffer[BufferLength] = { 0 };


	//通过CreateFile来获得设备的句柄
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"), // 设备名称,这里指第一块硬盘
		GENERIC_READ,                // no access to the drive
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		printf("Creatfile error!May be no permission!ERROR_ACCESS_DENIED！\n");
		return (FALSE);
	}

	//通过DeviceIoControl函数与设备进行IO
	bResult = DeviceIoControl(hDevice, // 设备的句柄
		IOCTL_DISK_GET_DRIVE_GEOMETRY, // 控制码，指明设备的类型
		NULL, 
		0, // no input buffer
		pdg,
		sizeof(*pdg),     
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL); // synchronous I/O

	LARGE_INTEGER offset;//long long signed
	offset.QuadPart = (ULONGLONG)addr * (ULONGLONG)512;//0
	SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN);//从0开始读MBR
	if(GetLastError())
		printf("错误类型代号：%ld\n\n", GetLastError());//如果出错了
		
	DWORD dwCB;
	//从这个位置开始读 
	BOOL bRet = ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);
	//如果不是MBR，
	bool finished=0; 
	int EBRnum=0;
	ULONGLONG *baseaddr=new ULONGLONG,*nextaddr= new ULONGLONG;//扩展分区起始地址，EBR地址 
	*baseaddr = (ULONGLONG)0;
	*nextaddr = (ULONGLONG)0;
	finished=HexOutput(lpBuffer, 512,true,baseaddr,nextaddr,EBRnum);//先是读取MBR
	
	if(finished)
		CloseHandle(hDevice);
	else
	{
		//继续读
		do{
			EBRnum++;
			memset(lpBuffer, 0, sizeof(lpBuffer));
			offset.QuadPart = (ULONGLONG)(*baseaddr + *nextaddr);//find the EBR
			cout<<"test test tes :"<<offset.QuadPart<<"  "<<offset.LowPart<<endl;
//			cout<<"\ntest "<<*baseaddr<<" "<<*nextaddr<<" "<<*baseaddt+*nextaddr<<endl;
			SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN);//读EBR
			if(GetLastError())
				printf("错误类型代号：%ld\n\n", GetLastError());//如果出错了
			
			ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);
			
		}while(!HexOutput(lpBuffer, 512,false,baseaddr,nextaddr,EBRnum));
		CloseHandle(hDevice);
	}

	delete baseaddr;
	delete nextaddr;
	return bResult;
}
extern int add[20];
extern int disknum;
int main()
{
	DISK_GEOMETRY pdg;            // 保存磁盘参数的结构体
	BOOL bResult;                 // generic results flag
	ULONGLONG DiskSize;           // size of the drive, in bytes
	printf("<-----------------欢迎使用分区读取程序----------------->\n\n");
	bResult = GetDriveGeometry(&pdg, 0);

	if (bResult)
	{
		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
			(ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;

		printf("磁盘总大小 = %I64d (Bytes) = %I64d (Gb)\n", DiskSize, DiskSize / (1024 * 1024 * 1024));
	}
	else
	{
		printf("GetDriveGeometry failed. Error %ld.\n", GetLastError());
	}
	return ((int)bResult);
}
