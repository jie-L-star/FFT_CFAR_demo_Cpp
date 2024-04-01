#include <load_bin_data.h>
#include <string.h>
#include <filesystem> // C++17中的文件系统库
#include <Windows.h>
#include <load_data.h>
#include <matlab_function.h>
#include <complex.h>
#include <FFT_2D.h>

const std::wstring L_PATH_DIR_GNB_95 = L"D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::wstring L_PATH_DIR_GNB_96 = L"D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";
const std::string PATH_DIR_GNB_95 = "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::string PATH_DIR_GNB_96 = "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";

std::vector<int> sc_idx = maohao<int>(-(nSC / 2), (nSC / 2) - 1, 1);

void load_bin_data(std::vector<std::vector<std::vector<Complex>>>& crs_rx_Iq_slot_ant) {

    std::vector<Complex> socketdata(LengthFiles);

    std::vector<std::string> file_Names_1 = listFilesInDirectory(L_PATH_DIR_GNB_95, PATH_DIR_GNB_95);
    std::vector<std::string> file_Names_2 = listFilesInDirectory(L_PATH_DIR_GNB_96, PATH_DIR_GNB_96);

    for (int islot = 0; islot < LengthFiles; ++islot) {
        socketdata = func_readbin(file_Names_1[islot]);
        crs_rx_Iq_slot_ant[0][islot].insert(crs_rx_Iq_slot_ant[0][islot].begin(), socketdata.begin(), socketdata.begin() + nSC);
        crs_rx_Iq_slot_ant[1][islot].insert(crs_rx_Iq_slot_ant[1][islot].begin(), socketdata.begin() + nSC, socketdata.end());

        socketdata = func_readbin(file_Names_2[islot]);
        crs_rx_Iq_slot_ant[2][islot].insert(crs_rx_Iq_slot_ant[2][islot].begin(), socketdata.begin(), socketdata.begin() + nSC);
        crs_rx_Iq_slot_ant[3][islot].insert(crs_rx_Iq_slot_ant[3][islot].begin(), socketdata.begin() + nSC, socketdata.begin() + nSC * 2);
        crs_rx_Iq_slot_ant[4][islot].insert(crs_rx_Iq_slot_ant[4][islot].begin(), socketdata.begin() + 2 * nSC, socketdata.begin() + nSC * 3);
        crs_rx_Iq_slot_ant[5][islot].insert(crs_rx_Iq_slot_ant[5][islot].begin(), socketdata.begin() + 3 * nSC, socketdata.begin() + nSC * 4);
        crs_rx_Iq_slot_ant[6][islot].insert(crs_rx_Iq_slot_ant[6][islot].begin(), socketdata.begin() + 4 * nSC, socketdata.begin() + nSC * 5);
        crs_rx_Iq_slot_ant[7][islot].insert(crs_rx_Iq_slot_ant[7][islot].begin(), socketdata.begin() + 5 * nSC, socketdata.begin() + nSC * 6);
    }

    socketdata.clear();

}

void Pre_load(std::vector<std::vector<Complex>> &root_seq_slot_temp, std::vector<std::vector<Complex>> &crs_temp) {
    std::vector<Complex> root_seq_slot = SrsSeqGen(Mzc, 0, 0);
    for (auto& val : root_seq_slot) {
        val = conj(val);
    }

    for (int iTx = 0; iTx < nTxAnts; ++iTx) {
        for (int i = 0; i < nSC / 2; ++i) {
            root_seq_slot_temp[iTx][i] = root_seq_slot[i] * Complex(std::polar(1.0, 2.0 * M_PI * iTx * nCsStep * i / CombMax));
        }
    }

    for (int i = 0; i < nSC / 2; ++i) {
        crs_temp[0][i] = std::polar(1.0, 2 * M_PI * 0.4 * tau_max * SCS * sc_idx[2 * i]);
        crs_temp[1][i] = std::polar(1.0, -2 * M_PI * 0.4 * tau_max * SCS * sc_idx[2 * i]);
    }
}

