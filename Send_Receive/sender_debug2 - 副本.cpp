#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <ws2tcpip.h> // 提供套接字地址结构和函数原型，如inet_pton
#include <iphlpapi.h>
#include <windows.h>// 提供获取网络适配器信息的函数原型，如GetAdaptersAddresses
#include<stdint.h>
#include <sys/types.h>       


#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define DATA_LENGTH 1584 * 2
#define ANT_LENGTH 8
#define ANT_DATA_1_LENGTH 2
#define ANT_DATA_2_LENGTH 6

typedef struct TransmissionData
{
    // 基站身份标识
    uint16_t nCellIdx;
    char BBU_Idx[20];
    // 通道数
    uint8_t NumTx;
    uint8_t NumRx;
    // 通道身份标识
    uint8_t nChannelNum;
    // 扫描波束的参数
    uint16_t TransmitBeamIdx;
    uint16_t ReceiveBeamIdx;
    // 时间标识
    uint32_t nSlotNum;
    uint32_t nRxFrameNum;
    // SRS接收数据
    int16_t CRSData[DATA_LENGTH * ANT_LENGTH];
} TransmissionDataHdr, * PTransmissionDataHdr;


//设置IP为本地IP，端口选择一个未占用端口即可

int main() {
    static TransmissionDataHdr transmissionData = { 0 };
    transmissionData.NumTx = 1;
    transmissionData.NumRx = 2;
    transmissionData.nCellIdx = 1;
    transmissionData.nChannelNum = 10;
    transmissionData.TransmitBeamIdx = 64;
    transmissionData.ReceiveBeamIdx = 32;
    transmissionData.nSlotNum = 10;
    transmissionData.nRxFrameNum = 0;

    char search_path1[MAX_PATH] = "D:/FFT_CFAR_demo_Cpp/datareal/95/013/*"; // replace with your directory
    char search_path2[MAX_PATH] = "D:/FFT_CFAR_demo_Cpp/datareal/96/013/*"; // replace with your directory
    char file_path1[MAX_PATH], file_path2[MAX_PATH];
    FILE* file1, * file2;

    WIN32_FIND_DATAA find_data1, find_data2;
    HANDLE handle1 = FindFirstFileA(search_path1, &find_data1);
    HANDLE handle2 = FindFirstFileA(search_path2, &find_data2);
    
    if ((handle1 == INVALID_HANDLE_VALUE)|| (handle2 == INVALID_HANDLE_VALUE)) {
        std::cerr << "FindFirstFile failed with error " << GetLastError() << std::endl;
        return 1;
    }

    // socket部分
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(12345);
    saddr.sin_addr.s_addr = inet_addr("169.254.232.2");

    uint32_t ttemp;
    ttemp = 0;
    do {
        if ((find_data1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (find_data2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
            // Ignore directories
        else {
            std::cout << "Found file: " << find_data1.cFileName << "    " << find_data2.cFileName << std::endl;

            // Now you can use fopen to open the file and read its content
            sprintf(file_path1, "D:/FFT_CFAR_demo_Cpp/datareal/95/013/%s", find_data1.cFileName); // replace with your directory
            sprintf(file_path2, "D:/FFT_CFAR_demo_Cpp/datareal/96/013/%s", find_data2.cFileName); // replace with your directory

            //FILE* 
                file1 = fopen(file_path1, "rb");
            //FILE* 
                file2 = fopen(file_path2, "rb");

            //fread(transmissionData.CRSData, sizeof(transmissionData.CRSData) * ANT_DATA_1_LENGTH / ANT_LENGTH, 1, file1);
            //fread(&(transmissionData.CRSData[ANT_DATA_1_LENGTH * DATA_LENGTH]), sizeof(transmissionData.CRSData) * ANT_DATA_2_LENGTH / ANT_LENGTH, 1, file2);

            if (file1 == NULL)      std::cerr << "Failed to open file " << file_path1 << std::endl;
            else if (file2 == NULL) std::cerr << "Failed to open file " << file_path2 << std::endl;

            else {
                //写在for中第二个文件读不到
                bool flag1 = fread(transmissionData.CRSData, sizeof(transmissionData.CRSData) * ANT_DATA_1_LENGTH / ANT_LENGTH, 1, file1) != 1;
                bool flag2 = fread(&(transmissionData.CRSData[ANT_DATA_1_LENGTH * DATA_LENGTH]), sizeof(transmissionData.CRSData) * ANT_DATA_2_LENGTH / ANT_LENGTH, 1, file2) != 1;

                if (flag1 && flag2){
                    perror("Error reading file");
                    fclose(file1);
                    fclose(file2);
                    return -1;
                }

                //// socket部分
                //WSADATA wsaData;
                //int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
                //if (result != 0) {
                //    printf("WSAStartup failed with error: %d\n", result);
                //    return -1;
                //}

                //int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                //if (sockfd == -1) {
                //    printf("Socket creation failed with error: %d\n", WSAGetLastError());
                //    return -1;
                //}

                //struct sockaddr_in saddr;
                //memset(&saddr, 0, sizeof(saddr));
                //saddr.sin_family = AF_INET;
                //saddr.sin_port = htons(12345);
                //saddr.sin_addr.s_addr = inet_addr("169.254.232.2");

                sendto(sockfd, (char*)&transmissionData, sizeof(transmissionData), 0, (struct sockaddr*)&saddr, sizeof(saddr));
                Sleep(10);
                transmissionData.nSlotNum = ttemp;
                ttemp = ttemp + 1;
                if (ttemp == 200)
                {
                    ttemp = 0;
                }

                fclose(file1);
                fclose(file2);
            }
        }
    } while ((FindNextFileA(handle1, &find_data1) != 0) && (FindNextFileA(handle2, &find_data2) != 0));

    closesocket(sockfd);
    WSACleanup();
    FindClose(handle1);
    FindClose(handle2);
    return 0;
}
