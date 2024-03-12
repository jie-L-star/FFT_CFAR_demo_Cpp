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

    //load_bin_data();

    clock_t start_time = clock();

    //std::string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/data_14.csv";
    //const int dim1 = 8;     // 第一维大小 通道(8个)
    //const int dim2 = 9600;  // 第二维大小 时域(慢时间)
    //const int dim3 = 792;   // 第三维大小 频域(快时间)

    std::string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";
    const int dim1 = 1;     // 第一维大小 通道(8个)
    const int dim2 = 9600;  // 第二维大小 时域(慢时间)
    const int dim3 = 792;   // 第三维大小 频域(快时间)

    //加载数据
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    threeDArray = load_data(dim1, dim2, dim3, path);

    std::cout << "load_data: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    //一组1000个符号，以100为步进长度
    const int step_size = 600;
    const int t_length1 = 1000;
    const int t_length = (dim2 - t_length1) / step_size;  //tlength

    //距离范围为0~50m 速度范围为-4~4m/s
    std::vector<int> range_index = maohao<int>(range_index_min, range_index_max, 1);
    std::vector<int> velocity_index = maohao<int>(velocity_index_min, velocity_index_max, 1);

    std::vector<std::vector<std::vector<Complex>>> Receive(t_length, std::vector<std::vector<Complex>>(t_length1, std::vector<Complex>(dim3)));
    std::vector<std::vector<std::vector<Complex>>> Result_fft(t_length, std::vector<std::vector<Complex>>(velocity_index_size, std::vector<Complex>(range_index_size)));

    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            threeDArray[i][j] = FFT_1D(threeDArray[i][j], false);
            for (int k = 0; k < dim3; ++k)
                threeDArray[i][j][k] = std::conj(threeDArray[i][j][k]);
        }
    }

    // FFT_2D结果
    for (int i = 0; i < t_length; ++i) {
        for (int j = 0; j < t_length1; ++j) {
            for (int k = 0; k < K; ++k) {
                Receive[i][j][k] = threeDArray[0][step_size * i + j][k];
            }
        }
        Result_fft[i] = FFT_2D(Receive[i]);
    }
    std::cout << "FFT_2D: " << (clock() - start_time)/1000.0 << "s" << std::endl;

    CFAR_result my_result[t_length];

    std::vector<std::vector<float>> R_temp(velocity_index_size, std::vector<float>(range_index_size));
    for (int ii = 0; ii < t_length; ++ii) {
        for (size_t i = 0; i < velocity_index_size; i++)
            for (size_t j = 0; j < range_index_size; j++)
                R_temp[i][j] = pow(std::abs(Result_fft[ii][i][j]), 2);

        my_result[ii] = CFAR(R_temp);
    }
    
    std::cout << "CFAR: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
    //绘图用
    //std::vector<float> range_2dfft = linspace<float>(0, (c0 / (2 * delta_f)), (K + 1));
    //std::vector<float> velocity_2dfft = linspace<float>(-lambda / 2 / Ts, lambda / 2 / Ts, 1000 + 1);

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

 