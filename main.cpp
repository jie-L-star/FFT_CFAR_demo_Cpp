#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>


int main() {
    
    std::string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/Y.csv";

    // 频域(快时间)* 时域(慢时间)* 通道(8个)
    //const int dim1 = 8;     // 第一维大小 通道(8个)
    //const int dim2 = 9600;  // 第二维大小 时域(慢时间)
    //const int dim3 = 792;   // 第三维大小 频域(快时间)

    const int dim1 = 4;     // 第一维大小 通道(8个)
    const int dim2 = 3;  // 第二维大小 时域(慢时间)
    const int dim3 = 2;   // 第三维大小 频域(快时间)

    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1,std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    
    //加载数据
    threeDArray = load_data(dim1, dim2, dim3, path);
    
    FFT_2D(threeDArray[0]);

    //std::ofstream outputFile("fft_result3.txt");
    //    // 进行一维FFT
    //for (int i = 0; i < dim1; ++i) {
    //    for (int j = 0; j < dim2; ++j) {
    //        threeDArray[i][j] = FFT_1D(threeDArray[i][j], false);
    //        // 将FFT结果保存到文件
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

 

 