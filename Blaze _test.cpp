#include <blaze/Math.h>
#include <main.h>

using namespace blaze;

// �������˷�����
std::vector<std::vector<Complex>> multiplyMatrices(const std::vector<std::vector<Complex>>& matA, const std::vector<std::vector<Complex>>& matB) {
    // ������� std::vector ת��Ϊ Blaze �� DynamicMatrix ����
    DynamicMatrix<Complex> blazeMatA(matA.size(), matA[0].size());
    DynamicMatrix<Complex> blazeMatB(matB.size(), matB[0].size());

    // �����ݴ� std::vector ���Ƶ� Blaze ������
    for (size_t i = 0; i < matA.size(); ++i) {
        for (size_t j = 0; j < matA[0].size(); ++j) {
            blazeMatA(i, j) = matA[i][j];
        }
    }

    for (size_t i = 0; i < matB.size(); ++i) {
        for (size_t j = 0; j < matB[0].size(); ++j) {
            blazeMatB(i, j) = matB[i][j];
        }
    }

    // ִ�о���˷�
    //DynamicMatrix<Complex> blazeResult = trans(blazeMatA) * trans(blazeMatB);
    DynamicMatrix<Complex> blazeResult = blazeMatB * blazeMatA;

    // �� Blaze ������ת��Ϊ std::vector
    std::vector<std::vector<Complex>> result(blazeResult.rows(), std::vector<Complex>(blazeResult.columns()));
    for (size_t i = 0; i < blazeResult.rows(); ++i) {
        for (size_t j = 0; j < blazeResult.columns(); ++j) {
            result[i][j] = blazeResult(i, j);
        }
    }

    return result;
}


int main(void) {

    clock_t start_time = clock();

    const int n = 792;
    std::vector<std::vector<Complex>> A(n, std::vector<Complex>(n, Complex(1, 1)));
    std::vector<std::vector<Complex>> B(200, std::vector<Complex>(n, Complex(1, 1)));

    multiplyMatrices(A, B);

    std::cout << "Ԥ���غ�ʱ: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    return 0;
}