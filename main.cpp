#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <load_bin_data.h>
#include <matlab_function.h>
#include <CFAR.h>
#include <time.h>

/*
* main.cpp 主函数，单线程读取并处理，结果和MATLAB通道6一致
* main_2.cpp 主函数，尝试双线程，仅读取1*1000*792共计15次，耗时较长，还需要更改
* FileName.cpp 用boost库读取数据，效果不变
* load_bin_data.cpp 读取bin文件
*/

int main() {

    clock_t start_time = clock();

    std::string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";
    const int dim1 = 1;     // 第一维大小 通道(8个)
    const int dim2 = 9600;  // 第二维大小 时域(慢时间)
    const int dim3 = 792;   // 第三维大小 频域(快时间)

    //加载数据
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    load_data(dim1, dim2, dim3, path, threeDArray);

    std::cout << "load_data: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    //一组1000个符号，以100为步进长度
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
