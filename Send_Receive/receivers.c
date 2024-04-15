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
#include <ws2tcpip.h> // �ṩ�׽��ֵ�ַ�ṹ�ͺ���ԭ�ͣ���inet_pton
#include <iphlpapi.h>
#include <windows.h>// �ṩ��ȡ������������Ϣ�ĺ���ԭ�ͣ���GetAdaptersAddresses
#include<stdint.h>
#include <sys/types.h>       

#define DATA_LENGTH 1854 * 4
#define ANT_LENGTH 2
#define MODSAMV 8

typedef int bool;
#define true 1
#define false 0

typedef struct TransmissionData
{
    // ��վ��ݱ�ʶ
    uint16_t nCellIdx;
    char BBU_Idx[20];
    // ͨ����
    uint8_t NumTx;
    uint8_t NumRx;
    // ͨ����ݱ�ʶ
    uint8_t nChannelNum;
    // ɨ�貨���Ĳ���
    uint16_t TransmitBeamIdx;
    uint16_t ReceiveBeamIdx;
    // ʱ���ʶ
    uint32_t nSlotNum;
    uint32_t nRxFrameNum;
    // SRS��������
    int16_t CRSData[1584 * 2 * ANT_LENGTH];
} TransmissionDataHdr, * PTransmissionDataHdr;


bool beamselectFlag(int cmd_beam_tx, int cmd_beam_rx, const TransmissionDataHdr* pData);
int writeHeaderToTxt(const TransmissionDataHdr* ptransmissionData, int conti);
int writeDataToBin(const TransmissionDataHdr* ptransmissionData, int conti);

bool beamselectFlag(int cmd_beam_tx, int cmd_beam_rx, const TransmissionDataHdr* pData) {
    return (cmd_beam_tx == -1 || pData->TransmitBeamIdx == cmd_beam_tx) &&
        (cmd_beam_rx == -1 || pData->ReceiveBeamIdx == cmd_beam_rx);
}
// uint16_t beamselectFlag(int cmd_beam_tx, int cmd_beam_rx, const TransmissionDataHdr *pData) {
//     // case1: no requirement of beam tx and rx
//     if (cmd_beam_tx == -1 && cmd_beam_rx ==-1) {
//         return 1;
//     }
//     // case2: only requirement of beam tx
//     if (cmd_beam_rx == -1 && cmd_beam_tx !=-1) {
//         if(pData->TransmitBeamIdx == cmd_beam_tx){
//             return 1;
//         } 
//     }
//     // case3: only requirement of beam rx
//     if (cmd_beam_tx == -1 && cmd_beam_rx !=-1) {
//         if(pData->ReceiveBeamIdx == cmd_beam_rx){
//             return 1;
//         } 
//     }
//     // case4: requirement of beam tx and rx
//     if (cmd_beam_tx != -1 && cmd_beam_rx !=-1) {
//         if(pData->TransmitBeamIdx == cmd_beam_tx&&pData->ReceiveBeamIdx == cmd_beam_rx){
//             return 1;
//         } 
//     }
//     return false;
// }

int writeHeaderToTxt(const TransmissionDataHdr* ptransmissionData, int conti) {
    char filename[256]; // ���ӻ�������С�Ա���Ǳ�ڵ����
    snprintf(filename, sizeof(filename), "data/dataHeader_%06d_%03d_%03d.txt",
        conti, ptransmissionData->TransmitBeamIdx, ptransmissionData->ReceiveBeamIdx);

    FILE* fpWrite = fopen(filename, "w");
    if (fpWrite == NULL) {
        perror("fopen error");
        return -1;
        exit(EXIT_FAILURE);
    }

    fprintf(fpWrite, "BBUIP:%s\nCellIdx:%d\nNumTx:%d\nNumRx:%d\nnChanne:%d\nTxBeamIdx:%d\nRxBeamIdx:%d\nnSlotNum:%d\nnFrameNum:%d\n",
        ptransmissionData->BBU_Idx,
        ptransmissionData->nCellIdx,
        ptransmissionData->NumTx,
        ptransmissionData->NumRx,
        ptransmissionData->nChannelNum,
        ptransmissionData->TransmitBeamIdx,
        ptransmissionData->ReceiveBeamIdx,
        ptransmissionData->nSlotNum,
        ptransmissionData->nRxFrameNum);

    fclose(fpWrite);
    return 0;
}

