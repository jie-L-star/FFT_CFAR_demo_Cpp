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

    std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnts, std::vector<std::vector<Complex>>(LengthFiles));
    load_bin_data(crs_rx_Iq_slot_ant);
    //数据预处理
    pre_load_struct pre_load_data;
    load_data(2, 792, 792, "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/W_combmax8.csv", pre_load_data.W_combmax8);
    Pre_load(pre_load_data.root_seq_slot_temp, pre_load_data.crs_temp);
    
    std::vector<std::vector<Complex>> data;
    CFAR_result my_result;
    std::vector<std::vector<float>> data_abs(velocity_index_size, std::vector<float>(range_index_size));

    std::cout << "预加载耗时: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    while (1) {
        data = Pre_process(crs_rx_Iq_slot_ant, pre_load_data);

        std::cout << "end_Pre_process: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

        for (int j = 0; j < LengthFiles; ++j) {
            FFT_1D(data[j]);
            for (int k = 0; k < nSC/2; ++k)
                data[j][k] = std::conj(data[j][k]);
        }

        FFT_2D(data);

        for (int i = 0; i < velocity_index_size; i++)
            for (int j = 0; j < range_index_size; j++)
                data_abs[i][j] = pow(std::abs(data_abs[i][j]), 2);

        my_result = CFAR(data_abs);
        std::cout << "end: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
    }

    return 0;
}
