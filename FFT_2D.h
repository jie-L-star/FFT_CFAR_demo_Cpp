#include <main.h>
#include <fftw3.h>

void FFT_1D(std::vector<Complex>& input);
void IFFT_1D(std::vector<Complex>& input);
void fftshift(std::vector<std::vector<Complex>>& input);
void FFT_2D(std::vector<std::vector<Complex>>& s);
