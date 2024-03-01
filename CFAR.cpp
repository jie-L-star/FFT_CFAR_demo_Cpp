#include <CFAR.h>
#include <matlab_function.h>


// 参数列表
#define Nc_R		32
#define Nc_V		16
#define	Nc_R_Gap	3
#define	Nc_V_Gap	10
#define	NfftRLeft	1
#define NfftRRight	50
#define PfN			1e-8

void CFAR(std::vector<std::vector<double>>& s) {
	int Rsize = s[0].size();
	int Vsize = s.size();
	double NoiseAlphaR = Nc_R * (std::pow((double)PfN, (double)(-1 / Nc_R))-1);
	double NoiseAlphaV = Nc_V * (std::pow((double)PfN, (double)(-1 / Nc_V))-1);

	std::vector<std::vector<double>> Threshold(Vsize, std::vector<double>(Rsize));
	std::vector<std::vector<double>> WCfar(Vsize, std::vector<double>(Rsize));
	
	std::vector<double> ReferenceR1, ReferenceR2, ReferenceV1, ReferenceV2;
	std::vector<int> R_temp;

	for (size_t i = 0; i < NfftRRight - NfftRLeft + 1; i++){
		for (size_t j = 0; j < Vsize; j++) {
			if (CFARHelper2(s, i, j, Rsize, Vsize) == 0) {
				continue;
			}
			//R_temp = CFARHelper3(maohao<int>(i - 2 - (Nc_R / 2 - 1) * Nc_R_Gap, i - 2, Nc_R_Gap), Rsize);
			//ReferenceR1 = {};
			

		}
	}
}

//函数重载输入为int和vector<int>
int CFARHelper1(int x, int Nmax) {
	int temp = x;

	if (temp < 1)
		temp += Nmax;
	else if (temp > Nmax)
		temp -= Nmax;

	return temp;
}

std::vector<int> CFARHelper1(std::vector<int> x, int Nmax) {
	auto item = x.begin();
	std::vector<int> temp;

	while (item!=x.end())
	{
		if (*item < 1)
			*item += Nmax;
		else if (*item > Nmax) 
			*item -= Nmax;

		temp.push_back(*item);

		item++;
	}

	return temp;
}


bool CFARHelper2(std::vector<std::vector<double>> data, int x, int y, int Xmax, int Ymax) {
	bool flag = false;

	if (data[y][x] > 1 * data[CFARHelper1(y, Ymax)-1][x] && data[y][x] > 1 * data[CFARHelper1(y + 1, Ymax)][x])
		if (data[y][x] > 1 * data[y][CFARHelper1(x - 1, Xmax)] && data[y][x] > 1 * data[y][CFARHelper1(x + 1, Xmax)])
			flag = true;

	return flag;
}


std::vector<int> CFARHelper3(std::vector<int> x, int Nmax) {
	auto item = x.begin();
	std::vector<int> temp;

	while (item != x.end())
	{
		if (!(*item < 1 || *item > Nmax)) {
			temp.push_back(*item);
		}

		item++;
	}

	return temp;
}