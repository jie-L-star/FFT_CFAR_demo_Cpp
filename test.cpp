#include <Eigen/Core>
#include <complex>
#include <vector>
#include <main.h>


Eigen::MatrixXcf vectorToMatrix(const std::vector<std::vector<Complex>>& vec) {
    int rows = vec.size();
    int cols = vec[0].size();

    Eigen::MatrixXcf matrix(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix(i, j) = vec[i][j];

    return matrix;
}


std::vector<std::vector<Complex>> multiplyVectors_eigen_map(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB) {
    // 将二维向量映射到Eigen矩阵
    Eigen::Map<Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenA(vecA[0].data(), vecA.size(), vecA[0].size());
    Eigen::Map<Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenB(vecB[0].data(), vecB.size(), vecB[0].size());

    // 执行矩阵乘法
    Eigen::MatrixXcf result = (eigenA.transpose()) * (eigenB.transpose());

    // 将结果复制回二维向量
    for (int i = 0; i < result.rows(); ++i) {
        for (int j = 0; j < result.cols(); ++j) {
            vecB[j][i] = result(i, j);
        }
    }

    return vecB;
}


// 二维vector矩阵相乘
std::vector<std::vector<Complex>> multiplyVectors_eigen(std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB) {

    Eigen::MatrixXcf eigenA = vectorToMatrix(vecA);
    Eigen::MatrixXcf eigenB = vectorToMatrix(vecB);
   
    Eigen::MatrixXcf result = (eigenA.transpose()) * (eigenB.transpose());

    for (int i = 0; i < result.rows(); ++i) {
        for (int j = 0; j < result.cols(); ++j) {
            vecB[j][i] = result(i, j);
        }
    }

    return vecB;
}
