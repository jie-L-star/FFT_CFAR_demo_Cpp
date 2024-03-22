#include <Eigen/Dense>
#include <complex>
#include <vector>
#include <main.h>


// ��άvector�������
std::vector<std::vector<Complex>> multiplyVectors_eigen(const std::vector<std::vector<Complex>>& vecA, std::vector<std::vector<Complex>>& vecB) {

    int rowsA = vecA.size();
    int colsA = vecA[0].size();
    int rowsB = vecB.size();
    int colsB = vecB[0].size();

    // ����Eigen��Map��ͼ
    Eigen::Map<const Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenA(&vecA[0][0], rowsA, colsA);
    Eigen::Map<Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eigenB(&vecB[0][0], rowsB, colsB); // ע������colsA��Ӧ������

    // ִ�о���˷�
    //eigenB = eigenB.transposeInPlace();
    Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> result = eigenA * (eigenB.transpose());
    //Eigen::MatrixXcf result = eigenA * (eigenB.transpose());
    //auto result = eigenA * (eigenB.transpose());

    for (int i = 0; i < result.rows(); ++i) {
        for (int j = 0; j < result.cols(); ++j) {
            vecB[i][j] = result(i, j);
        }
    }

    return vecB;
}



// ��άvector�������
std::vector<std::vector<Complex>> multiplyVectors(const std::vector<std::vector<Complex>>& matrixA, const std::vector<std::vector<Complex>>& matrixB) {
    assert(matrixA[0].size() == matrixB.size()); // ȷ������˷��Ϸ�

    std::vector<std::vector<Complex>> result(matrixA.size(), std::vector<Complex>(matrixB[0].size()));

    for (size_t i = 0; i < matrixA.size(); ++i) {
        for (size_t j = 0; j < matrixB[0].size(); ++j) {
            for (size_t k = 0; k < matrixA[0].size(); ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}