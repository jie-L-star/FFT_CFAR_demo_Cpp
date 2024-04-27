#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <assert.h>
#include <ws2tcpip.h> // �ṩ�׽��ֵ�ַ�ṹ�ͺ���ԭ�ͣ���inet_pton
#include <iphlpapi.h>
#include <windows.h>// �ṩ��ȡ������������Ϣ�ĺ���ԭ�ͣ���GetAdaptersAddresses
#include<stdint.h>
#include <sys/types.h>   

#define DATA_LENGTH 1584 * 2
#define ANT_LENGTH 8
#define MODSAMV 8

#define true 1
#define false 0

#define FILE_LENGTH 200

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

typedef int my_bool;

my_bool beamselectFlag(int cmd_beam_tx, int cmd_beam_rx, const TransmissionDataHdr* pData);
int writeHeaderToTxt(const TransmissionDataHdr* ptransmissionData, int conti);
int writeDataToBin(const TransmissionDataHdr* ptransmissionData, int conti);

#ifdef __cplusplus
    extern "C" {
#endif
        int receive_init(void);
        int receive_data(int16_t receive_data_package[]);
        void receive_stop();

#ifdef __cplusplus
    }
#endif