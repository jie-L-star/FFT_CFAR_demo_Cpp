#include <main.h>

struct CFAR_result {
	std::vector<std::vector<int>> SWCfarResultNJ_c;
	std::vector<double> Amplititude;

	CFAR_result() :SWCfarResultNJ_c(2) {}
};


CFAR_result CFAR(std::vector<std::vector<double>>& s);

int CFARHelper1(int x, int Nmax);
std::vector<int> CFARHelper1(std::vector<int> x, int Nmax);
bool CFARHelper2(std::vector<std::vector<double>> data, int x, int y, int Xmax, int Ymax);
std::vector<int> CFARHelper3(std::vector<int> x, int Nmax);
double findMedian(std::vector<double>& nums1, std::vector<double>& nums2);
