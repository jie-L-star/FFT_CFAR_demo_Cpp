#include <load_bin_data.h>
#include <string.h>
#include <filesystem> // C++17中的文件系统库
#include <Windows.h>


const std::wstring L_PATH_DIR_GNB_95 = L"D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::wstring L_PATH_DIR_GNB_96 = L"D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";
const std::string PATH_DIR_GNB_95 = "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/95/013/";
const std::string PATH_DIR_GNB_96 = "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/96/013/";

const int nRxAnt1 = 2;
const int nRxAnt2 = 6;

std::vector<std::vector<std::vector<Complex>>> load_bin_data(void) {
    const int nRxAnts = nRxAnt1 + nRxAnt2;
    const int nSC = 1584;
    const int LengthFiles = 9600; // 假设有100个文件

    std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnt1, std::vector<std::vector<Complex>>(LengthFiles));
    std::vector<Complex> socketdata(9600);

    std::vector<std::string> file_Names = listFilesInDirectory(L_PATH_DIR_GNB_95);

    for (int islot = 0; islot < LengthFiles; ++islot) {
        socketdata = func_readbin(file_Names[islot]);
        crs_rx_Iq_slot_ant[0][islot].insert(crs_rx_Iq_slot_ant[0][islot].begin(), socketdata.begin(), socketdata.begin() + nSC);
        crs_rx_Iq_slot_ant[1][islot].insert(crs_rx_Iq_slot_ant[1][islot].begin(), socketdata.begin() + nSC, socketdata.end());
    }

    //// 加载96文件夹中的数据
    //for (int iAnt = 0; iAnt < nRxAnt2; ++iAnt) {
    //    std::string filePath = PATH_DIR_GNB_96 + "dataContent_" + std::to_string(iAnt) + "_*.bin"; // 文件名模式
    //    for (int islot = 0; islot < LengthFiles; ++islot) {
    //        std::string path = filePath; // 此处添加找到具体文件的代码
    //        // 调用 func_readbin 函数读取数据并存储到 crs_rx_Iq_slot_ant 中
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
            // 忽略文件夹
            continue;
        }
        //std::wcout << "文件名：" << findFileData.cFileName << std::endl;
        file_names.push_back(PATH_DIR_GNB_95 + wstring2String(findFileData.cFileName));
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