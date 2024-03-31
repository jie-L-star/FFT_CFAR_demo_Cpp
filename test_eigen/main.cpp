#include <iostream>
#include <vector>
#include <armadillo>

using namespace std;
using namespace arma;

// ���帴������
typedef complex<float> Complex;

// ���������ά����
vector<vector<Complex>> generateRandomVector(int rows, int cols) {
    vector<vector<Complex>> vec(rows, vector<Complex>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            vec[i][j] = { static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX };
        }
    }
    return vec;
}

// ����ά����ת��Ϊ Armadillo ����
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
    // ���þ����С
    int rowsA = 792;
    int colsA = 792;
    int rowsB = 200;
    int colsB = 792;
    auto start = chrono::high_resolution_clock::now();

    // �����������
    auto vecA = generateRandomVector(rowsA, colsA);
    auto vecB = generateRandomVector(rowsB, colsB);

    // ת��Ϊ Armadillo ����
    Mat<Complex> armaA = vectorToMatrix(vecA);
    Mat<Complex> armaB = vectorToMatrix(vecB);

    // ����������ʱ��
    Mat<Complex> armaResult = trans(armaA) * trans(armaB);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Matrix multiplication time using Armadillo: " << duration.count() << " milliseconds" << endl;

    return 0;
}
