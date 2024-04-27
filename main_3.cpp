#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <load_bin_data.h>
#include <matlab_function.h>
#include <CFAR.h>
#include <time.h>
#include <receivers.h>
#include <my_plot.h>


//数据包长度 FILE_LENGTH     数据包大小 1584 * 2 * ANT_LENGTH
//修改端口与sender.c一致

int main() {

    clock_t start_time = clock();

    //对应matlab crs_ce_slot_rx_tx_013.mat
    //std::vector<std::vector<std::vector<Complex>>> crs_rx_Iq_slot_ant(nRxAnts, std::vector<std::vector<Complex>>(LengthFiles));
    //load_bin_data(crs_rx_Iq_slot_ant);

    //数据预处理
    pre_load_struct pre_load_data;
    pre_load_data.nSlot = FILE_LENGTH;  //同步数据包长度

    //距离范围
    std::vector<double> range_2dfft(range_index_max);
    std::iota(range_2dfft.begin(), range_2dfft.end(), 0);
    double step_r = (c0 / (2 * delta_f)) / (double)(K / 1);
    for (auto& val : range_2dfft) {
        val = val * step_r;
    }
    //range_2dfft_1.push_back(range_2dfft[range_index_max]);
    //速度范围
    std::vector<double> velocity_2dfft(FILE_LENGTH);
    std::iota(velocity_2dfft.begin(), velocity_2dfft.end(), -FILE_LENGTH / 2);
    double step_v = ((lambda / (2 * Ts)) - (-lambda / (2 * Ts))) / (double)(FILE_LENGTH);
    for (auto& val : velocity_2dfft) {
        val = val * step_v;
    }

    load_data(2, 792, 792, "D:/FFT_CFAR_demo_Cpp/datareal/W_combmax8.csv", pre_load_data.W_combmax8);
    Pre_load(pre_load_data.root_seq_slot_temp, pre_load_data.crs_temp);
    
    std::vector<std::vector<Complex>> data;
    std::vector<std::vector<Complex>> temp;
    CFAR_result my_result;

    //std::vector<std::vector<float>> data_abs(velocity_index_size, std::vector<float>(range_index_size));
    std::vector<std::vector<float>> data_abs(pre_load_data.nSlot, std::vector<float>(65));

    std::cout << "预加载耗时: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    int16_t data_bin[FILE_LENGTH * DATA_LENGTH * ANT_LENGTH];
    //std::vector<Complex> data_complex;
    std::vector<std::vector<std::vector<Complex>>> data_tune(ANT_LENGTH, std::vector<std::vector<Complex>>(FILE_LENGTH, std::vector<Complex>(nSC)));

    uint16_t order = 0;

    receive_init();

    //PLOT_GIF pl[6];
    //for (int i = 0; i < 6; i++)
    //    pl->plot_GIF_start();

    while (order<6) {
        start_time = clock();
        
        // ---------------------bin文件数据包接收---------------------
        receive_data(data_bin);
        //存入ANT_LENGTH个通道，将其分开分别处理
        func_bin2complex(data_bin, sizeof(data_bin), data_tune);

        std::cout << "end_Pre_Load: " << (clock() - start_time) << "ms" << std::endl;

        order++;

        //分别处理ANT_LENGTH个通道
        for (int ii = 0; ii < ANT_LENGTH; ii++){
            // ---------------------数据预处理---------------------

            data = Pre_process(data_tune[ii], pre_load_data);
            //std::cout << "end_Pre_process: " << (clock() - start_time) << "ms" << std::endl;

            // ---------------------数据处理---------------------
            for (int j = 0; j < pre_load_data.nSlot; ++j) {
                FFT_1D(data[j]);
                for (int k = 0; k < nSC / 2; ++k)
                    data[j][k] = std::conj(data[j][k]);
            }

            //切片为200*65
            FFT_2D(data);

            for (int i = 0; i < data.size(); i++)
                for (int j = 0; j < data[0].size(); j++)
                    data_abs[i][j] = pow(std::abs(data[i][j]), 2);

            my_result = CFAR(data_abs);
            /*std::cout << "end: " << (clock() - start_time) << "ms" << std::endl;*/

            // ---------------------结果输出---------------------
            my_plot(data_abs, ii+1, order, my_result.SWCfarResultNJ_c);

            //pl[order].plot_GIF(data_abs, ii + 1, my_result.SWCfarResultNJ_c);

            if (my_result.Amplititude.size() != 0) {
                std::cout << "Channel " << ii+1 << " CFAR result:\n  range:";
                for (int i = 0; i < my_result.Amplititude.size(); i++) {
                    std::cout << (range_2dfft[my_result.SWCfarResultNJ_c[0][i]]) << "m; ";
                }
                std::cout << std::endl;
                std::cout << "  velocity:";
                for (int i = 0; i < my_result.Amplititude.size(); i++) {
                    std::cout << (velocity_2dfft[my_result.SWCfarResultNJ_c[1][i]]) << "m/s; ";
                }
                std::cout << std::endl << std::endl;
            }
            else std::cout << "Channel " << ii+1 << " CFAR result:\n  None" << std::endl << std::endl;
        }
        std::cout << "end: " << (clock() - start_time) << "ms" << std::endl;
    }

    //for (int i = 0; i < 6; i++)
    //    pl->plot_GIF_start();

    receive_stop();

    return 0;
}
