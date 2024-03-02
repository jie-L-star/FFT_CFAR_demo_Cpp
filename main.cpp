#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <matlab_function.h>
#include <CFAR.h>


int main() {

    //std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14.csv";
    //const int dim1 = 8;     // ��һά��С ͨ��(8��)
    //const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    //const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";
    const int dim1 = 1;     // ��һά��С ͨ��(8��)
    const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    //һ��1000�����ţ���100Ϊ��������
    const int t_length1 = 1000;
    const int t_length = (dim2 - 1000) / 100;  //tlength

    //���뷶ΧΪ0~50m �ٶȷ�ΧΪ-4~4m/s
    std::vector<int> range_index = maohao<int>(range_index_min, range_index_max, 1);
    std::vector<int> velocity_index = maohao<int>(velocity_index_min, velocity_index_max, 1);


    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    std::vector<std::vector<std::vector<Complex>>> Receive(t_length, std::vector<std::vector<Complex>>(t_length1, std::vector<Complex>(dim3)));
    std::vector<std::vector<std::vector<Complex>>> Result_fft(t_length, std::vector<std::vector<Complex>>(velocity_index_size, std::vector<Complex>(range_index_size)));

    //��������
    threeDArray = load_data(dim1, dim2, dim3, path);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            threeDArray[i][j] = FFT_1D(threeDArray[i][j], false);
            for (int k = 0; k < dim3; ++k)
                threeDArray[i][j][k] = std::conj(threeDArray[i][j][k]);
        }
    }

    // FFT_2D���
    for (int i = 0; i < t_length; ++i) {
        for (int j = 0; j < t_length1; ++j) {
            for (int k = 0; k < K; ++k) {
                Receive[i][j][k] = threeDArray[0][100 * i + j][k];
            }
        }
        Result_fft[i] = FFT_2D(Receive[i]);
    }

    CFAR_result my_result[t_length];

    std::vector<std::vector<double>> R_temp(velocity_index_size, std::vector<double>(range_index_size));
    for (int ii = 0; ii < t_length; ++ii) {
        for (size_t i = 0; i < velocity_index_size; i++)
            for (size_t j = 0; j < range_index_size; j++)
                R_temp[i][j] = pow(std::abs(Result_fft[ii][i][j]), 2);

        my_result[ii] = CFAR(R_temp);
    }


    //��ͼ��
    //std::vector<double> range_2dfft = linspace<double>(0, (c0 / (2 * delta_f)), (K + 1));
    //std::vector<double> velocity_2dfft = linspace<double>(-lambda / 2 / Ts, lambda / 2 / Ts, 1000 + 1);

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

    return 0;
}

 

 