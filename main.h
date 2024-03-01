#include <iostream>
#include <sstream>
#include <complex>
#include <cmath>
#include <vector>
#include <fstream>

#define Ts		(0.625e-3)
#define c0		(3e+8)
#define delta_f (1*240e+3)
#define K		(1584/2)
#define N_rx	4
#define N_tx	1
#define fc		(25.5e9)
#define lambda	(c0/fc)

#define range_index_min	(std::ceil(0/(c0/(2*delta_f)/(K+1)))+1)
#define range_index_max	(std::ceil(50/(c0/(2*delta_f)/(K+1)))+1)

#define velocity_index_min (std::ceil((-4-(-lambda/2/Ts))/(lambda/Ts/(1000+1))))
#define velocity_index_max (std::ceil((4-(-lambda/2/Ts))/(lambda/Ts/(1000+1))))

#define range_index_size	((int)(range_index_max-range_index_min+1))
#define velocity_index_size	((int)(velocity_index_max-velocity_index_min+1))

// 简化的复数定义（基于标准库）
using Complex = std::complex<float>;
