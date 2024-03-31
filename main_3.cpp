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
    //����Ԥ����
    pre_load_struct pre_load_data;
    load_data(2, 792, 792, "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/datareal/W_combmax8.csv", pre_load_data.W_combmax8);
    Pre_load(pre_load_data.root_seq_slot_temp, pre_load_data.crs_temp);
    
    std::vector<std::vector<Complex>> data;
    CFAR_result my_result;
    //std::vector<std::vector<float>> data_abs(velocity_index_size, std::vector<float>(range_index_size));
    std::vector<std::vector<float>> data_abs(1000, std::vector<float>(792));

    std::cout << "Ԥ���غ�ʱ: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    while (1) {
        start_time = clock();
        
        //��Ҫ��crs_rx_Iq_slot_ant��Ϊ���ݰ����ռ� LengthFiles ������Խ���Ԥ������load_bin_data.h�и���
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
