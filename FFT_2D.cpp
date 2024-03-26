#include <FFT_2D.h>


// 使用Hanning窗函数
std::vector<float> HanningWindow(int size) {
    std::vector<float> window(size);
    for (int i = 0; i < size; ++i) {
        window[i] = 0.5 * (1 - cos(2 * M_PI * (i+1) / (size + 1)));
    }
    return window;
}


//移位函数
void fftshift(std::vector<std::vector<Complex>>& input) {
    int shift_rows = input.size() / 2;
    int shift_cols = input[0].size() / 2;

    for (int i = 0; i < input.size(); ++i) {
        std::rotate(input[i].begin(), input[i].begin() + shift_cols, input[i].end());
    }
    std::rotate(input.begin(), input.begin() + shift_rows, input.end());

    return;
}


// 执行一维FFT并返回复数结果向量
void FFT_1D(std::vector<Complex>& input) {
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

    for (int i = 0; i < N; ++i) {
        input[i] = Complex(out[i][0], out[i][1]);
    }
    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return;
}


// 2维FFT变换
void FFT_2D(std::vector<std::vector<Complex>>& s) {
    int QNum = s[0].size();    //行
    int SNum = s.size();       //列
    int NfftR = 1024;
    int WindowLenR = std::min(NfftR, QNum); //距离维窗长度

    std::vector<std::vector<Complex>> s_for_Rfft(SNum);

    // 应用窗函数和FFT于每一行（距离维度）
    std::vector<float> WindowR = HanningWindow(QNum);
    for (int i = 0; i < SNum; ++i) {
        std::vector<Complex> row(QNum);
        for (int j = 0; j < QNum; ++j) {
            row[j] = s[i][j] * WindowR[j];
        }
        FFT_1D(row); // 使用fftshift
        s_for_Rfft[i].assign(row.begin(), row.end());
    }

    fftshift(s_for_Rfft);

    // 应用窗函数和FFT于每一列（速度维度）
    std::vector<float> WindowV = HanningWindow(SNum);
    for (int j = 0; j < QNum; ++j) {
        std::vector<Complex> col(SNum);
        for (int i = 0; i < SNum; ++i) {
            col[i] = s_for_Rfft[i][j] * WindowV[i];
        }
        FFT_1D(col); // 使用fftshift

        for (int i = 0; i < SNum; ++i) {
            s[i][j] = col[i];
        }
    }

    fftshift(s);

    //切片 0~50m,-2m/s~2m/s
    s.erase(s.begin(), s.begin() + 287);
    s.erase(s.end() - 286, s.end());
    for (int i = 0; i < s.size(); i++)
        s[i].resize(65);
}


// 1维IFFT函数
// 执行一维FFT并返回复数结果向量
void IFFT_1D(std::vector<Complex>& input) {
    int N = input.size();
    fftw_complex* in = fftw_alloc_complex(N);
    fftw_complex* out = fftw_alloc_complex(N);

    // 复制输入数据到FFTW复数数组
    for (int i = 0; i < N; ++i) {
        in[i][0] = input[i].real();
        in[i][1] = input[i].imag();
    }

    // 创建计划并执行FFT
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    for (int i = 0; i < N; ++i) {
        input[i] = Complex(out[i][0]/N, out[i][1]/N);
    }
    // 清理
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return;
}