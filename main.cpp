#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <load_bin_data.h>
#include <matlab_function.h>
#include <CFAR.h>
#include <time.h>

/*
* main.cpp �����������̶߳�ȡ�����������MATLABͨ��6һ��
* main_2.cpp ������������˫�̣߳�����ȡ1*1000*792����15�Σ���ʱ�ϳ�������Ҫ����
* FileName.cpp ��boost���ȡ���ݣ�Ч������
* load_bin_data.cpp ��ȡbin�ļ�
*/

int main() {

    clock_t start_time = clock();

    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";
    const int dim1 = 1;     // ��һά��С ͨ��(8��)
    const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    //��������
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    load_data(dim1, dim2, dim3, path, threeDArray);

    std::cout << "load_data: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    //һ��1000�����ţ���100Ϊ��������
    const int step_size = 100;
    const int t_length1 = 1000;
    const int t_length = (dim2 - t_length1) / step_size;  //tlength

    std::vector<std::vector<Complex>> Receive;
    std::vector<std::vector<std::vector<Complex>>> Result_fft(t_length, std::vector<std::vector<Complex>>(velocity_index_size, std::vector<Complex>(range_index_size)));

    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            FFT_1D(threeDArray[i][j]);
            for (int k = 0; k < dim3; ++k)
                threeDArray[i][j][k] = std::conj(threeDArray[i][j][k]);
        }
    }

    CFAR_result my_result[t_length];
    std::vector<std::vector<float>> R_temp(velocity_index_size, std::vector<float>(range_index_size));

    for (int ii = 0; ii < t_length; ++ii) {
        Receive.assign(threeDArray[0].begin()+ step_size * ii, threeDArray[0].begin() + step_size * ii + t_length1);
        FFT_2D(Receive);

        for (int i = 0; i < velocity_index_size; i++)
            for (int j = 0; j < range_index_size; j++)
                R_temp[i][j] = pow(std::abs(Receive[i][j]), 2);

        my_result[ii] = CFAR(R_temp);
    }

    std::cout << "CFAR: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
    std::ofstream outputFile("CFAR_results.txt");

    for (int i = 0; i < t_length; ++i) {
        for (int j = 0; j < my_result[i].Amplititude.size(); ++j) {
            outputFile << my_result[i].Amplititude[j] << "  ";
        }

        for (int j = 0; j < my_result[i].SWCfarResultNJ_c[0].size(); ++j) {
            outputFile << "(" << my_result[i].SWCfarResultNJ_c[0][j] << ", " << my_result[i].SWCfarResultNJ_c[1][j] << ") ";
        }
        outputFile << std::endl;
    }

    outputFile.close();

    std::cout << "end: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    return 0;
}
