#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <matlab_function.h>
#include <CFAR.h>

int main() {

    std::string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/CFAR.csv";
    const int dim2 = 427;
    const int dim3 = 65;

    std::vector<std::vector<double>> R_temp(dim2, std::vector<double>(dim3));

    // 打开CSV文件
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return {};  //返回空容器
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::stringstream ss(line);
    std::string cell;
    int i = 0, j = 0;
    while (std::getline(ss, cell, ',')) {

        R_temp[i][j] = std::stod(cell);

        j++;
        if (j == dim3) {
            j = 0;
            i++;
        }
    }
    

    CFAR_result my_result = CFAR(R_temp);

    return 0;
}
