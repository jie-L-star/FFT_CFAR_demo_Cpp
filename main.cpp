#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>


int main() {

    //std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14.csv";
    //const int dim1 = 8;     // ��һά��С ͨ��(8��)
    //const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    //const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";
    const int dim1 = 1;     // ��һά��С ͨ��(8��)
    const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    //std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/Y.csv";
    //const int dim1 = 2;     // ��һά��С ͨ��(8��)
    //const int dim2 = 3;  // �ڶ�ά��С ʱ��(��ʱ��)
    //const int dim3 = 3;   // ����ά��С Ƶ��(��ʱ��)

    const int t_length1 = 1000;
    const int t_length2 = (dim2 - 1000) / 100;  //tlength

    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    //��������

    threeDArray = load_data(dim1, dim2, dim3, path);
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim2; ++j) {
            threeDArray[i][j] = FFT_1D(threeDArray[i][j], false);
            for (int k = 0; k < dim3; ++k)
                threeDArray[i][j][k] = std::conj(threeDArray[i][j][k]);
        }
    }
    //for (int i = 0; i < dim1; i++) {
    //    //for (int j = 0; j < 1; j++) {
    //    //    threeDArray[i].back().pop_back();
    //    //}
    //    // ������ά�����Ĵ�С��ƥ���µ�����������
    //    threeDArray[i].resize(2);
    //    for (auto& row : threeDArray[i]) {
    //        row.resize(2);
    //    }
    //}

    //for (int i = 0; i < dim1; i++) {
    //    for (int j = 0; j < t_length1; j++) {
    //        threeDArray[i].back().pop_back();
    //    }
    //    // ������ά�����Ĵ�С��ƥ���µ�����������
    //    threeDArray[i].resize(t_length2);
    //    for (auto& row : threeDArray[i]) {
    //        row.resize(t_length1);
    //    }
    //}

    //for (int i = 0; i < dim1; ++i) {
    //    for (int row = 0; row < dim2; ++row) {
    //        for (int col = 0; col < 1000; ++col) {
    //            Recieve[row][col][i] = Recieve_14[row][100 * (i - 1)][5];
    //        }
    //    }
    //}

    FFT_2D(threeDArray[0]);

    //std::ofstream outputFile("fft_result.txt");

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

 

 