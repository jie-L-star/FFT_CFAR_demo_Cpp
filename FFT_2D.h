#include <iostream>
#include <vector>
#include <fftw3.h> // 包含FFTW头文件
#include <cmath>
#include <complex>

#define M_PI 3.1415926

// 简化的复数定义（基于标准库）
using Complex = std::complex<float>;

std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift);
void FFT(std::vector<Complex>& input, bool inverse = false);
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s);
