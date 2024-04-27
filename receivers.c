#include <receivers.h>   

typedef int my_bool;
int sockfd, len;
struct sockaddr_in saddr, caddr;
PTransmissionDataHdr ptransmissionData;
TransmissionDataHdr transmissionData;
char buff[sizeof(transmissionData)] = { 0 };

int receive_init(void)
{
    int argc = 0;
    char* argv = "";
    // ---------------------�����в�������---------------------
    //int max_write_num = -1;
    //int cmd_beam_tx = -1;
    //int cmd_beam_rx = -1;
    //int cmd_write_txt = -1;
    //int cmd_write_mod = 1;
    //if (argc >= 2) {
    //    max_write_num = atoi(argv[1]);
    //}
    //if (argc >= 3) {
    //    cmd_write_mod = atoi(argv[2]);
    //}
    //if (argc >= 4) {
    //    cmd_beam_tx = atoi(argv[3]);
    //}
    //if (argc >= 5) {
    //    cmd_beam_rx = atoi(argv[4]);
    //}
    //printf("max_write_num:%d cmd_write_mod:%d cmd_beam_tx:%d cmd_beam_rx:%d\n",
    //    max_write_num, cmd_write_mod, cmd_beam_tx, cmd_beam_rx);
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return -1;
    }

    // ---------------------������socket׼��---------------------
    // 1 ʹ��socket()������ȡһ��socket�ļ�������
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);

    // 2 �󶨱��ص������Ϣ
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(12345);              //�˿ں�
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);  //��������IP
    //saddr.sin_addr.s_addr = htonl("169.254.232.2");  //��������IP
    int res = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
    assert(res != -1);

    len = sizeof(caddr);
    ptransmissionData = &transmissionData;
    fflush(stdout);
}

int receive_data(int16_t receive_data_package[])
{
    for(int i=0; i< FILE_LENGTH; i++)
    {
        //if (contiWrite > max_write_num) {
        //    break;
        //}

        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&caddr, &len);
        memcpy(ptransmissionData, buff, sizeof(transmissionData));

        /*
        int contiRecv = 0;
        int contiWrite = 0;
        my_bool beamFlag = beamselectFlag(cmd_beam_tx, cmd_beam_rx, ptransmissionData);
        my_bool modFlag = (contiRecv % cmd_write_mod == 0);

        if (beamFlag && modFlag) {
            //printf("============================================================\n");
            //printf("BBUIP:%s\nCellIdx:%d\nNumTx:%d\nNumRx:%d\nnChanne:%d\nTxBeamIdx:%d\nRxBeamIdx:%d\nnSlotNum:%d\nnFrameNum:%d\n",
            //    ptransmissionData->BBU_Idx,
            //    ptransmissionData->nCellIdx,
            //    ptransmissionData->NumTx,
            //    ptransmissionData->NumRx,
            //    ptransmissionData->nChannelNum,
            //    ptransmissionData->TransmitBeamIdx,
            //    ptransmissionData->ReceiveBeamIdx,
            //    ptransmissionData->nSlotNum,
            //    ptransmissionData->nRxFrameNum);
            
            //// txtд�룬�жϣ�һ�㲻��Ҫд��
            //if (cmd_write_txt == 1) {
            //    writeHeaderToTxt(ptransmissionData, contiWrite);
            //}
            //// bin�ļ�д��
            //writeDataToBin(ptransmissionData, contiWrite);

            //printf("contiWrite:%d contiRecv:%d\n", contiWrite, contiRecv);
            //fflush(stdout);

            //printf("���յ�%d�����ݰ�   nnSlotNum=%d\n", i+1, ptransmissionData->nSlotNum);

            contiWrite++;
        }
        contiRecv++;
        */

        memcpy(&receive_data_package[i * DATA_LENGTH * ANT_LENGTH], ptransmissionData->CRSData, sizeof(ptransmissionData->CRSData));
    }
    return 0;
}


void receive_stop()
{
    WSACleanup();
    closesocket(sockfd);
}

my_bool beamselectFlag(int cmd_beam_tx, int cmd_beam_rx, const TransmissionDataHdr* pData) {
    return (cmd_beam_tx == -1 || pData->TransmitBeamIdx == cmd_beam_tx) &&
        (cmd_beam_rx == -1 || pData->ReceiveBeamIdx == cmd_beam_rx);
}

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