std::vector<std::vector<Complex>> Pre_process(std::vector<std::vector<Complex>>&crs_rx_Iq_slot_ant, pre_load_struct& pre_load_data) {

    std::vector<std::vector<Complex>> crs_mmse_ce_slot_ant(pre_load_data.nSlot, std::vector<Complex>(nSC / 2));

    for (int iRx = 0; iRx < nRxAnts; ++iRx) {
        for (int iTx = 0; iTx < nTxAnts; ++iTx) {
            for (int i = 0; i < pre_load_data.nSlot; ++i) {
                for (int j = 0; j < nSC / 2; ++j) {
                    crs_mmse_ce_slot_ant[i][j] = crs_rx_Iq_slot_ant[i][2 * j] * pre_load_data.crs_temp[0][j] / pre_load_data.root_seq_slot_temp[iTx][j];
                }
            }
            //multiplyVectors_eigen(pre_load_data.W_combmax8[0], crs_mmse_ce_slot_ant);
            multiplyVectors_Armadillo(pre_load_data.W_combmax8[0], crs_mmse_ce_slot_ant);

            for (int i = 0; i < pre_load_data.nSlot; ++i) {
                for (int j = 0; j < nSC / 2; ++j) {
                    //crs_mmse_ce_slot_ant[i][j] = crs_mmse_ce_slot_ant[i][j] * pre_load_data.crs_temp[1][j];
                    crs_mmse_ce_slot_ant[i][j] *= pre_load_data.crs_temp[1][j];
                }
                IFFT_1D(crs_mmse_ce_slot_ant[i]);
                std::rotate(crs_mmse_ce_slot_ant[i].begin(), crs_mmse_ce_slot_ant[i].end() - 10, crs_mmse_ce_slot_ant[i].end());
            }

            return crs_mmse_ce_slot_ant;
        }
    }
}


std::vector<Complex> func_readbin(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return {}; // 返回空向量
    }

    // 获取文件长度
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 读取数据
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();

    // 解析数据
    std::vector<Complex> readOut;
    readOut.reserve(fileSize / sizeof(short) / 2); // 每个复数由两个 short 组成

    short* dataPtr = reinterpret_cast<short*>(buffer.data());
    for (size_t i = 0; i < fileSize / sizeof(short); i += 2) {
        readOut.emplace_back(dataPtr[i], -dataPtr[i + 1]);  //共轭在此实现
    }

    return readOut;
}


std::vector<std::string> listFilesInDirectory(const std::wstring& folderPath, const std::string& my_folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::vector<std::string> file_names;

    std::wstring searchPath = folderPath + L"\\*";

    hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 忽略文件夹
            continue;
        }
        //wcout << "文件名：" << findFileData.cFileName << endl;
        file_names.push_back(my_folderPath + wstring2String(findFileData.cFileName));
    }while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return file_names;
}


std::string wstring2String(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        return ""; // 转换失败
    }

    std::string str(bufferSize, '\0'); // 将 '\0' 替换为 0，以免出现警告
    if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr) == 0) {
        return ""; // 转换失败
    }

    return str;
}

std::vector<Complex> SrsSeqGen(int M_zc, int u, int v) {
    int N_zc = 0;
    for (int i = 2; i <= M_zc; ++i) {
        bool isPrime = true;
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            N_zc = i;
        }
    }

    double q_1 = 1.0 * N_zc * (u + 1) / 31;
    int q = floor(q_1 + 0.5) + v * pow(-1, static_cast<int>(2 * q_1));
    std::vector<Complex> x_q(N_zc);

    for (int m = 0; m < N_zc; ++m) {
        double angle = -1 * M_PI * q * m * (m + 1) / N_zc;
        x_q[m] = std::polar(1.0, angle);
    }

    std::vector<Complex> r_u_v(M_zc);
    for (int n = 0; n < M_zc; ++n) {
        if (n == 786)
            n = 786;
        r_u_v[n] = x_q[(n % N_zc)];
    }

    return r_u_v;
}
