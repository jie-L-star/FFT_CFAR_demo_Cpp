#include <iostream>
#include <vector>
#include <armadillo>

using namespace std;
using namespace arma;

// 定义复数类型
typedef complex<float> Complex;

// 生成随机二维向量
vector<vector<Complex>> generateRandomVector(int rows, int cols) {
    vector<vector<Complex>> vec(rows, vector<Complex>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            vec[i][j] = { static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX };
        }
    }
    return vec;
}

// 将二维向量转换为 Armadillo 矩阵
Mat<Complex> vectorToMatrix(const vector<vector<Complex>>& vec) {
    int rows = vec.size();
    int cols = vec[0].size();
    Mat<Complex> mat(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mat(i, j) = vec[i][j];
        }
    }
    return mat;
}

int main() {
    // 设置矩阵大小
    int rowsA = 792;
    int colsA = 792;
    int rowsB = 200;
    int colsB = 792;
    auto start = chrono::high_resolution_clock::now();

    // 生成随机矩阵
    auto vecA = generateRandomVector(rowsA, colsA);
    auto vecB = generateRandomVector(rowsB, colsB);

    // 转换为 Armadillo 矩阵
    Mat<Complex> armaA = vectorToMatrix(vecA);
    Mat<Complex> armaB = vectorToMatrix(vecB);

    // 计算矩阵相乘时间
    Mat<Complex> armaResult = trans(armaA) * trans(armaB);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Matrix multiplication time using Armadillo: " << duration.count() << " milliseconds" << endl;

    return 0;
}
