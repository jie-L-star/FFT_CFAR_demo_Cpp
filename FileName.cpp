#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <complex>
#include <boost/algorithm/string.hpp> // �����ַ����ָ�


// ���帴������
using Complex = std::complex<float>;

std::vector<std::vector<std::vector<Complex>>> boost_load_data(int dim1, int dim2, int dim3, const std::string& path) {
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    // ��CSV�ļ�
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�" << std::endl;
        return {};  // ���ؿ�����
    }

    // ʹ�û�����
    constexpr std::streamsize buffer_size = 60*1024*1024; // ���Ը�����Ҫ������������С
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

            // ʹ�� std::stof() ֱ��ת���ַ�����������
            size_t pos = token.find('+');
            if (pos == std::string::npos) {
                std::cerr << "����: CSV�ļ��еĸ�����ʽ��Ч" << std::endl;
                return {};  // ���ؿ�����
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