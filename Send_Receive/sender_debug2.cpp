#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <winsock2.h>
#include <string.h>
#include <assert.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <net/if.h>
//#include <sys/ioctl.h>
#include <iostream>
#include <ws2tcpip.h> // 提供套接字地址结构和函数原型，如inet_pton
#include <iphlpapi.h>
#include <windows.h>// 提供获取网络适配器信息的函数原型，如GetAdaptersAddresses
#include<stdint.h>
#include <sys/types.h>       
//
// <fstream>

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define DATA_LENGTH 1584 * 2
#define ANT_LENGTH 6

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

    WIN32_FIND_DATAA find_data;
    HANDLE handle;
    char search_path[MAX_PATH] = "D:/FFT_CFAR_demo_Cpp/datareal/96/013/*"; // replace with your directory

    handle = FindFirstFileA(search_path, &find_data);
    if (handle == INVALID_HANDLE_VALUE) {
        std::cerr << "FindFirstFile failed with error " << GetLastError() << std::endl;
        return 1;
    }
    uint32_t ttemp;
    ttemp = 0;
    do {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Ignore directories
        }
        else {
            std::cout << "Found file: " << find_data.cFileName << std::endl;

            // Now you can use fopen to open the file and read its content
            char file_path[MAX_PATH];
            sprintf(file_path, "D:/FFT_CFAR_demo_Cpp/datareal/96/013/%s", find_data.cFileName); // replace with your directory
            FILE* file = fopen(file_path, "rb");
            if (file == NULL) {
                std::cerr << "Failed to open file " << file_path << std::endl;
            }
            else {
                if (fread(transmissionData.CRSData, sizeof(transmissionData.CRSData), 1, file) != 1) {
                    perror("Error reading file");
                    fclose(file);
                    return -1;
                }


                WSADATA wsaData;
                int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
                if (result != 0) {
                    printf("WSAStartup failed with error: %d\n", result);
                    return -1;
                }

                // 现在可以安全地使用Winsock API了


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
                //printf("121.248.52.93\n");
                //while (1)
                //{
                sendto(sockfd, (char*)&transmissionData, sizeof(transmissionData), 0, (struct sockaddr*)&saddr, sizeof(saddr));
                Sleep(10);
                //usleep(500)
                transmissionData.nSlotNum = ttemp;
                ttemp = ttemp + 1;
                if (ttemp == 200)
                {
                    ttemp = 0;
                }
                //}
                closesocket(sockfd);

                // 在程序结束前，确保调用 WSACleanup 来释放Winsock库占用的资源
                WSACleanup();
                
                
                // Read the file content...
                fclose(file);
            }
        }
    } while (FindNextFileA(handle, &find_data) != 0);

    FindClose(handle);
    return 0;
}


    //process_directory("D:/FFT_CFAR_demo/datareal/96/013/");
    /*FILE* file;
    // 读取本地bin文件，虚拟发射
    // 打开二进制文件
    file = fopen("D:/FFT_CFAR_demo/datareal/96/013/dataContent_00000_039_129.bin", "rb");

    

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }*/

    // 读取二进制文件内容到结构体中


    /*uint32_t ttemp;
    ttemp = 0;


    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return -1;
    }

    // 现在可以安全地使用Winsock API了


    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(12345);
    saddr.sin_addr.s_addr = inet_addr("121.248.54.68");
    printf("121.248.54.68\n");
    while (1)
    {
        sendto(sockfd, (char*)&transmissionData, sizeof(transmissionData), 0, (struct sockaddr*)&saddr, sizeof(saddr));
        Sleep(1);
        //usleep(500)
        transmissionData.nSlotNum = ttemp;
        ttemp = ttemp + 1;
    }
    close(sockfd);

    // 在程序结束前，确保调用 WSACleanup 来释放Winsock库占用的资源
    WSACleanup();
    return 0;*/


/*void process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        printf("Open file: %s\n", filename);
        // Do something with the file.
        if (fread(transmissionData.CRSData, sizeof(transmissionData.CRSData), 1, file) != 1) {
            perror("Error reading file");
            fclose(file);
            return -1;
        }
        fclose(file);
    }
    else {
        printf("Cannot open file: %s\n", filename);
    }
}

void process_directory(const char* dirname) {
    WIN32_FIND_DATA find_data;
    char search_path[MAX_PATH];

    sprintf(search_path, "%s\\*", dirname);
    HANDLE handle = FindFirstFile(search_path, &find_data);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
                char file_path[MAX_PATH];
                sprintf(file_path, "%s\\%s", dirname, find_data.cFileName);
                process_file(file_path);
            }
        } while (FindNextFile(handle, &find_data));
        FindClose(handle);
    }
}
*/