/**
//int main(int argc, char* argv[])
//{
//    // ---------------------�����в�������---------------------
//    int max_write_num = -1;
//    int cmd_beam_tx = -1;
//    int cmd_beam_rx = -1;
//    int cmd_write_txt = -1;
//    int cmd_write_mod = 1;
//    if (argc >= 2) {
//        max_write_num = atoi(argv[1]);
//    }
//    if (argc >= 3) {
//        cmd_write_mod = atoi(argv[2]);
//    }
//    if (argc >= 4) {
//        cmd_beam_tx = atoi(argv[3]);
//    }
//    if (argc >= 5) {
//        cmd_beam_rx = atoi(argv[4]);
//    }
//    printf("max_write_num:%d cmd_write_mod:%d cmd_beam_tx:%d cmd_beam_rx:%d\n",
//        max_write_num, cmd_write_mod, cmd_beam_tx, cmd_beam_rx);
//    WSADATA wsaData;
//    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (result != 0) {
//        printf("WSAStartup failed with error: %d\n", result);
//        return -1;
//    }
//
//    // ���ڿ��԰�ȫ��ʹ��Winsock API��
//
//
//    // ---------------------������socket׼��---------------------
//    // 1 ʹ��socket()������ȡһ��socket�ļ�������
//    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//    assert(sockfd != -1);
//    // 2 �󶨱��ص������Ϣ
//    struct sockaddr_in saddr, caddr;
//    memset(&saddr, 0, sizeof(saddr));
//    saddr.sin_family = AF_INET;
//    saddr.sin_port = htons(12345);
//    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    int res = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
//
//    //if (res == -1) {
//    //    printf("Bind failed. Error code: %d\n", WSAGetLastError());
//    //    char errorMessage[256];
//    //    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//    //        NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//    //        errorMessage, sizeof(errorMessage), NULL);
//    //    printf("Error message: %s\n", errorMessage);
//    //    return -1; // ��ʧ�ܣ������˳�
//    //}
//
//    assert(res != -1);
//    int len = sizeof(caddr);
//
//    // ---------------------��ʼ��---------------------
//    TransmissionDataHdr transmissionData;
//    PTransmissionDataHdr ptransmissionData;
//    ptransmissionData = &transmissionData;
//    char buff[sizeof(transmissionData)] = { 0 };
//    printf("size of data package: %d Bytes\n", sizeof(TransmissionDataHdr));
//    fflush(stdout);
//
//
//    int contiRecv = 0;
//    int contiWrite = 0;
//    bool beamFlag;
//    bool modFlag;
//    while (1)
//    {
//        //if (contiWrite > max_write_num) {
//        //    break;
//        //}
//
//        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&caddr, &len);
//        memcpy(ptransmissionData, buff, sizeof(transmissionData));
//
//        beamFlag = beamselectFlag(cmd_beam_tx, cmd_beam_rx, ptransmissionData);
//        modFlag = (contiRecv % cmd_write_mod == 0);
//
//        if (beamFlag && modFlag) {
//            printf("============================================================\n");
//            printf("BBUIP:%s\nCellIdx:%d\nNumTx:%d\nNumRx:%d\nnChanne:%d\nTxBeamIdx:%d\nRxBeamIdx:%d\nnSlotNum:%d\nnFrameNum:%d\n",
//                ptransmissionData->BBU_Idx,
//                ptransmissionData->nCellIdx,
//                ptransmissionData->NumTx,
//                ptransmissionData->NumRx,
//                ptransmissionData->nChannelNum,
//                ptransmissionData->TransmitBeamIdx,
//                ptransmissionData->ReceiveBeamIdx,
//                ptransmissionData->nSlotNum,
//                ptransmissionData->nRxFrameNum);
//            // txtд�룬�жϣ�һ�㲻��Ҫд��
//            if (cmd_write_txt == 1) {
//                writeHeaderToTxt(ptransmissionData, contiWrite);
//            }
//            // bin�ļ�д��
//            writeDataToBin(ptransmissionData, contiWrite);
//
//            printf("contiWrite:%d contiRecv:%d\n", contiWrite, contiRecv);
//            fflush(stdout);
//
//            contiWrite++;
//        }
//        contiRecv++;
//    }    // �ڳ������ǰ��ȷ������ WSACleanup ���ͷ�Winsock��ռ�õ���Դ
//    WSACleanup();
//    // �ر�socket
//    close(sockfd);
//}
**/