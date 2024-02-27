#include <iostream>
#include <vector>
#include <fftw3.h> // 包含FFTW头文件
#include <cmath>
#include <complex>

#define M_PI 3.1415926

// 简化的复数定义（基于标准库）
using Complex = std::complex<float>;

// 使用Hanning窗函数
std::vector<float> HanningWindow(int size) {
    std::vector<float> window(size);
    for (int i = 0; i < size; ++i) {
        window[i] = 0.5 * (1 - cos(2 * M_PI * (i+1) / (size + 1)));
    }
    return window;
}

std::vector<std::vector<Complex>> fftshift(const std::vector<std::vector<Complex>>& input) {
    int rows = input.size();
    int cols = input[0].size();
    int shift_rows = rows / 2;
    int shift_cols = cols / 2;

    std::vector<std::vector<Complex>> output(rows, std::vector<Complex>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx_row = (i + shift_rows) % rows;
            int idx_col = (j + shift_cols) % cols;
            output[idx_row][idx_col] = input[i][j];
        }
    }

    return output;
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
            result[i] = Complex(out[i][0], out[i][1]);
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
    int QNum = s[0].size();    //行
    int SNum = s.size();       //列

    std::vector<std::vector<Complex>> result_fft(SNum, std::vector<Complex>(QNum));

    // 应用窗函数和FFT于每一行（距离维度）
    std::vector<float> WindowR = HanningWindow(QNum);
    for (int i = 0; i < SNum; ++i) {
        std::vector<Complex> row(QNum);
        for (int j = 0; j < QNum; ++j) {
            row[j] = s[i][j] * WindowR[j];
        }
        row = FFT_1D(row, false); // 使用fftshift
        for (int j = 0; j < QNum; ++j) {
            s[i][j] = row[j];
        }
    }
    s = fftshift(s);

    // 应用窗函数和FFT于每一列（速度维度）
    std::vector<float> WindowV = HanningWindow(SNum);
    for (int j = 0; j < QNum; ++j) {
        std::vector<Complex> col(SNum);
        for (int i = 0; i < SNum; ++i) {
            col[i] = s[i][j] * WindowV[i];
        }
        col = FFT_1D(col, false); // 使用fftshift
        for (int i = 0; i < SNum; ++i) {
            result_fft[i][j] = col[i];
        }
    }
    result_fft = fftshift(result_fft);

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
