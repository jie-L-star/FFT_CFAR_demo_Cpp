#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>


int main() {
    
    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/Y.csv";

    // Ƶ��(��ʱ��)* ʱ��(��ʱ��)* ͨ��(8��)
    //const int dim1 = 8;     // ��һά��С ͨ��(8��)
    //const int dim2 = 9600;  // �ڶ�ά��С ʱ��(��ʱ��)
    //const int dim3 = 792;   // ����ά��С Ƶ��(��ʱ��)

    const int dim1 = 4;     // ��һά��С ͨ��(8��)
    const int dim2 = 3;  // �ڶ�ά��С ʱ��(��ʱ��)
    const int dim3 = 2;   // ����ά��С Ƶ��(��ʱ��)

    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1,std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    
    //��������
    threeDArray = load_data(dim1, dim2, dim3, path);
    
    FFT_2D(threeDArray[0]);

    //std::ofstream outputFile("fft_result3.txt");
    //    // ����һάFFT
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

 

 