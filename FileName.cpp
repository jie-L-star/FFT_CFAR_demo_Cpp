#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <complex>
#include <cmath>
#include <vector>
#include <fstream>
#include <fftw3.h>
#include <FFT_2D.h>


int main() {
    
    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/";

    // Ƶ��(��ʱ��)* ʱ��(��ʱ��)* ͨ��(8��)
    //const int dim1 = 8;     // ��һά��С ͨ��(8��)
    //const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    //const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    const int dim1 = 4;     // ��һά��С ͨ��(8��)
    const int dim2 = 3;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 2;   // ����ά��С Ƶ��(��ʱ��)

    int size;
    size=dim1*dim2*dim3;

    //Complex threeDArray[dim1][dim2][dim3];    //��̬��ջ�ᱬըstd::vector<std::vector<std::vector<std::complex<double>>>> data(CHANNELS,
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1,std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    //std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    // ��CSV�ļ�
    std::ifstream file(path+"Y.csv");

    if (!file.is_open()) {
        std::cerr << "�޷����ļ�" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::stringstream ss(line);
    std::string cell;
    int op = 0;
    int i = 0, j = 0, k = 0;
    while (std::getline(ss, cell, ',')) {
        float real_part, imag_part;
        char plus_sign, imaginary_unit;
        op++;
        if (op % 100000 == 0)
        {
            std::cout << op << std::endl;
        }

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

    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            for (int k = 0; k < dim3; ++k) {
                std::cout << threeDArray[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    FFT_2D(threeDArray[0]);

    //std::ofstream outputFile("fft_result3.txt");
    //    // ����һάFFT
    //for (int i = 0; i < dim1; ++i) {
    //    for (int j = 0; j < dim2; ++j) {
    //        threeDArray[i][j] = FFT_1D(threeDArray[i][j], false);
    //        // ��FFT������浽�ļ�
    //        for (int k = 0; k < dim3; ++k) {
    //            outputFile << threeDArray[i][j][k];
    //        }
    //        outputFile << std::endl;
    //    }
    //    outputFile << std::endl;
    //}

    //outputFile.close();

    return 0;
}

 

 