int writeDataToBin(const TransmissionDataHdr* ptransmissionData, int conti) {
    char filename[256];  // ���󻺳����԰�ȫ�����ļ���
    snprintf(filename, sizeof(filename), "data/dataContent_%05d_%03d_%03d_%04d_%03d.bin",
        conti, ptransmissionData->TransmitBeamIdx, ptransmissionData->ReceiveBeamIdx, ptransmissionData->nRxFrameNum, ptransmissionData->nSlotNum);

    FILE* fpWrite1 = fopen(filename, "wb");
    if (fpWrite1 == NULL) {
        perror("fopen error");
        return -1;
        exit(EXIT_FAILURE);
    }
    fwrite(ptransmissionData->CRSData, sizeof(int16_t), 1584 * 2 * ANT_LENGTH, fpWrite1);
    fclose(fpWrite1);
    return 0;
}



int main(int argc, char* argv[])
{
    // ---------------------�����в�������---------------------
    int max_write_num = -1;
    int cmd_beam_tx = -1;
    int cmd_beam_rx = -1;
    int cmd_write_txt = -1;
    int cmd_write_mod = 1;
    if (argc >= 2) {
        max_write_num = atoi(argv[1]);
    }
    if (argc >= 3) {
        cmd_write_mod = atoi(argv[2]);
    }
    if (argc >= 4) {
        cmd_beam_tx = atoi(argv[3]);
    }
    if (argc >= 5) {
        cmd_beam_rx = atoi(argv[4]);
    }
    printf("max_write_num:%d cmd_write_mod:%d cmd_beam_tx:%d cmd_beam_rx:%d\n",
        max_write_num, cmd_write_mod, cmd_beam_tx, cmd_beam_rx);
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return -1;
    }

    // ���ڿ��԰�ȫ��ʹ��Winsock API��


    // ---------------------������socket׼��---------------------
    // 1 ʹ��socket()������ȡһ��socket�ļ�������
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);
    // 2 �󶨱��ص������Ϣ
    struct sockaddr_in saddr, caddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(5001);
    int res = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
    assert(res != -1);
    int len = sizeof(caddr);

    // ---------------------��ʼ��---------------------
    TransmissionDataHdr transmissionData;
    PTransmissionDataHdr ptransmissionData;
    ptransmissionData = &transmissionData;
    char buff[sizeof(transmissionData)] = { 0 };
    printf("size of data package: %d Bytes\n", sizeof(TransmissionDataHdr));
    fflush(stdout);


    int contiRecv = 0;
    int contiWrite = 0;
    bool beamFlag;
    bool modFlag;
    while (1)
    {
        if (contiWrite > max_write_num) {
            break;
        }

        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&caddr, &len);
        memcpy(ptransmissionData, buff, sizeof(transmissionData));

        beamFlag = beamselectFlag(cmd_beam_tx, cmd_beam_rx, ptransmissionData);
        modFlag = (contiRecv % cmd_write_mod == 0);

        if (beamFlag && modFlag) {
            printf("============================================================\n");
            printf("BBUIP:%s\nCellIdx:%d\nNumTx:%d\nNumRx:%d\nnChanne:%d\nTxBeamIdx:%d\nRxBeamIdx:%d\nnSlotNum:%d\nnFrameNum:%d\n",
                ptransmissionData->BBU_Idx,
                ptransmissionData->nCellIdx,
                ptransmissionData->NumTx,
                ptransmissionData->NumRx,
                ptransmissionData->nChannelNum,
                ptransmissionData->TransmitBeamIdx,
                ptransmissionData->ReceiveBeamIdx,
                ptransmissionData->nSlotNum,
                ptransmissionData->nRxFrameNum);
            // txtд�룬�жϣ�һ�㲻��Ҫд��
            if (cmd_write_txt == 1) {
                writeHeaderToTxt(ptransmissionData, contiWrite);
            }
            // bin�ļ�д��
            writeDataToBin(ptransmissionData, contiWrite);

            printf("contiWrite:%d contiRecv:%d\n", contiWrite, contiRecv);
            fflush(stdout);

            contiWrite++;
        }
        contiRecv++;
    }    // �ڳ������ǰ��ȷ������ WSACleanup ���ͷ�Winsock��ռ�õ���Դ
    WSACleanup();
    // �ر�socket
    close(sockfd);
}
