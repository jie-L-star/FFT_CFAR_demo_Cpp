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
#include <ws2tcpip.h> // 提供套接字地址结构和函数原型，如inet_pton
#include <iphlpapi.h>
#include <windows.h>// 提供获取网络适配器信息的函数原型，如GetAdaptersAddresses
#include<stdint.h>
 #include <sys/types.h>       

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define DATA_LENGTH 1854 * 4
#define ANT_LENGTH 4

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
    int16_t CRSData[1584 * 2 * ANT_LENGTH];
} TransmissionDataHdr, * PTransmissionDataHdr;


//设置IP为本地IP，端口选择一个未占用端口即可

int main()
{
    static TransmissionDataHdr transmissionData = { 0 };
    transmissionData.NumTx = 1;
    transmissionData.NumRx = 2;
    transmissionData.nCellIdx = 1;
    transmissionData.nChannelNum = 10;
    transmissionData.TransmitBeamIdx = 64;
    transmissionData.ReceiveBeamIdx = 32;
    transmissionData.nSlotNum = 10;
    transmissionData.nRxFrameNum = 0;

    FILE* file;
    // 读取本地bin文件，虚拟发射
    // 打开二进制文件
    file = fopen("D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/dataContent_00000_039_129.bin", "rb");

    

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // 读取二进制文件内容到结构体中
    if (fread(transmissionData.CRSData, sizeof(transmissionData.CRSData), 1, file) != 1) {
        perror("Error reading file");
        fclose(file);
        return -1;
    }

    uint32_t ttemp;
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
    saddr.sin_addr.s_addr = inet_addr("172.18.112.1");
    printf("172.18.112.1\n");
    while (1)
    {
        sendto(sockfd, (char*)&transmissionData, sizeof(transmissionData), 0, (struct sockaddr*)&saddr, sizeof(saddr));
        Sleep(10);
        //usleep(500)
        transmissionData.nSlotNum = ttemp;
        ttemp = ttemp + 1;
    }
    close(sockfd);

    // 在程序结束前，确保调用 WSACleanup 来释放Winsock库占用的资源
    WSACleanup();
    return 0;
}

