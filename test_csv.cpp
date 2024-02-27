#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <complex>
#include <vector>

// �򻯵ĸ������壨���ڱ�׼�⣩
using Complex = std::complex<float>;

int main() {
    // Ƶ��(��ʱ��)* ʱ��(��ʱ��)* ͨ��(8��)
    const int dim1 = 8;     // ��һά��С ͨ��(8��)
    const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    //Complex threeDArray[dim1][dim2][dim3];    //��̬��ջ�ᱬը
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    // ��CSV�ļ�
    std::ifstream file("D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14.csv");
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::stringstream ss(line);
    std::string cell;

    int i = 0, j = 0, k = 0;
    while (std::getline(ss, cell, ',')) {
        float real_part, imag_part;
        char plus_sign, imaginary_unit;

        std::stringstream cell_ss(cell);
        cell_ss >> real_part >> plus_sign >> imag_part >> imaginary_unit;

        if (plus_sign == '+' && imaginary_unit == 'i') {
            threeDArray[i][j][k] = Complex(real_part, imag_part);
        }
        else if (plus_sign == '-' && imaginary_unit == 'i')
        {
            threeDArray[i][j][k] = Complex(real_part, -imag_part);
        }
        else {
            std::cerr << "Error: Invalid complex number format in CSV file" << std::endl;
            return 1;
        }

        k++;
        if (k == dim3) {
            k = 0;
            j++;
            if (j == dim2) {
                j = 0;
                i++;
            }
        }
    }

    //while (1) {
    //    std::cin >> i >> j >> k;
    //    std::cout << threeDArray[i-1][j-1][k-1] << std::endl;
    //}

    // �����ȡ������
    //for (int i = 0; i < dim1; ++i) {
    //    for (int j = 0; j < dim2; ++j) {
    //        for (int k = 0; k < dim3; ++k) {
    //            std::cout << threeDArray[i][j][k] << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //    std::cout << std::endl;
    //}

    return 0;
}