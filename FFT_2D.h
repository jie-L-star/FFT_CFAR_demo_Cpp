#include <main.h>
#include <fftw3.h>

#define M_PI 3.1415926


std::vector<Complex> FFT_1D(const std::vector<Complex>& input, bool shift);
std::vector<std::vector<Complex>> fftshift(const std::vector<std::vector<Complex>>& input);
std::vector<std::vector<Complex>> FFT_2D(std::vector<std::vector<Complex>>& s);
