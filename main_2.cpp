#include <main.h>
#include <FFT_2D.h>
#include <load_data.h>
#include <matlab_function.h>
#include <CFAR.h>

int main() {

    std::string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/CFAR.csv";
    const int dim2 = 427;
    const int dim3 = 65;

    std::vector<std::vector<double>> R_temp(dim2, std::vector<double>(dim3));

    // ��CSV�ļ�
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "�޷����ļ�" << std::endl;
        return {};  //���ؿ�����
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
