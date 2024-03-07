#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <complex>
#include <boost/algorithm/string.hpp> // 用于字符串分割


// 定义复数类型
using Complex = std::complex<float>;

std::vector<std::vector<std::vector<Complex>>> boost_load_data(int dim1, int dim2, int dim3, const std::string& path) {
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    // 打开CSV文件
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return {};  // 返回空容器
    }

    // 使用缓冲区
    constexpr std::streamsize buffer_size = 60*1024*1024; // 可以根据需要调整缓冲区大小
    char buffer[buffer_size];

    std::string line;
    int i = 0, j = 0, k = 0;
    while (file.getline(buffer, buffer_size)) {
        line.assign(buffer, file.gcount());
        std::vector<std::string> tokens;
        boost::algorithm::split(tokens, line, boost::is_any_of(","));

        for (const std::string& token : tokens) {
            float real_part, imag_part;
            char plus_sign, imaginary_unit;

            // 使用 std::stof() 直接转换字符串到浮点数
            size_t pos = token.find('+');
            if (pos == std::string::npos) {
                std::cerr << "错误: CSV文件中的复数格式无效" << std::endl;
                return {};  // 返回空容器
            }
            real_part = std::stof(token.substr(0, pos));
            pos++;
            imag_part = std::stof(token.substr(pos, token.size() - pos - 1));

            threeDArray[i][j][k] = Complex(real_part, imag_part);

            k++;
            if (k == dim3) {
                k = 0;
                j++;
                if (j == dim2) {
                    j = 0;
                    i++;
                    if (i == dim1) {
                        break;
                    }
                }
            }
        }
    }

    return threeDArray;
}