#include <locate.h>
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

const double c = 3e8;

// 计算定位结果的函数
void locate(const std::vector<std::vector<double>>& BS_Sets, int N, const std::vector<double>& R, float r_zp[]) {
    std::vector<double> k(N);
    std::vector<double> hh(N - 1);

    // 计算 k
    for (int i = 0; i < N; ++i) {
        k[i] = BS_Sets[0][i] * BS_Sets[0][i] + BS_Sets[1][i] * BS_Sets[1][i];
    }

    // 计算 h
    for (int i = 0; i < N - 1; ++i) {
        hh[i] = 0.5 * (R[i] * R[i] - k[i + 1] + k[0]);
    }
    Eigen::VectorXd h(N - 1); // h

    for (int i = 0; i < N - 1; ++i) {
        h(i) = hh[i];
    }
    /*  std::cout << h << std::endl;*/

    std::vector<std::vector<double>> Gaa(N - 1, std::vector<double>(3));

    // 计算 Ga
    for (int i = 0; i < N - 1; ++i) {
        Gaa[i][0] = -(BS_Sets[0][i + 1] - BS_Sets[0][0]);
        Gaa[i][1] = -(BS_Sets[1][i + 1] - BS_Sets[1][0]);
        Gaa[i][2] = R[i];
    }


    Eigen::MatrixXd Ga(N - 1, 3); // Ga
    Eigen::MatrixXd GaT(3, N - 1); // Ga

    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < 3; ++j) {
            Ga(i, j) = Gaa[i][j];
        }
    }
    GaT = Ga.transpose();
    //// 打印 Ga 的值
    ////std::cout << GaT  << std::endl;
    //std::cout << Ga<< std::endl;



    Eigen::MatrixXd Q = Eigen::MatrixXd::Identity(N - 1, N - 1); // Q
    /*  std::cout << Q << std::endl;*/

    Eigen::MatrixXd pinv_GaT_Q_Ga = (Ga.transpose() * Q.inverse() * Ga).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(Eigen::MatrixXd::Identity(3, 3));
    /*  std::cout << pinv_GaT_Q_Ga << std::endl;*/
    Eigen::VectorXd za0 = pinv_GaT_Q_Ga * Ga.transpose() * Q.inverse() * h;

    // 输出结果
   /* std::cout << "za0:" << std::endl;
    std::cout << za0 << std::endl;*/
    Eigen::VectorXd B(N - 1);
    for (int i = 1; i < N; ++i) {
        B(i - 1) = std::sqrt(std::pow(BS_Sets[0][i] - za0(0), 2) + std::pow(BS_Sets[1][i] - za0(1), 2));
    }

    // 将 B 转换为对角矩阵
    Eigen::MatrixXd B_diag = B.asDiagonal();

    // 计算 S
    Eigen::MatrixXd S = c * c * B_diag * Eigen::MatrixXd::Identity(N - 1, N - 1) * B_diag;

    // 计算 za1
    Eigen::VectorXd za1 = (Ga.transpose() * S.inverse() * Ga).inverse() * Ga.transpose() * S.inverse() * h;

    //std::cout << "za1: " << za1 << std::endl;

    double x1 = BS_Sets[0][0];
    double y1 = BS_Sets[1][0];

    Eigen::VectorXd h2(3);
    h2 << pow(za1(0, 0) - x1, 2),
        pow(za1(1, 0) - y1, 2),
        pow(za1(2, 0), 2);

    Eigen::MatrixXd Ga2(3, 2);
    Ga2 << 1, 0,
        0, 1,
        1, 1;

    Eigen::MatrixXd B2(3, 3);
    B2 << za1(0, 0) - x1, 0, 0,
        0, za1(1, 0) - y1, 0,
        0, 0, za1(2, 0);

    Eigen::MatrixXd S2 = 4 * B2 * pinv_GaT_Q_Ga * B2;

    Eigen::VectorXd za2 = (Ga2.transpose() * (S2.inverse()) * Ga2).inverse() * (Ga2.transpose() * (S2.inverse()) * h2);

    Eigen::MatrixXd zp(2, 2);
    zp << sqrt(fabs(za2(0))) + x1, -sqrt(fabs(za2(0))) + x1,
        sqrt(fabs(za2(1))) + y1, -sqrt(fabs(za2(1))) + y1;

    //std::cout << "初始化WLS结果 x: " << za0(0) << " y: " << za0(1) << std::endl;
    //std::cout << "第一次WLS结果 x: " << za1(0) << " y: " << za1(1) << std::endl;
    std::cout << "第二次WLS结果[1] x: " << zp(0, 0) << " y: " << zp(1, 0) << std::endl;
    //std::cout << "第二次WLS结果[2] x: " << zp(0, 1) << " y: " << zp(1, 1) << std::endl;

    r_zp[0] = zp(0, 0);
    r_zp[1] = zp(0, 1);

    return;
   
}
