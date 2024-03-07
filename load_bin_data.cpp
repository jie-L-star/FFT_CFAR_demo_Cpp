#include <load_bin_data.h>
#include <string.h>
#include <filesystem> // C++17�е��ļ�ϵͳ��
#include <Windows.h>


const std::wstring L_PATH_DIR_GNB_95 = L"D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::wstring L_PATH_DIR_GNB_96 = L"D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";
const std::string PATH_DIR_GNB_95 = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::string PATH_DIR_GNB_96 = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";

const int nRxAnt1 = 2;
const int nRxAnt2 = 6;

std::vector<std::vector<std::vector<Complex>>> load_bin_data(void) {
    const int nRxAnts = nRxAnt1 + nRxAnt2;
    const int nSC = 1584;
    const int LengthFiles = 9600; // ������100���ļ�

    std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnt1, std::vector<std::vector<Complex>>(LengthFiles));
    std::vector<Complex> socketdata(9600);

    std::vector<std::string> file_Names = listFilesInDirectory(L_PATH_DIR_GNB_95);

    for (int islot = 0; islot < LengthFiles; ++islot) {
        socketdata = func_readbin(file_Names[islot]);
        crs_rx_Iq_slot_ant[0][islot].insert(crs_rx_Iq_slot_ant[0][islot].begin(), socketdata.begin(), socketdata.begin() + nSC);
        crs_rx_Iq_slot_ant[1][islot].insert(crs_rx_Iq_slot_ant[1][islot].begin(), socketdata.begin() + nSC, socketdata.end());
    }

    //// ����96�ļ����е�����
    //for (int iAnt = 0; iAnt < nRxAnt2; ++iAnt) {
    //    std::string filePath = PATH_DIR_GNB_96 + "dataContent_" + std::to_string(iAnt) + "_*.bin"; // �ļ���ģʽ
    //    for (int islot = 0; islot < LengthFiles; ++islot) {
    //        std::string path = filePath; // �˴�����ҵ������ļ��Ĵ���
    //        // ���� func_readbin ������ȡ���ݲ��洢�� crs_rx_Iq_slot_ant ��
    //         socketdata = func_readbin(path);
    //         for (int i = 0; i < nSC; ++i) {
    //             crs_rx_Iq_slot_ant[i][islot][nRxAnt1 + iAnt] = socketdata[i];
    //         }
    //    }
    //}

    return crs_rx_Iq_slot_ant;
}


std::vector<Complex> func_readbin(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filepath << std::endl;
        return {}; // ���ؿ�����
    }

    // ��ȡ�ļ�����
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // ��ȡ����
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();

    // ��������
    std::vector<Complex> readOut;
    readOut.reserve(fileSize / sizeof(short) / 2); // ÿ������������ short ���

    short* dataPtr = reinterpret_cast<short*>(buffer.data());
    for (size_t i = 0; i < fileSize / sizeof(short); i += 2) {
        readOut.emplace_back(dataPtr[i], dataPtr[i + 1]);
    }

    return readOut;
}


std::vector<std::string> listFilesInDirectory(const std::wstring& folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::vector<std::string> file_names;

    std::wstring searchPath = folderPath + L"\\*";
    hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // �����ļ���
            continue;
        }
        //std::wcout << "�ļ�����" << findFileData.cFileName << std::endl;
        file_names.push_back(PATH_DIR_GNB_95 + wstring2String(findFileData.cFileName));
    }while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return file_names;
}


std::string wstring2String(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        return ""; // ת��ʧ��
    }

    std::string str(bufferSize, '\0'); // �� '\0' �滻Ϊ 0��������־���
    if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr) == 0) {
        return ""; // ת��ʧ��
    }

    return str;
}