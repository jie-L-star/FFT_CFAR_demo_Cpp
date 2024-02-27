#include <load_data.h>

std::vector<std::vector<std::vector<Complex>>>load_data(int dim1, int dim2, int dim3, std::string path) {

    // 频域(快时间)* 时域(慢时间)* 通道(8个)
    //const int dim1 = 8;     // 第一维大小 通道(8个)
    //const int dim2 = 9600;  // 第二维大小 时域(慢时间)
    //const int dim3 = 792;   // 第三维大小 频域(快时间)

    //const int dim1 = 4;     // 第一维大小 通道(8个)
    //const int dim2 = 3;  // 第二维大小 时域(慢时间)
    //const int dim3 = 2;   // 第三维大小 频域(快时间)

    int size;
    size = dim1 * dim2 * dim3;

    //Complex threeDArray[dim1][dim2][dim3];    //静态堆栈会爆炸std::vector<std::vector<std::vector<std::complex<double>>>> data(CHANNELS,
    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
    //std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));

    // 打开CSV文件
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        //return;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::stringstream ss(line);
    std::string cell;
    int op = 0;
    int i = 0, j = 0, k = 0;
    while (std::getline(ss, cell, ',')) {
        float real_part, imag_part;
        char plus_sign, imaginary_unit;
        op++;
        if (op % 1000000 == 0)
        {
            std::cout << op << std::endl;
        }

        std::stringstream cell_ss(cell);
        cell_ss >> real_part >> plus_sign >> imag_part >> imaginary_unit;

        if (plus_sign == '+' && imaginary_unit == 'i') {
            threeDArray[i][j][k] = Complex(real_part, imag_part);
        }
        else if (plus_sign == '-' && imaginary_unit == 'i')
        {
            threeDArray[i][j][k] = Complex(real_part, -imag_part);
        }
        else {
            std::cerr << "Error: Invalid complex number format in CSV file" << std::endl;
            //return;
        }

        k++;
        if (k == dim3) {
            k = 0;
            j++;
            if (j == dim2) {
                j = 0;
                i++;
            }
        }
    }
    return threeDArray;
}