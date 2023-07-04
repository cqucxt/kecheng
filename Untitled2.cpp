#include <windows.h>
#include <winioctl.h> //DDK�������������
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
using namespace std;

typedef unsigned char uint8_t;

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <windows.h>

using namespace std;

#pragma pack(push,1)

// DOS Header
//struct IMAGE_DOS_HEADER {
//    uint16_t e_magic;
//    uint16_t e_cblp;
//    uint16_t e_cp;
//    uint16_t e_crlc;
//    uint16_t e_cparhdr;
//    uint16_t e_minalloc;
//    uint16_t e_maxalloc;
//    uint16_t e_ss;
//    uint16_t e_sp;
//    uint16_t e_csum;
//    uint16_t e_ip;
//    uint16_t e_cs;
//    uint16_t e_lfarlc;
//    uint16_t e_ovno;
//    uint16_t e_res[4];
//    uint16_t e_oemid;
//    uint16_t e_oeminfo;
//    uint16_t e_res2[10];
//    uint32_t e_lfanew;
//};
//
//// File Header
//struct IMAGE_FILE_HEADER {
//    uint16_t Machine;
//    uint16_t NumberOfSections;
//    uint32_t TimeDateStamp;
//    uint32_t PointerToSymbolTable;
//    uint32_t NumberOfSymbols;
//    uint16_t SizeOfOptionalHeader;
//    uint16_t Characteristics;
//};
//
//// Optional Header
//struct IMAGE_OPTIONAL_HEADER {
//    // ��׼�ֶΣ�����ȷ���ļ���Ϣ
//    uint16_t Magic;
//    uint8_t MajorLinkerVersion;
//    uint8_t MinorLinkerVersion;
//    uint32_t SizeOfCode;
//    uint32_t SizeOfInitializedData;
//    uint32_t SizeOfUninitializedData;
//    uint32_t AddressOfEntryPoint;
//    uint32_t BaseOfCode;
//
//    // NT additional fields�����ڲ���ϵͳ�ض���Ϣ
//    uint32_t ImageBase;
//    uint32_t SectionAlignment;
//    uint32_t FileAlignment;
//    uint16_t MajorOperatingSystemVersion;
//    uint16_t MinorOperatingSystemVersion;
//    uint16_t MajorImageVersion;
//    uint16_t MinorImageVersion;
//    uint16_t MajorSubsystemVersion;
//    uint16_t MinorSubsystemVersion;
//    uint32_t Win32VersionValue;
//    uint32_t SizeOfImage;
//    uint32_t SizeOfHeaders;
//    uint32_t CheckSum;
//    uint16_t Subsystem;
//    uint16_t DllCharacteristics;
//    uint32_t SizeOfStackReserve;
//    uint32_t SizeOfStackCommit;
//    uint32_t SizeOfHeapReserve;
//    uint32_t SizeOfHeapCommit;
//    uint32_t LoaderFlags;
//    uint32_t NumberOfRvaAndSizes;
//};
//
//// Section Header
//struct IMAGE_SECTION_HEADER {
//    uint8_t Name[8];
//    uint32_t VirtualSize;
//    uint32_t VirtualAddress;
//    uint32_t SizeOfRawData;
//    uint32_t PointerToRawData;
//    uint32_t PointerToRelocations;
//    uint32_t PointerToLinenumbers;
//    uint16_t NumberOfRelocations;
//    uint16_t NumberOfLinenumbers;
//    uint32_t Characteristics;
//};

//#pragma pack(pop)
//
//struct IMAGE_FIRST_SECTION {
//    IMAGE_DOS_HEADER dos_header;
//    IMAGE_NT_HEADERS nt_headers;
//    IMAGE_SECTION_HEADER section_headers[1];
//};

