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

struct BPB {
	uint8_t bytes_per_sector[2];//�����ֽ���
	uint8_t secotrs_per_cluster;//ÿ��������
	uint8_t reserve_sectors[2];//����DBR�Լ����ڵ�FAT֮ǰ����������,������������
	uint8_t FATnum;//FAT������һ��Ϊ2
	uint8_t unimportant1[11];
	uint8_t DBR_LBA[4];//�÷�����DBR���ڵ���������ţ��������չ���������������չ�����׵�
	uint8_t totalsectors[4];//����������������
	uint8_t sectors_per_FAT[4];//ÿ��FAT��������
	uint8_t unimportant2[4];
	uint8_t root_cluster_number[4];//��Ŀ¼�غ�
	uint8_t file_info[2];
	uint8_t backup_DBR[2];//�������������������DBR�������ţ�һ��Ϊ6�����ݺ�DBRһģһ��
	uint8_t zero1[12];
	uint8_t extBPB[26];//��չBPB
};
struct DBR {
	uint8_t jumpcode[3];//EB 58 90
	uint8_t OEM[8];//OEM����
	BPB bpb;
	uint8_t osboot[422];//���������55AA
};
struct shortfile {
	uint8_t FileName[8];//�ļ���
	uint8_t ExtendName[3];//��չ��
	uint8_t attributeOfFile;//�����ֽ�
	uint8_t SystemReserve;//ϵͳ����
	uint8_t CreateTime_ms;//����ʱ���10����λ
	uint8_t CreateTime[2];//����ʱ��
	uint8_t CreateDate[2];//��������
	uint8_t LastAccess[2];//����������
	uint8_t HighCluster[2];//�ļ���ʼ�غŸ�16λ
	uint8_t LastModifyTime[2];//����޸�ʱ��
	uint8_t LastModifyDate[2];//����޸�ʱ��
	uint8_t LowCluster[2];//�ļ���ʼ�غŵ�16λ
	uint8_t FileSize[4];//�ļ�����
};
struct longfile {
	uint8_t attributeOfFile;//�����ֽ�
	uint8_t unicodeOfFile1[10];//���ļ���Unicode��
	uint8_t longFileSymbol;//���ļ���Ŀ¼���־
	uint8_t SystemReserve;//ϵͳ����
	uint8_t checkNum;//У��ֵ
	uint8_t unicodeOfFile2[12];//���ļ���Unicode��
	uint8_t FileStartCluster[2];//�ļ���ʼ�غţ�����Ϊ0
	uint8_t unicodeOfFile3[4];//���ļ���Unicode��
};
//���ﲻ����ʹ������ĸ�ʽ��������ʹ��ջ�����������Ƚ�������պ÷��ϳ��ļ��Ĵ���
struct longfile_list
{
	struct longfile* lf;
	struct longfile_list* prev;
	struct longfile_list* next;
};
//rootdir��512�ֽڣ�
struct rootdir {
	shortfile shortfile[16];//��Ϊÿ��ֻ�ܶ�512�ֽ�
};
struct rootdir1 {
	shortfile longfile[16];//��Ϊÿ��ֻ�ܶ�512�ֽ�
};

//fat��Ļ�����Ϣ
struct fatInfo {
	uint32_t reserveSector;//��������
	uint32_t FatPerSector;//ÿ��Fat��������
	uint32_t fat1;//fat1��ʼ����
	uint32_t fat2;//fat2��ʼ����
	uint32_t rootdir;//rootdir��ʼ����
	uint32_t SectorPercluster;//ÿ���ص�������
	//���ַ�����
		//��Ϊ���ļ��ǿ��ַ������������������Ӧ��Ҳ�ǻ�ǳ�2���ֽ����㡣
		//�õ�����unicode�ĳ���
		//Ȼ������ַ�ƴ��
	//	Ȼ����Ϊ��һ�����ַ�������������Ҫ�������ֽ�ƴ�ӷ���һ�����ַ����д���
		//������ȡ��������ǿ���ѡ��ʹ��wcoutȥ������ַ���Ҳ���Խ����ַ�תΪ string ����Ȼ�Ҹ��Ƽ����ߡ�
		//���������APIȥ���Ŵ��� WideCharToMultiByte ��ת����string��
	//	�����ڴ���������ļ���������ȴ����õ��ļ����ƽ�ȥ����Ϊ��������½��ļ�������������ǰ�� �½�
};
//�غ���
struct clusterChain {
	uint32_t cluster[128];//ÿ��clusterchain��128��Ŀ¼��
};
struct fileInfo {
	std::string fileName;//�ļ�����
	uint32_t firstCluster;//�״غ�
	uint8_t fileClass;
};

