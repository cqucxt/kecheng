#include <windows.h>
#include <winioctl.h> //DDK�������������
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
#define BufferLength 1024
using namespace std;

//���ĸ������ֽڴ�ŵ�ֵתΪint��
uint32_t transtoint(unsigned char a[])
{
	uint32_t sum = 0UL;
	uint32_t temp = 0UL;
	
	for (int i = 3; i>=0; i--)
	{
		temp = a[i];//�ȸ�ֵ				
		temp = temp <<( 8 * (3-i));		
		sum = sum | temp;
				
	}
	return sum;
}
//ʮ���������
bool HexOutput(char* buf, size_t len,bool ismbr, ULONGLONG * baseaddr,ULONGLONG * nextaddr,int EBRnum)
{
	bool mbrflag=1;//�ڶ�ȡMBR��ʱ���ж���Ŀ��������������չ������Ŀ 
	unsigned char a = buf[0];
	if(ismbr)
		printf("-----------------------------------------------------");
	else
		printf("--------------------------��%d��EBR------------------",EBRnum);
	//printf("��һ�ֽ��ǣ�%x\n\n", a);
	if(ismbr)
		printf("          ��һ����(MBR)��\n\n");
	else
		printf("          ��һ����(EBR)��\n\n");
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
			printf("\n\n          �ڶ�����(������)��\n");
		}
		if (i == 509)
		{
			flag = 0;
			printf("\n\n          ��������(������־)��\n");
		}
		if ((flag) % 16 == 0)
			printf("\n");
	}
	printf("\n<-------------------��������Ϣ����------------------->\n\n");
	printf("\n\n������ַ�ʹ�С�ֱ�Ϊ: \n\n");	
	int limit=ismbr?509:477;//����Ѿ�֪������չ������EBR�����ö�ȡ��ô��,rank=1�Ƿ�����rank=2��ָ����һ����
	for (int m = 445, rank = 1; m < limit&&rank<=4; m += 16, rank++)
	{
		unsigned char fifth = buf[m + 5];//ȡ�õ���λ�ļ�ϵͳ��־λ��ʮ������Ϊ05H����0FH����չ���� 		
		if(fifth==0x5)//����չ������Ŀ������������� 
		{
			printf("This is an extend patition!\n");
			mbrflag=0;	
			rank = 4;
		}//����mbr����չ����EBR 
		if (fifth < 16) //���������ʽ
			printf("��%d�������־λΪ: 0%x\n", rank, fifth);
		else
			printf("��%d�������־λΪ: %x\n", rank, fifth);
		if (fifth == 0x00)//������λ����־λ����00ʱ�������������ϢΪ�գ��޷���
		{
			printf(" ������Ϊ��\n\n");//Ҳ����������� 
		}
		else {//������ 
			if(fifth==0xf||fifth==0x5)//����չ������Ŀ������������� 
			{
				printf("This is an extend patition!\n");
				mbrflag=0;	
//				rank = 4;
			}else{
				mbrflag=1; 
			} 
			unsigned char offsetadd[4] = { 0 };
			//���Ŷ������ֽ��ڸߵ�ַ��
			for (int n = m + 12, t = 0; n > m + 8, t < 4; n--, t++)
			{
				unsigned char temp = buf[n];
				if (temp < 16)
					printf("  0%x  ", temp);
				else
					printf("  %x  ", temp);
				offsetadd[t] = buf[n];
			}
			//�����ַ��ת��Ϊʮ����������LBA
			printf("\n");
			uint32_t tempadd = transtoint(offsetadd);
			printf("\n��ʼ��ַΪ�� %I64x", (ULONGLONG)tempadd * (ULONGLONG)512 + *baseaddr);
			
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
			printf("��С��");
			for (int p = m + 16, w = 0; p > m + 12, w < 4; p--, w++)
			{
				unsigned char temp1 = buf[p];
				if (temp1 < 16)
					printf("  0%x  ", temp1);
				else
					printf("  %x  ", temp1);
				offsetadd[w] = buf[p];
			}
			//�����С��ת��ΪGB��λ
			printf("\n");
			uint32_t tempsize = transtoint(offsetadd);
			
			if(ismbr && !mbrflag)
				printf("\n��չ���ܴ�СΪ�� %lu ���� = %lf GB \n", tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
			else if(!ismbr)
			{
				printf("\n��%d�߼��̴�СΪ�� %lu ���� = %lf GB \n",EBRnum, tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
			}
			else
				printf("\n���̴�СΪ�� %lu ���� = %lf GB \n", tempsize, ((double)tempsize / 2.0 / 1024.0 / 1024.0));
		}
	}
	printf("\n\n");
	if(ismbr){
		return 0;
	}else{
		return mbrflag;
	}
}

//������������������н������ֱ�õ�ÿ��������ƫ�Ƶ�ַ�Լ�������С
BOOL GetDriveGeometry(DISK_GEOMETRY *pdg, int addr)
{
	HANDLE hDevice;               // �豸���
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard resultscc
	char lpBuffer[BufferLength] = { 0 };


	//ͨ��CreateFile������豸�ľ��
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"), // �豸����,����ָ��һ��Ӳ��
		GENERIC_READ,                // no access to the drive
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // share mode
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		printf("Creatfile error!May be no permission!ERROR_ACCESS_DENIED��\n");
		return (FALSE);
	}

	//ͨ��DeviceIoControl�������豸����IO
	bResult = DeviceIoControl(hDevice, // �豸�ľ��
		IOCTL_DISK_GET_DRIVE_GEOMETRY, // �����룬ָ���豸������
		NULL, 
		0, // no input buffer
		pdg,
		sizeof(*pdg),     
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL); // synchronous I/O

	LARGE_INTEGER offset;//long long signed
	offset.QuadPart = (ULONGLONG)addr * (ULONGLONG)512;//0
	SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN);//��0��ʼ��MBR
	if(GetLastError())
		printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������
		
	DWORD dwCB;
	//�����λ�ÿ�ʼ�� 
	BOOL bRet = ReadFile(hDevice, lpBuffer, 512, &dwCB, NULL);
	//�������MBR��
	bool finished=0; 
	int EBRnum=0;
	ULONGLONG *baseaddr=new ULONGLONG,*nextaddr= new ULONGLONG;//��չ������ʼ��ַ��EBR��ַ 
	*baseaddr = (ULONGLONG)0;
	*nextaddr = (ULONGLONG)0;
	finished=HexOutput(lpBuffer, 512,true,baseaddr,nextaddr,EBRnum);//���Ƕ�ȡMBR
	
	if(finished)
		CloseHandle(hDevice);
	else
	{
		//������
		do{
			EBRnum++;
			memset(lpBuffer, 0, sizeof(lpBuffer));
			offset.QuadPart = (ULONGLONG)(*baseaddr + *nextaddr);//find the EBR
			cout<<"test test tes :"<<offset.QuadPart<<"  "<<offset.LowPart<<endl;
//			cout<<"\ntest "<<*baseaddr<<" "<<*nextaddr<<" "<<*baseaddt+*nextaddr<<endl;
			SetFilePointer(hDevice, offset.LowPart, &offset.HighPart, FILE_BEGIN);//��EBR
			if(GetLastError())
				printf("�������ʹ��ţ�%ld\n\n", GetLastError());//���������
			
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
	DISK_GEOMETRY pdg;            // ������̲����Ľṹ��
	BOOL bResult;                 // generic results flag
	ULONGLONG DiskSize;           // size of the drive, in bytes
	printf("<-----------------��ӭʹ�÷�����ȡ����----------------->\n\n");
	bResult = GetDriveGeometry(&pdg, 0);

	if (bResult)
	{
		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
			(ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;

		printf("�����ܴ�С = %I64d (Bytes) = %I64d (Gb)\n", DiskSize, DiskSize / (1024 * 1024 * 1024));
	}
	else
	{
		printf("GetDriveGeometry failed. Error %ld.\n", GetLastError());
	}
	return ((int)bResult);
}