int main(int argc, char* argv[])
{
    HANDLE hDevice;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <PE_file_path>" << endl;
        return 1;
    }

    hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"),// ͨ�����̺Ż�ȡ�豸���
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        cerr << "Error: Cannot open physical disk." << endl;
        return 1;
    }

    DWORD exesector = 0; // �ļ��ڴ��̵���
    // ��ȡ���̲���
    DISK_GEOMETRY pdg;
    DWORD cbBytesReturned;
    if (!DeviceIoControl(
        hDevice,
        IOCTL_DISK_GET_DRIVE_GEOMETRY,
        NULL, 0,
        &pdg, sizeof(pdg),
        &cbBytesReturned,
        NULL)) {
        cerr << "Error: Cannot retrieve disk geometry information." << endl;
        CloseHandle(hDevice);
        return 1;
    }

    // ���� PE �ļ��ڴ��̵�������ƫ����
    const uint64_t SectorSize = 512;
    uint64_t offset = exesector * SectorSize;
    LARGE_INTEGER liOffset;
    liOffset.QuadPart = offset;

    // ��λ�� PE �ļ�λ��
    if (SetFilePointerEx(hDevice, liOffset, NULL, FILE_BEGIN) != liOffset.LowPart) {
        cerr << "Error: Cannot set file pointer." << endl;
        CloseHandle(hDevice);
        return 1;
    }

    // �����ڴ����洢����ȡ PE �ļ�ͷ
    const uint32_t cbMaxRead = 32 * 1024; // �����ȡ 32KB
    uint8_t* pvRead = new uint8_t[cbMaxRead];
    DWORD cbRead;
    if (!ReadFile(hDevice, pvRead, cbMaxRead, &cbRead, NULL) || cbRead < sizeof(IMAGE_DOS_HEADER)) {
        cerr << "Error: Cannot read PE file." << endl;
        CloseHandle(hDevice);
        delete[] pvRead;
        return 1;
    }

    // ���� DOS ͷ
    IMAGE_DOS_HEADER* pdh = reinterpret_cast<IMAGE_DOS_HEADER*>(pvRead);
    if (pdh->e_magic != IMAGE_DOS_SIGNATURE) {
        cerr << "Error: Invalid DOS signature." << endl;
        CloseHandle(hDevice);
        delete[] pvRead;
        return 1;
    }

    // ���� NT ͷƫ����
    uint64_t offset_nt_header = pdh->e_lfanew;
    liOffset.QuadPart = offset + offset_nt_header;

    // �����ڴ����洢����ȡ NT ͷ
    const uint32_t cbMaxRead_nt_header = sizeof(IMAGE_NT_HEADERS);
    uint8_t* pvRead_nt_header = new uint8_t[cbMaxRead_nt_header];
    DWORD cbRead_nt_header;
    if (!ReadFile(hDevice, pvRead_nt_header, cbMaxRead_nt_header,
        &cbRead_nt_header, NULL) ||
        cbRead_nt_header != cbMaxRead_nt_header) {
        cerr << "Error: Cannot read NT header." << endl;
        CloseHandle(hDevice);
        delete[] pvRead;
        delete[] pvRead_nt_header;
        return 1;
    }

    // ת��Ϊ NT ͷָ��
    IMAGE_NT_HEADERS* pnt = reinterpret_cast<IMAGE_NT_HEADERS*>(pvRead_nt_header);

    // ��֤ NT ͷ����
    if (pnt->Signature != IMAGE_NT_SIGNATURE) {
        cerr << "Error: Invalid NT signature." << endl;
        CloseHandle(hDevice);
        delete[] pvRead;
        delete[] pvRead_nt_header;
        return 1;
    }

    // ����������ָ��
    IMAGE_SECTION_HEADER* pSections =
        reinterpret_cast<IMAGE_SECTION_HEADER*>(
            reinterpret_cast<uint8_t*>(pnt) +
            sizeof(IMAGE_NT_HEADERS));

    // ����������
    const int nSections = pnt->FileHeader.NumberOfSections;
    cout << "Number of sections: " << nSections << endl << endl;
    cout << setw(10) << left << "Name"
        << setw(10) << left << "VirtSize"
        << setw(10) << left << "VirtAddr"
        << setw(10) << left << "RawSize"
        << setw(10) << left << "RawAddr" << endl;
    for (int i = 0; i < nSections; i++) {
        const IMAGE_SECTION_HEADER& section = pSections[i];

        // ��ý�����
        char Name[9];
        memcpy(Name, section.Name, 8);
        Name[8] = '\0';

        // ��ӡ��������Ϣ
        cout << setw(10) << left << Name
            << setw(10) << left << section.Misc.VirtualSize
            << setw(10) << left << section.VirtualAddress
            << setw(10) << left << section.SizeOfRawData
            << setw(10) << left << section.PointerToRawData << endl;
    }

    // ������ѡͷ
    IMAGE_OPTIONAL_HEADER& optionalHeader = pnt->OptionalHeader;

    cout << endl;
    cout << "ImageBase: 0x" << hex << optionalHeader.ImageBase << endl;
    cout << "EntryPoint RVA: 0x" << hex << optionalHeader.AddressOfEntryPoint << endl;
    cout << "SizeOfImage: 0x" << hex << optionalHeader.SizeOfImage << endl;
    cout << "Subsystem: " << dec << optionalHeader.Subsystem << endl;

    // �����ڴ沢�رվ��
    delete[] pvRead;
    delete[] pvRead_nt_header;
    CloseHandle(hDevice);

    return 0;
}
