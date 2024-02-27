#include <iostream>
#include <vector>
#include <fftw3.h> // 包含FFTW头文件
#include <cmath>
#include <complex>

// 简化的复数定义（基于标准库）
using Complex = std::complex<float>;

// 使用Hanning窗函数
std::vector<float> HanningWindow(int size) {
    std::vector<float> window(size);
    for (int i = 0; i < size; ++i) {
        //window[i] = 0.5 * (1 - cos(2 * M_PI * i / (size - 1)));
        window[i] = 1;
    }
    return window;
}

void FFT(std::vector<Complex>& input, bool inverse = false) {
    int size = input.size();

    // 创建FFTW输入和输出数组
    fftw_complex* in = reinterpret_cast<fftw_complex*>(input.data());
    fftw_complex* out = reinterpret_cast<fftw_complex*>(input.data());

    // 创建FFTW计划
    fftw_plan plan;
    if (inverse) {
        plan = fftw_plan_dft_1d(size, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    }
    else {
        plan = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }

    // 执行FFT
    fftw_execute(plan);

    // 销毁计划
    fftw_destroy_plan(plan);
}

// 执行一维FFT并返回复数结果向量
std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift) {
    int N = input.size();
    fftw_complex* in = fftw_alloc_complex(N);
    fftw_complex* out = fftw_alloc_complex(N);

    // 复制输入数据到FFTW复数数组
    for (int i = 0; i < N; ++i) {
        in[i][0] = input[i].real();
        in[i][1] = input[i].imag();
    }

    // 创建计划并执行FFT
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // 转换结果回复数向量
    std::vector<Complex> result(N);
    if (shift) {
        for (int i = 0; i < N / 2; ++i) {
            result[i + N / 2] = Complex(out[i][0], out[i][1]);
            result[i] = Complex(out[i + N / 2][0], out[i + N / 2][1]);
        }
    } else {
        for (int i = 0; i < N; ++i) {
            result[i] = Complex(out[i][0], -out[i][1]);
        }
    }

    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return result;
}

// 二维FFT变换
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s) {
    int QNum = s.size();    //行
    int SNum = s[0].size(); //列

    std::vector<std::vector<Complex>> result_fft(QNum, std::vector<Complex>(SNum));

    // 应用窗函数和FFT于每一行（距离维度）
    std::vector<float> WindowR = HanningWindow(QNum);
    for (int i = 0; i < QNum; ++i) {
        std::vector<Complex> row(SNum);
        for (int j = 0; j < SNum; ++j) {
            row[j] = s[i][j] * WindowR[i];
        }
        row = FFT_1D(row, true); // 使用fftshift
        for (int j = 0; j < SNum; ++j) {
            s[i][j] = row[j];
        }
    }

    // 应用窗函数和FFT于每一列（速度维度）
    std::vector<float> WindowV = HanningWindow(SNum);
    for (int j = 0; j < SNum; ++j) {
        std::vector<Complex> col(QNum);
        for (int i = 0; i < QNum; ++i) {
            col[i] = s[i][j] * WindowV[j];
        }
        col = FFT_1D(col, true); // 使用fftshift
        for (int i = 0; i < QNum; ++i) {
            result_fft[i][j] = col[i];
        }
    }

    return result_fft;
}

//int main() {
//    // 示例：创建一个简单的输入信号
//    int QNum = 1024; // 假设的天线数
//    int SNum = 1024; // 假设的快时间样本数
//
//    std::vector<std::vector<Complex>> s(QNum, std::vector<Complex>(SNum, Complex(1.0, 0))); // 示例输入
//
//    auto result_fft = FFT_2D(s, QNum, SNum);
//
//    // 打印某些结果以验证（实际使用时应替换为适当的结果处理或可视化代码）
//    std::cout << "FFT result (some values):" << std::endl;
//    for (int i = 0; i < 5; ++i) { // 仅打印前5个结果进行示意
//        std::cout << result_fft[i][i].real() << " + " << result_fft[i][i].imag() << "i" << std::endl;
//    }
//
//    return 0;
//}
