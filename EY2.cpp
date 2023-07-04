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

struct BPB {
	uint8_t bytes_per_sector[2];//扇区字节数
	uint8_t secotrs_per_cluster;//每簇扇区数
	uint8_t reserve_sectors[2];//包括DBR自己在内的FAT之前的扇区个数,即保留扇区数
	uint8_t FATnum;//FAT个数，一般为2
	uint8_t unimportant1[11];
	uint8_t DBR_LBA[4];//该分区的DBR所在的相对扇区号，如果是扩展分区，是相对于扩展分区首的
	uint8_t totalsectors[4];//本分区的总扇区数
	uint8_t sectors_per_FAT[4];//每个FAT的扇区数
	uint8_t unimportant2[4];
	uint8_t root_cluster_number[4];//根目录簇号
	uint8_t file_info[2];
	uint8_t backup_DBR[2];//备份引导扇区的相对于DBR的扇区号，一般为6，内容和DBR一模一样
	uint8_t zero1[12];
	uint8_t extBPB[26];//扩展BPB
};
struct DBR {
	uint8_t jumpcode[3];//EB 58 90
	uint8_t OEM[8];//OEM代号
	BPB bpb;
	uint8_t osboot[422];//引导代码和55AA
};
struct shortfile {
	uint8_t FileName[8];//文件名
	uint8_t ExtendName[3];//扩展名
	uint8_t attributeOfFile;//属性字节
	uint8_t SystemReserve;//系统保留
	uint8_t CreateTime_ms;//创建时间的10毫秒位
	uint8_t CreateTime[2];//创建时间
	uint8_t CreateDate[2];//创建日期
	uint8_t LastAccess[2];//最后访问日期
	uint8_t HighCluster[2];//文件起始簇号高16位
	uint8_t LastModifyTime[2];//最近修改时间
	uint8_t LastModifyDate[2];//最近修改时间
	uint8_t LowCluster[2];//文件起始簇号低16位
	uint8_t FileSize[4];//文件长度
};
struct longfile {
	uint8_t attributeOfFile;//属性字节
	uint8_t unicodeOfFile1[10];//长文件名Unicode码
	uint8_t longFileSymbol;//长文件名目录项标志
	uint8_t SystemReserve;//系统保留
	uint8_t checkNum;//校验值
	uint8_t unicodeOfFile2[12];//长文件名Unicode码
	uint8_t FileStartCluster[2];//文件起始簇号，常置为0
	uint8_t unicodeOfFile3[4];//长文件名Unicode码
};
//这里不建议使用链表的格式，还不如使用栈，这样可以先进后出，刚好符合长文件的处理
struct longfile_list
{
	struct longfile* lf;
	struct longfile_list* prev;
	struct longfile_list* next;
};
//rootdir（512字节）
struct rootdir {
	shortfile shortfile[16];//因为每次只能读512字节
};
struct rootdir1 {
	shortfile longfile[16];//因为每次只能读512字节
};

//fat表的基本信息
struct fatInfo {
	uint32_t reserveSector;//保留扇区
	uint32_t FatPerSector;//每个Fat表扇区数
	uint32_t fat1;//fat1起始扇区
	uint32_t fat2;//fat2起始扇区
	uint32_t rootdir;//rootdir起始扇区
	uint32_t SectorPercluster;//每个簇的扇区数
	//宽字符处理
		//因为长文件是宽字符来处理，包括你的中文应该也是会记成2个字节来算。
		//得到三个unicode的长度
		//然后进行字符拼接
	//	然后因为是一个宽字符，所以你们需要将两个字节拼接放入一个宽字符进行处理
		//这样提取处理后，你们可以选择使用wcout去输出宽字符，也可以将宽字符转为 string ，当然我更推荐后者。
		//可以用这个API去试着处理 WideCharToMultiByte ，转换成string。
	//	提醒在磁盘里面放文件，最好是先创建好的文件复制进去，因为如果你是新建文件再重命名，以前的 新建
};
//簇号链
struct clusterChain {
	uint32_t cluster[128];//每个clusterchain有128个目录项
};
struct fileInfo {
	std::string fileName;//文件名字
	uint32_t firstCluster;//首簇号
	uint8_t fileClass;
};