bool DBR_HexOutput(HANDLE hDevice, DBR* the_dbr, char* buffer, size_t len, fatInfo the_fatInfo) {
	//���DBR
	cout << "===========================" << "DBR��Ϣ��" << "===========================================" << endl;
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
	//������������ʮ����
	uint8_t offsetadd[4] = { 0 };
	for (int j = 2, k = 1; j < 4; j++, k--) {
		offsetadd[j] = the_dbr->bpb.reserve_sectors[k];
	}
	uint32_t temp = transtoint(offsetadd);
	the_fatInfo.reserveSector = temp;

	//ÿ��fat���������
	for (int j = 0; j < 4; j++) {
		offsetadd[j] = 0;
	}
	for (int j = 0, k = 3; j < 4; j++, k--) {
		offsetadd[j] = the_dbr->bpb.sectors_per_FAT[k];
	}
	temp = transtoint(offsetadd);
	the_fatInfo.FatPerSector = temp;

	//ÿ��������
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
	cout << "����������" << the_fatInfo.reserveSector << endl;
	cout << "ÿ��fat�����������" << the_fatInfo.FatPerSector << endl;
	cout << "fat1��ʼ������" << the_fatInfo.fat1 << endl;
	cout << "fat2��ʼ������" << the_fatInfo.fat2 << endl;
	cout << "rootdir��ʼ������" << the_fatInfo.rootdir << endl;
	cout << "ÿ���ص���������" << the_fatInfo.SectorPercluster << endl;

	//============================�ֽ���====================================================

		//��ȡfat��
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
	//		printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������

	DWORD dwCB;
	bool bRet = ReadFile(
		hDevice,
		&buf,
		512,
		&dwCB,
		NULL
	);
	cout << "===========================" << "FAT��Ϣ��" << "===========================================" << endl;
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
	//===================�ɹ���ȡfat1���==========================================

	//��ȡ��Ŀ¼
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
	//		printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������

	bRet = ReadFile(
		hDevice,
		&buf,
		512,
		&dwCB,
		NULL
	);
	cout << "===========================" << "��Ŀ¼��Ϣ��" << "===========================================" << endl;
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
	

	//=============================�ɹ���ȡ��Ŀ¼================================================================
	//Ŀǰֻ�ܶ�ȡ��Ŀ¼�ĵ�һ��sector������sector��ʱû��ȡ�� 
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

		//ɾ�����һ���ֽڻ���E5 
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
			cout << "��д" << endl;
			break;
		case 0x01:
			cout << "ֻ��" << endl;
			break;
		case 0x02:
			cout << "����" << endl;
			break;
		case 0x04:
			cout << "ϵͳ" << endl;
			break;
		case 0x08:
			cout << "���" << endl;
			break;
		case 0x10:
			cout << "��Ŀ¼" << endl;
			cout << "��ʼ�غ�Ϊ��" << chushicuhao << endl;
			cout << "��СΪ��" << filesize << endl;
			//���Ŀ¼���ܳ����򻹲��ܽ�� 
			cout << "�ļ�����" << shstring.FileName << endl;
			//������Լ�һ���������������Ϊtemp����Ϊ��ʼ�غţ�ת��fat����Դ�غ������õ������غ��� 
			break;
		case 0x20:
			cout << "�鵵" << endl;
			cout << "��ʼ�غ�Ϊ��" << chushicuhao << endl;
			cout << "��СΪ��" << filesize << endl;
			cout << "�ļ�����" <<shstring.FileName<< endl; //��û��� //���㽨һ���������������Ϊi����iΪ��ʼ����������
			//ֱ������E5����4x�ͽ�����i������ֱ�Ӽ�����Ҫ��һ���м���� 
			tempi = i;
			//	//��ȡ���ļ��� 
			
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
			cout << "��Ŀ¼��" << endl;
			//��Ӵ��� 
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
//	//��ȡ���ļ��� 
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
	//		printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������

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
	//================================================�ֽ���=================================================================

	the_dbr = (struct DBR*)buffer;
	cout << "test 1/n" << endl;

	BOOL finded = 0;
	finded = DBR_HexOutput(hDevice, the_dbr, buffer, 512, the_fatInfo);


	return ((int)bResult);
}


