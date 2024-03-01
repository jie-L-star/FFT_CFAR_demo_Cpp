#include <main.h>

//实现":"生成一维向量
template <typename T>
std::vector<T> maohao(T start, T end, T step) {
    std::vector<T> result_maohao;

    while (start <= end) {
        result_maohao.push_back(start);
        start += step;
    }

    return result_maohao;
}

//实现"linspace"生成一维向量
template <typename T>
std::vector<T> linspace(T start, T end, int size) {
    std::vector<T> result_lin;

    T diff = (end - start) / <T>(size - 1);
    for (int i = 0; i < size; ++i) {
        result_lin.push_back(start + i * diff);
    }
    return result_lin;
}