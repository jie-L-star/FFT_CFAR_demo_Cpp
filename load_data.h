#include <main.h>

void load_data(int dim1, int dim2, int dim3, std::string path, std::vector<std::vector<std::vector<Complex>>>& threeDArray);
std::vector<std::vector<std::vector<Complex>>> boost_load_data(int dim1, int dim2, int dim3, const std::string& path);
void load_data_void(int dim1, int dim2, int dim3, std::string path, std::vector<std::vector<std::vector<Complex>>> threeDArray);
