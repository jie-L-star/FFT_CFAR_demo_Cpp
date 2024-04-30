#ifndef LOCATE_H
#define LOCATE_H

#include <vector>
//#include <Eigen/Dense>

void locate(const std::vector<std::vector<double>>& BS_Sets, int N, const std::vector<double>& R, float r_zp[]);

#endif