bool DBR_HexOutput(HANDLE hDevice, DBR* the_dbr, char* buffer, size_t len, fatInfo the_fatInfo) {
	//输出DBR
	cout << "===========================" << "DBR信息：" << "===========================================" << endl;
	int flag = 0;
	for (size_t i = 0; i < len; ++i)
	{
		unsigned char c = buffer[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if ((flag) % 16 == 0)
			printf("\n");
	}
	//fatInfo
	//保留扇区数，十进制
	uint8_t offsetadd[4] = { 0 };
	for (int j = 2, k = 1; j < 4; j++, k--) {
		offsetadd[j] = the_dbr->bpb.reserve_sectors[k];
	}
	uint32_t temp = transtoint(offsetadd);
	the_fatInfo.reserveSector = temp;

	//每个fat表的扇区数
	for (int j = 0; j < 4; j++) {
		offsetadd[j] = 0;
	}
	for (int j = 0, k = 3; j < 4; j++, k--) {
		offsetadd[j] = the_dbr->bpb.sectors_per_FAT[k];
	}
	temp = transtoint(offsetadd);
	the_fatInfo.FatPerSector = temp;

	//每簇扇区数
	for (int j = 0; j < 4; j++) {
		offsetadd[j] = 0;
	}
	offsetadd[3] = the_dbr->bpb.secotrs_per_cluster;
	temp = transtoint(offsetadd);
	the_fatInfo.SectorPercluster = temp;

	the_fatInfo.fat1 = the_fatInfo.reserveSector;
	the_fatInfo.fat2 = the_fatInfo.fat1 + the_fatInfo.FatPerSector;
	the_fatInfo.rootdir = the_fatInfo.fat2 + the_fatInfo.FatPerSector;
	//fatInfo
	cout << "保留扇区：" << the_fatInfo.reserveSector << endl;
	cout << "每个fat表的扇区数：" << the_fatInfo.FatPerSector << endl;
	cout << "fat1起始扇区：" << the_fatInfo.fat1 << endl;
	cout << "fat2起始扇区：" << the_fatInfo.fat2 << endl;
	cout << "rootdir起始扇区：" << the_fatInfo.rootdir << endl;
	cout << "每个簇的扇区数：" << the_fatInfo.SectorPercluster << endl;

	//============================分界线====================================================

		//读取fat表
	char buf[512] = { 0 };
	clusterChain* the_clusterChain;
	rootdir* the_rootdir;
	rootdir1* the_rootdir1;
	LARGE_INTEGER offset;
	int addr = 59336704 + the_fatInfo.fat1;
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

	DWORD dwCB;
	bool bRet = ReadFile(
		hDevice,
		&buf,
		512,
		&dwCB,
		NULL
	);
	cout << "===========================" << "FAT信息：" << "===========================================" << endl;
	flag = 0;
	for (size_t i = 0; i < 512; ++i)
	{
		unsigned char c = buf[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if ((flag) % 16 == 0)
			printf("\n");
	}
	//	cout<<"test11"<<endl;
	the_clusterChain = (struct clusterChain*)buf;
	//===================成功读取fat1表格==========================================

	//读取根目录
//	menset(buf,0,sizeof(buf)); 
	addr = 59336704 + the_fatInfo.rootdir;
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

	bRet = ReadFile(
		hDevice,
		&buf,
		512,
		&dwCB,
		NULL
	);
	cout << "===========================" << "根目录信息：" << "===========================================" << endl;
	flag = 0;
	for (size_t i = 0; i < 512; ++i)
	{
		unsigned char c = buf[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if ((flag) % 16 == 0)
			printf("\n");
	}
	//	cout<<"test11"<<endl;
	the_rootdir1 = (struct rootdir1*)buf;
	the_rootdir = (struct rootdir*)buf;
	

	//=============================成功读取根目录================================================================
	//目前只能读取根目录的第一个sector，其余sector暂时没有取出 
	for (int i = 0; i < 16; i++) {
		shortfile shstring = the_rootdir->shortfile[i];
		uint8_t thefirstbytes = shstring.FileName[0];

		//		uint16_t highcluster=shstring.HighCluster;
		//		uint16_t lowcluster=shstring.LowCluster;
		for (int j = 0; j < 4; j++) {
			offsetadd[j] = 0;
		}
		for (int j = 0, k = 1; j < 2; j++, k--) {
			offsetadd[j] = shstring.HighCluster[k];
		}
		for (int j = 2, k = 1; j < 4; j++, k--) {
			offsetadd[j] = shstring.LowCluster[k];
		}
		uint32_t chushicuhao = transtoint(offsetadd);

		for (int j = 0; j < 4; j++) {
			offsetadd[j] = 0;
		}
		for (int j = 0, k = 3; j < 4; j++, k--) {
			offsetadd[j] = shstring.FileSize[k];
		}
		uint32_t filesize = transtoint(offsetadd);

		//删除后第一个字节会变成E5 
		if (thefirstbytes == 0xe5) {
			continue;
		}

		int tempi;
		char longfiles[16] = { 0 };
		string filenames;
		longfile the_longfile;

		uint8_t shuxing = shstring.attributeOfFile;
		switch (shuxing) {
		case 0x00:
			cout << "读写" << endl;
			break;
		case 0x01:
			cout << "只读" << endl;
			break;
		case 0x02:
			cout << "隐藏" << endl;
			break;
		case 0x04:
			cout << "系统" << endl;
			break;
		case 0x08:
			cout << "卷标" << endl;
			break;
		case 0x10:
			cout << "子目录" << endl;
			cout << "起始簇号为：" << chushicuhao << endl;
			cout << "大小为：" << filesize << endl;
			//如果目录名很长，则还不能解决 
			cout << "文件名：" << shstring.FileName << endl;
			//这里可以加一个函数，输入参数为temp，作为起始簇号，转到fat表，溯源簇号链，得到完整簇号链 
			break;
		case 0x20:
			cout << "归档" << endl;
			cout << "起始簇号为：" << chushicuhao << endl;
			cout << "大小为：" << filesize << endl;
			cout << "文件名：" <<shstring.FileName<< endl; //还没解决 //打算建一个函数，传入参数为i，以i为开始，倒着来，
			//直到遇到E5或者4x就结束，i不可以直接减减，要有一个中间变量 
			tempi = i;
			//	//获取长文件名 
			
//			for (int j = tempi; j >= 0; j--) {
//				if (the_rootdir->shortfile[j].attributeOfFile == 0x0f && (the_rootdir->shortfile[j].FileName[0] != 0xe5)) {
//					for (int k = 0; k < 10; k++) {
//						char ctemp = the_rootdir1->longfile[j].unicodeOfFile1[k];
//						filenames += ctemp;
//					}
//					for (int k = 0; k < 12; k++) {
//						char ctemp = the_rootdir1->longfile[j].unicodeOfFile2[k];
//						filenames += ctemp;
//					}
//					for (int k = 0; k < 4; k++) {
//						char ctemp = the_rootdir1->longfile[j].unicodeOfFile3[k];
//						filenames += ctemp;
//					}
//
//				}
//			}
			//longfilename(the_rootdir, tempi, longname);
			break;
		case 0x0f:
			cout << "长目录项" << endl;
			//添加代码 
			break;
		default:
			break;
		}
	}


	//	for(int j=0;j<4;j++){
	//		offsetadd[j] = 0;
	//	}
	//	for (int j = 0, k = 3; j < 4; j++, k--) {
	//		offsetadd[j] = the_dbr->bpb.root_cluster_number[k];
	//	}
	//	int root_cluster = transtoint(offsetadd);

	//	cout<<"tess:";
	//	cout<<root_cluster<<endl;
	//	int rootsector=1;
	//	cout<<the_clusterChain->cluster[root_cluster]<<endl;
	//
	//	while (the_clusterChain->cluster[root_cluster] != 0xFFFFFF0F) {
	//		rootsector++;
	//		for(int j=0;j<4;j++){
	//			offsetadd[j] = 0;
	//		}
	//		offsetadd[3] =the_clusterChain->cluster[root_cluster];
	//		root_cluster = transtoint(offsetadd);
	//		cout << root_cluster << endl;
	//	}

};

//void longfilename(rootdir* the_rootdir, int i, char* buffer) {
//	//获取长文件名 
//	for (int j = i; j > 0; j--) {
//		if (the_rootdir->shortfile[j].attributeOfFile == 0x0f && (the_rootdir->shortfile[j].FileName[0] != 0xe5)) {
//
//		}
//	}
//};

int main() {
	HANDLE hDevice;
	DISK_GEOMETRY* pdg;
	BOOL bResult, bRet;
	DWORD junk, dwCB;
	DBR* the_dbr;
	fatInfo the_fatInfo = { 0 };
	fileInfo the_fileInfo;
	clusterChain the_clusterChain;
	rootdir the_rootdir;
	shortfile the_shortfile;
	longfile the_longfile;
	longfile_list the_longfile_list;
	char buffer[512] = { 0 };

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

	LARGE_INTEGER offset;
	int addr = 59336704;
	offset.QuadPart = (ULONGLONG)addr * (ULONGLONG)512;
	cout << offset.LowPart << endl;
	SetFilePointer(
		hDevice,
		offset.LowPart,
		&offset.HighPart,
		FILE_BEGIN
	);
	if (GetLastError())
		printf("/n");
	//		printf("错误类型代号：%ld\n\n", GetLastError());//如果出错了

	bRet = ReadFile(
		hDevice,
		&buffer,
		512,
		&dwCB,
		NULL
	);

	int flag = 0;
	for (size_t i = 0; i < 512; ++i)
	{
		unsigned char c = buffer[i]; // must use unsigned char to print >128 value 
		flag++;
		if (c < 16)
			printf("0%x ", c);
		else
			printf("%x ", c);
		if ((flag) % 16 == 0)
			printf("\n");
	}
	//================================================分界线=================================================================

	the_dbr = (struct DBR*)buffer;
	cout << "test 1/n" << endl;

	BOOL finded = 0;
	finded = DBR_HexOutput(hDevice, the_dbr, buffer, 512, the_fatInfo);


	return ((int)bResult);
}


