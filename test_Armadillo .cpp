#include <armadillo>
#include <main.h>


// 生成随机二维向量
std::vector<std::vector<Complex>> generateRandomVector(int rows, int cols) {
    std::vector<std::vector<Complex>> vec(rows, std::vector<Complex>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            vec[i][j] = { static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX };
        }
    }
    return vec;
}

// 将二维向量转换为 Armadillo 矩阵
arma::Mat<Complex> vectorToMatrix_Armadillo(const std::vector<std::vector<Complex>>& vec) {
    int rows = vec.size();
    int cols = vec[0].size();
    arma::Mat<Complex> mat(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat(i, j) = vec[i][j];
        }
    }
    return mat;
}

std::vector<std::vector<Complex>> multiplyVectors_Armadillo(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB) {

    // 转换为 Armadillo 矩阵
    arma::Mat<Complex> armaA = vectorToMatrix_Armadillo(vecA);
    arma::Mat<Complex> armaB = vectorToMatrix_Armadillo(vecB);

    arma::Mat<Complex> armaResult = strans(armaA) * strans(armaB);

    for (int i = 0; i < vecB[0].size(); ++i) {
        for (int j = 0; j < vecB.size(); ++j) {
            vecB[j][i] = armaResult(i, j);
        }
    }

    return vecB;
}
