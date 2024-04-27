#include <mgl2/mgl.h>
#include <my_plot.h>
#include <receivers.h>


void my_plot(std::vector<std::vector<float>> data_abs, int tune, int order, std::vector<std::vector<int>>targets_pos)
{
    float max_data = 20 * log10(data_abs[0][0]);
    float min_data = 20 * log10(data_abs[0][0]);

    // 创建一个二维数组，假设是一个10x10的数组
    const int velocity = data_abs.size();
    const int range = data_abs[0].size();

    mglGraph gr;
    mglData mat(velocity, range);

    for (int i = 0; i < velocity; ++i) {
        for (int j = 0; j < range; ++j) {
            data_abs[i][j] = 20 * log10(data_abs[i][j]);
            if (data_abs[i][j] > max_data) max_data = data_abs[i][j];
            if (data_abs[i][j] < min_data) min_data = data_abs[i][j];
        }
    }

    for (int i = 0; i < velocity; ++i)
        for (int j = 0; j < range; ++j)
            mat.a[i * range + j] = (data_abs[i][j] - min_data) / (max_data - min_data) * PLOT_Z_MAX;

    gr.ClearFrame();    //刷新图窗
    gr.SetRanges(0, velocity, 0, range, 0, PLOT_Z_MAX); // 设置坐标轴范围
    gr.Box();
    gr.Surf(mat,"bcyr");

    if (!targets_pos[0].empty()){
        for (int i = 0; i < targets_pos[0].size(); i++){
            mglPoint p = { (double)targets_pos[1][i], (double)targets_pos[0][i] ,PLOT_Z_MAX };
            gr.FaceZ(p, 4, 2, "r"); // 在目标处画一个矩形，颜色为红色
        }
    }

    char str[100] = "";
    sprintf(str, "D:/FFT_CFAR_demo_Cpp/result_image/channel_%d_%d.png", tune, order);
    gr.WriteFrame(str);

    //sprintf(str, "D:/FFT_CFAR_demo_Cpp/result_data_abs/data/tune_%d_%d.csv", tune, order);
    //writeCSV(data_abs, str);

    return;
}

//void plot_GIF(mglGraph gr, std::vector<std::vector<float>> data_abs, int tune, int order, std::vector<std::vector<int>>targets_pos)
//{
//    float max_data = 20 * log10(data_abs[0][0]);
//    float min_data = 20 * log10(data_abs[0][0]);
//
//    // 创建一个二维数组，假设是一个10x10的数组
//    const int velocity = data_abs.size();
//    const int range = data_abs[0].size();
//
//    mglData mat(velocity, range);
//
//    for (int i = 0; i < velocity; ++i) {
//        for (int j = 0; j < range; ++j) {
//            data_abs[i][j] = 20 * log10(data_abs[i][j]);
//            if (data_abs[i][j] > max_data) max_data = data_abs[i][j];
//            if (data_abs[i][j] < min_data) min_data = data_abs[i][j];
//        }
//    }
//
//    for (int i = 0; i < velocity; ++i)
//        for (int j = 0; j < range; ++j)
//            mat.a[i * range + j] = (data_abs[i][j] - min_data) / (max_data - min_data) * PLOT_Z_MAX;
//
//    gr.NewFrame();    //刷新图窗
//    gr.SetRanges(0, velocity, 0, range, 0, PLOT_Z_MAX); // 设置坐标轴范围
//    gr.Box();
//    gr.Surf(mat, "bcyr");
//
//    if (!targets_pos[0].empty()) {
//        for (int i = 0; i < targets_pos[0].size(); i++) {
//            mglPoint p = { (double)targets_pos[1][i], (double)targets_pos[0][i] ,PLOT_Z_MAX };
//            gr.FaceZ(p, 4, 2, "r"); // 在目标处画一个矩形，颜色为红色
//        }
//    }
//
//    gr.EndFrame();
//
//    return;
//}
//
//
//void plot_GIF_start(mglGraph* graphs)
//{
//    char str[100] = "";
//    //mglGraph* graphs[ANT_LENGTH];
//    for (int i = 0; i < ANT_LENGTH; i++)
//    {
//        sprintf(str, "D:/FFT_CFAR_demo_Cpp/result_image/channel_%d.gif", i);
//        graphs[i] = new mglGraph();
//        graphs[i]->StartGIF(str);
//    }
//
//    //return graphs;
//}
//
//void plot_GIF_stop(std::vector<mglGraph*>& gr_array)
//{
//    for (int i = 0; i < ANT_LENGTH; i++)
//    {
//        gr_array[i]->CloseGIF();
//        delete gr_array[i];
//    }
//}


//int main(int, char**)
//{
//    mglGraph gr;
//    mglData dat(100);
//    char str[32];
//    gr.StartGIF("sample.gif");
//
//    for (int i = 0; i < 40; i++)
//    {
//        gr.NewFrame(); // start frame
//        gr.Box(); // some plotting
//        for (int j = 0; j < dat.nx; j++)
//            dat.a[j] = sin(M_PI * j / dat.nx + M_PI * 0.05 * i);
//        gr.Plot(dat, "b");
//        gr.EndFrame(); // end frame
//    }
//    gr.CloseGIF();
//
//    return 0;
//}

//int main()
//{
//    std::vector<std::vector<float>> data_abs = readCSV("D:/FFT_CFAR_demo_Cpp/result_data_abs/tune_5_1.csv");
//
//    const int velocity = data_abs.size();
//    for (int i = 0; i < velocity; ++i)
//        data_abs[i].erase(data_abs[i].begin() + 65, data_abs[i].end());
//
//    std::vector<std::vector<int>> targets_pos = { {30}, {100} };
//    my_plot(data_abs, 1, 1, targets_pos);
//
//    return 0;
//}


void writeCSV(const std::vector<std::vector<float>>& data, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // 遍历二维向量并将其写入文件
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            // 在每个元素后面添加逗号，除了最后一个元素
            if (i != row.size() - 1) {
                file << ",";
            }
        }
        file << std::endl; // 换行
    }

    file.close();
}

std::vector<std::vector<float>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<float>> data;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<float> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            // 将字符串转换为浮点数，并将其添加到行向量中
            row.push_back(std::stof(cell));
        }
        data.push_back(row);
    }

    file.close();

    return data;
}