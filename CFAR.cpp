#include <CFAR.h>
#include <algorithm>
#include <matlab_function.h>


// �����б�
#define Nc_R		32
#define Nc_V		16
#define	Nc_R_Gap	3
#define	Nc_V_Gap	10
#define	NfftRLeft	1
#define NfftRRight	50
#define PfN			1e-8

CFAR_result CFAR(std::vector<std::vector<double>>& s) {
	int Rsize = s[0].size();
	int Vsize = s.size();
	double NoiseAlphaR = Nc_R * (std::pow((double)PfN, (double)(-1 / (double)Nc_R))-1);
	double NoiseAlphaV = Nc_V * (std::pow((double)PfN, (double)(-1 / (double)Nc_V))-1);

	std::vector<std::vector<double>> Threshold(Vsize, std::vector<double>(Rsize));
	std::vector<std::vector<double>> WCfar(Vsize, std::vector<double>(Rsize));
	
	// �������ֵ
	CFAR_result my_result;

	std::vector<double> ReferenceR1, ReferenceR2, ReferenceV1, ReferenceV2;
	std::vector<int> R_temp;
	double NoisePowerR, NoisePowerV;


	for (size_t i = 0; i < NfftRRight - NfftRLeft + 1; i++){
		for (size_t j = 0; j < Vsize; j++) {
			if (CFARHelper2(s, i, j, Rsize, Vsize) == 0) {
				continue;
			}
			//R_temp = CFARHelper3(maohao<int>(i - 2 - (Nc_R / 2 - 1) * Nc_R_Gap, i - 2, Nc_R_Gap), Rsize);
			for (int r : CFARHelper3(maohao<int>(i - 2 - (Nc_R / 2 - 1) * Nc_R_Gap, i - 2, Nc_R_Gap), Rsize)) ReferenceR1.push_back(s[j][r]);
			for (int r : CFARHelper3(maohao<int>(i + 2, i + 2 + (Nc_R / 2 - 1) * Nc_R_Gap, Nc_R_Gap), Rsize)) ReferenceR2.push_back(s[j][r]);
			for (int r : CFARHelper1(maohao<int>(j - 2 - (Nc_V / 2 - 1) * Nc_V_Gap, j - 2, Nc_V_Gap), Vsize)) ReferenceV1.push_back(s[r][i]);
			for (int r : CFARHelper1(maohao<int>(j + 2, j + 2 + (Nc_V / 2 - 1) * Nc_V_Gap, Nc_V_Gap), Vsize)) ReferenceV2.push_back(s[r][i]);

			NoisePowerR = findMedian(ReferenceR1, ReferenceR2);
			NoisePowerV = findMedian(ReferenceV1, ReferenceV2);
			Threshold[j][i] = std::max(NoisePowerR * NoiseAlphaR, NoisePowerV * NoiseAlphaV);
			WCfar[j][i] = s[j][i] > 1 * Threshold[j][i];

			if (WCfar[j][i] == 1) {
				my_result.SWCfarResultNJ_c[0].push_back(i);
				my_result.SWCfarResultNJ_c[1].push_back(j);
				my_result.Amplititude.push_back(s[j][i]);
			}

			ReferenceR1.clear();
			ReferenceR2.clear();
			ReferenceV1.clear();
			ReferenceV2.clear();
		}
	}

	return my_result;
}

//������������Ϊint��vector<int>
int CFARHelper1(int x, int Nmax) {
	int temp = x;

	if (temp < 0)
		temp += Nmax;
	else if (temp > Nmax-1)
		temp -= Nmax;

	return temp;
}

std::vector<int> CFARHelper1(std::vector<int> x, int Nmax) {
	auto item = x.begin();
	std::vector<int> temp;

	while (item!=x.end())
	{
		if (*item < 0)
			*item += Nmax;
		else if (*item > Nmax-1) 
			*item -= Nmax;

		temp.push_back(*item);

		item++;
	}

	return temp;
}


bool CFARHelper2(std::vector<std::vector<double>> data, int x, int y, int Xmax, int Ymax) {
	bool flag = false;

	//std::cout << CFARHelper1(y - 1, Ymax) << " " << CFARHelper1(y + 1, Ymax) << " " << CFARHelper1(x - 1, Xmax) << " " << CFARHelper1(x + 1, Xmax) << std::endl;

	if (data[y][x] > 1 * data[CFARHelper1(y-1, Ymax)][x] && data[y][x] > 1 * data[CFARHelper1(y + 1, Ymax)][x])
		if (data[y][x] > 1 * data[y][CFARHelper1(x - 1, Xmax)] && data[y][x] > 1 * data[y][CFARHelper1(x + 1, Xmax)])
			flag = true;

	return flag;
}


std::vector<int> CFARHelper3(std::vector<int> x, int Nmax) {
	auto item = x.begin();
	std::vector<int> temp;

	while (item != x.end())
	{
		if (!(*item < 0 || *item > Nmax-1)) {
			temp.push_back(*item);
		}

		item++;
	}

	return temp;
}

// Ѱ��������λ��
double findMedian(std::vector<double>& nums1, std::vector<double>& nums2) {
	std::vector<double> merged;
	merged.insert(merged.end(), nums1.begin(), nums1.end());
	merged.insert(merged.end(), nums2.begin(), nums2.end());

	std::sort(merged.begin(), merged.end());

	int n = merged.size();
	if (n % 2 == 0) {
		return (merged[n / 2 - 1] + merged[n / 2]) / 2.0;
	}
	else {
		return merged[n / 2];
	}
}