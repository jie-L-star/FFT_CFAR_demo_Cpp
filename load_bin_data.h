#include <main.h>

std::vector<std::vector<std::vector<Complex>>> load_bin_data(void);
std::vector<Complex> func_readbin(const std::string& filepath);
std::vector<std::string> listFilesInDirectory(const std::wstring& folderPath, const std::string& my_folderPath);
std::string wstring2String(const std::wstring& wstr);
std::vector<Complex> SrsSeqGen(int M_zc, int u, int v);
void ChannelEstimation(std::vector<std::vector<std::vector<Complex>>>& crs_rx_Iq_slot_ant, std::vector<std::vector<std::vector<Complex>>>& W_combmax8);

std::vector<std::vector<Complex>> multiplyVectors_eigen(const std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB);
std::vector<std::vector<Complex>> multiplyVectors(const std::vector<std::vector<Complex>>& matrixA, const std::vector<std::vector<Complex>>& matrixB);
