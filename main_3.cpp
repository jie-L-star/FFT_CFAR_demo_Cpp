#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <load_bin_data.h>
#include <matlab_function.h>
#include <CFAR.h>
#include <time.h>
#include <receivers.h>

//数据包长度 DATA_LENGTH     数据包大小 1584 * 2 * ANT_LENGTH
//修改端口与sender.c一致


int main() {

    //test();

    clock_t start_time = clock();

    //对应matlab crs_ce_slot_rx_tx_013.mat
    //std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnts, std::vector<std::vector<Complex>>(LengthFiles));
    //load_bin_data(crs_rx_Iq_slot_ant);

    //数据预处理
    pre_load_struct pre_load_data;
    pre_load_data.nSlot = DATA_LENGTH;  //同步数据包长度

    load_data(2, 792, 792, "D:/干活/MATLAB-C++/FFT_CFAR_demo/datareal/W_combmax8.csv", pre_load_data.W_combmax8);
    Pre_load(pre_load_data.root_seq_slot_temp, pre_load_data.crs_temp);
    
    std::vector<std::vector<Complex>> data;
    std::vector<std::vector<Complex>> temp;
    CFAR_result my_result;

    //std::vector<std::vector<float>> data_abs(velocity_index_size, std::vector<float>(range_index_size));
    std::vector<std::vector<float>> data_abs(pre_load_data.nSlot, std::vector<float>(792));

    std::cout << "预加载耗时: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    while (1) {
        start_time = clock();
        
        // ---------------------bin文件数据包接收---------------------
        int16_t data_bin[DATA_LENGTH * 1584 * 2 * ANT_LENGTH];
        receive_data(data_bin);
        std::vector<Complex> data_complex = func_bin2complex(data_bin, sizeof(data_bin));

        //data_complex中存入的是两个通道，此处将通道1存入data_tune中
        std::vector<std::vector<Complex>> data_tune(DATA_LENGTH);
        for (int i = 0; i < DATA_LENGTH; i++)
        {
            data_tune[i].insert(data_tune[i].begin(), data_complex.begin() + 2 * i * nSC, data_complex.begin() + (2 * i + 1) * nSC);
        }

        std::cout << "end_Pre_Load: " << (clock() - start_time) << "ms" << std::endl;

        // ---------------------数据预处理---------------------
        data = Pre_process(data_tune, pre_load_data);

        std::cout << "end_Pre_process: " << (clock() - start_time) << "ms" << std::endl;

        // ---------------------数据处理---------------------
        for (int j = 0; j < pre_load_data.nSlot; ++j) {
            FFT_1D(data[j]);
            for (int k = 0; k < nSC/2; ++k)
                data[j][k] = std::conj(data[j][k]);
        }

        FFT_2D(data);

        for (int i = 0; i < data.size(); i++)
            for (int j = 0; j < data[0].size(); j++)
                data_abs[i][j] = pow(std::abs(data[i][j]), 2);

        my_result = CFAR(data_abs);
        std::cout << "end: " << (clock() - start_time) << "ms" << std::endl;
    }

    return 0;
}
