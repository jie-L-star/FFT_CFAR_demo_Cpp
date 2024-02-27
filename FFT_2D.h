#include <iostream>
#include <vector>
#include <fftw3.h> // 包含FFTW头文件
#include <cmath>
#include <complex>

// 简化的复数定义（基于标准库）
using Complex = std::complex<float>;

std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift);
std::vector<std::vector<Complex>> fftshift(const std::vector<std::vector<Complex>>& input);
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s);
