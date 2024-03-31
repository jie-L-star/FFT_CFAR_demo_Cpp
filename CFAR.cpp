#include <CFAR.h>
#include <algorithm>
#include <matlab_function.h>


// 参数列表
#define Nc_R		32
#define Nc_V		16
#define	Nc_R_Gap	3
#define	Nc_V_Gap	10
#define	NfftRLeft	1
#define NfftRRight	30
#define PfN			1e-8

CFAR_result CFAR(std::vector<std::vector<float>>& s) {
	int Vsize = s.size();
	int Rsize = s[0].size();
	float NoiseAlphaR = Nc_R * (std::pow((float)PfN, (float)(-1 / (float)Nc_R))-1);
	float NoiseAlphaV = Nc_V * (std::pow((float)PfN, (float)(-1 / (float)Nc_V))-1);

	std::vector<std::vector<float>> Threshold(Vsize, std::vector<float>(Rsize));
	std::vector<std::vector<bool>> WCfar(Vsize, std::vector<bool>(Rsize));
	
	// 结果返回值
	CFAR_result my_result;

	//std::vector<float> ReferenceR1, ReferenceR2, ReferenceV1, ReferenceV2;
	std::vector<float> ReferenceR1;
	std::vector<float> ReferenceR2;

	std::vector<float> ReferenceV1;
	std::vector<float> ReferenceV2;

	std::vector<int> R_temp;
	float NoisePowerR, NoisePowerV;

	std::vector<std::vector<int>> temp_Vsize_1;
	std::vector<std::vector<int>> temp_Vsize_2;

	std::vector<std::vector<bool>> check_around(Vsize, std::vector<bool>(NfftRRight - NfftRLeft + 1, false));
	compareAdjacentElements(s, check_around);

	for (size_t j = 0; j < Vsize;j++) {
		temp_Vsize_1.push_back(CFARHelper1(maohao<int>(j - 2 - (Nc_V / 2 - 1) * Nc_V_Gap, j - 2, Nc_V_Gap), Vsize));
		temp_Vsize_2.push_back(CFARHelper1(maohao<int>(j + 2, j + 2 + (Nc_V / 2 - 1) * Nc_V_Gap, Nc_V_Gap), Vsize));
	}

	for (size_t i = 0; i < NfftRRight - NfftRLeft + 1; i++){

		std::vector<int> temp_Rsize_1 = CFARHelper3(maohao<int>(i - 2 - (Nc_R / 2 - 1) * Nc_R_Gap, i - 2, Nc_R_Gap), Rsize);
		std::vector<int> temp_Rsize_2 = CFARHelper3(maohao<int>(i + 2, i + 2 + (Nc_R / 2 - 1) * Nc_R_Gap, Nc_R_Gap), Rsize);

		for (size_t j = 0; j < Vsize; j++) {
			//if (CFARHelper2(s, i, j, Rsize, Vsize) == 0) {
			//	continue;
			//}
			if (check_around[j][i] == false)
				continue;

			if (i == 15 && j == 391)
				std::cout << "hello" << std::endl;


			for (int r : temp_Rsize_1) ReferenceR1.push_back(s[j][r]);
			for (int r : temp_Rsize_2) ReferenceR2.push_back(s[j][r]);
			for (int r : temp_Vsize_1[j]) ReferenceV1.push_back(s[r][i]);
			for (int r : temp_Vsize_2[j]) ReferenceV2.push_back(s[r][i]);

			NoisePowerR = findMedian(ReferenceR1, ReferenceR2);
			NoisePowerV = findMedian(ReferenceV1, ReferenceV2);
			Threshold[j][i] = std::max(NoisePowerR * NoiseAlphaR, NoisePowerV * NoiseAlphaV);

			// 高于阈值代表目标
			//WCfar[j][i] = s[j][i] > Threshold[j][i];
			//if (WCfar[j][i] == 1) {

			if (WCfar[j][i] = s[j][i] > Threshold[j][i]) {
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

//函数重载输入为int和vector<int>
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


bool CFARHelper2(std::vector<std::vector<float>> data, int x, int y, int Xmax, int Ymax) {
	bool flag = false;

	//std::cout << CFARHelper1(y - 1, Ymax) << " " << CFARHelper1(y + 1, Ymax) << " " << CFARHelper1(x - 1, Xmax) << " " << CFARHelper1(x + 1, Xmax) << std::endl;

	if (data[y][x] > data[CFARHelper1(y-1, Ymax)][x] && data[y][x] > data[CFARHelper1(y + 1, Ymax)][x])
		if (data[y][x] > data[y][CFARHelper1(x - 1, Xmax)] && data[y][x] > data[y][CFARHelper1(x + 1, Xmax)])
			flag = true;

	return flag;
}

std::vector<int> CFARHelper3(std::vector<int> vec, int Nmax) {
	vec.erase(std::remove_if(vec.begin(), vec.end(),[Nmax](int x) { return x < 0 || x > Nmax-1; }), vec.end());
	return vec;
}

// 寻找数组中位数
float findMedian(std::vector<float>& nums1, std::vector<float>& nums2) {
	std::vector<float> merged;
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


void compareAdjacentElements(const std::vector<std::vector<float>>& data, std::vector<std::vector<bool>> &result) {
	int rows = data.size();
	int cols = data[0].size();
	int cnt_true = 0;
	// 初始化结果矩阵，全部设为false

	// 遍历矩阵中的每个元素
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < result[0].size(); ++j) {
			if (data[i][j] > data[(i - 1 + rows) % rows][j] && data[i][j] > data[(i + 1) % rows][j] && data[i][j] > data[i][(j - 1 + cols) % cols] && data[i][j] > data[i][(j + 1) % cols]) {
				result[i][j] = true;
				cnt_true++;
			}
		}
	}
	printf("%d\n", cnt_true);
	
	return;
}
