#include <iostream>
#include <vector>
#include <fftw3.h> // ����FFTWͷ�ļ�
#include <cmath>
#include <complex>

// �򻯵ĸ������壨���ڱ�׼�⣩
using Complex = std::complex<float>;

std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift);
std::vector<std::vector<Complex>> fftshift(const std::vector<std::vector<Complex>>& input);
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s);
