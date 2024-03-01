#include <load_data.h>

std::vector<std::vector<std::vector<Complex>>>load_data(int dim1, int dim2, int dim3, std::string path) {

    int size;
    size = dim1 * dim2 * dim3;

    std::vector<std::vector<std::vector<Complex>>> threeDArray(dim1, std::vector<std::vector<Complex>>(dim2, std::vector<Complex>(dim3)));
 
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
            return {};  //返回空容器
        }

        k++;
        if (k == dim3) {
            k = 0;
            j++;
            if (j == dim2) {
                j = 0;
                i++;
                //if (i==2)
                //    return threeDArray;
            }
        }
    }

    return threeDArray;
}


//实现"linspace"生成1维向量
//std::vector