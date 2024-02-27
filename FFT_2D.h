#include <iostream>
#include <vector>
#include <fftw3.h> // ����FFTWͷ�ļ�
#include <cmath>
#include <complex>

#define M_PI 3.1415926

// �򻯵ĸ������壨���ڱ�׼�⣩
using Complex = std::complex<float>;

std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift);
void FFT(std::vector<Complex>& input, bool inverse = false);
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s);
