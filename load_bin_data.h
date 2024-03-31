#include <main.h>

const int nRxAnt1 = 2;
const int nRxAnt2 = 6;
const int nTxAnts = 1;
const int Nfft = 2048;
const int nRxAnts = nRxAnt1 + nRxAnt2;
const int nSC = 1584;
//收集数据包个数
const int LengthFiles = 1000;

const float tau_max = 1.2e-6;
const int SCS = 1.2e5;
const int CombMax = 8;
const int nComb = 2;
const int nAntPerRRU = 2;
const int nCsStep = 4;
const int Mzc = 132 * 12 / nComb;
const int cr_win = floor(nSC / nComb / (CombMax / nCsStep));
const int win_len = cr_win / 2;

//预加载数据包
struct pre_load_struct {
	int nSlot;
	std::vector<std::vector<Complex>> root_seq_slot_temp;
	std::vector<std::vector<Complex>> crs_temp;
    std::vector<std::vector<std::vector<Complex>>> W_combmax8;

    pre_load_struct()
        : nSlot(LengthFiles),
        root_seq_slot_temp(nTxAnts, std::vector<Complex>(nSC / 2)),
        crs_temp(2, std::vector<Complex>(nSC / 2)),
        W_combmax8(2, std::vector<std::vector<Complex>>(nSC / 2, std::vector<Complex>(nSC / 2))){}
};


void load_bin_data(std::vector<std::vector<std::vector<Complex>>>& crs_rx_Iq_slot_ant);
std::vector<Complex> func_readbin(const std::string& filepath);
std::vector<std::string> listFilesInDirectory(const std::wstring& folderPath, const std::string& my_folderPath);
std::string wstring2String(const std::wstring& wstr);
std::vector<Complex> SrsSeqGen(int M_zc, int u, int v);

void Pre_load(std::vector<std::vector<Complex>>& root_seq_slot_temp, std::vector<std::vector<Complex>>& crs_temp);
std::vector<std::vector<Complex>> Pre_process(std::vector<std::vector<std::vector<Complex>>>& crs_rx_Iq_slot_ant, pre_load_struct& pre_load_data);

std::vector<std::vector<Complex>> multiplyVectors_eigen_map(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB);
std::vector<std::vector<Complex>> multiplyVectors_eigen(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB);
std::vector<std::vector<Complex>> multiplyVectors_Armadillo(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB);

void test(void);
