#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <load_bin_data.h>
#include <matlab_function.h>
#include <CFAR.h>
#include <time.h>

int main() {

    //test();

    clock_t start_time = clock();

    std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnts, std::vector<std::vector<Complex>>(LengthFiles));
    load_bin_data(crs_rx_Iq_slot_ant);
    //数据预处理
    pre_load_struct pre_load_data;
    load_data(2, 792, 792, "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/W_combmax8.csv", pre_load_data.W_combmax8);
    Pre_load(pre_load_data.root_seq_slot_temp, pre_load_data.crs_temp);
    
    std::vector<std::vector<Complex>> data;
    CFAR_result my_result;
    //std::vector<std::vector<float>> data_abs(velocity_index_size, std::vector<float>(range_index_size));
    std::vector<std::vector<float>> data_abs(1000, std::vector<float>(792));

    std::cout << "预加载耗时: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    while (1) {
        start_time = clock();
        
        //需要将crs_rx_Iq_slot_ant换为数据包，收集 LengthFiles 个便可以进行预处理，在load_bin_data.h中更改
        data = Pre_process(crs_rx_Iq_slot_ant, pre_load_data);

        std::cout << "end_Pre_process: " << (clock() - start_time) << "ms" << std::endl;

        for (int j = 0; j < LengthFiles; ++j) {
            FFT_1D(data[j]);
            for (int k = 0; k < nSC/2; ++k)
                data[j][k] = std::conj(data[j][k]);
        }

        FFT_2D(data);

        for (int i = 0; i < data.size(); i++)
            for (int j = 0; j < data[0].size(); j++)
                data_abs[i][j] = pow(std::abs(data_abs[i][j]), 2);

        my_result = CFAR(data_abs);
        std::cout << "end: " << (clock() - start_time) << "ms" << std::endl;
    }

    return 0;
}
