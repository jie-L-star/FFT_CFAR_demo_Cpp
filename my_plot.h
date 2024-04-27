#include <main.h>
//#include <mgl2/mgl.h>

//��һ��0��100
#define PLOT_Z_MAX 100

//class PLOT_GIF
//{
//	mglGraph gr;
//    char str[100];
//public:
//	void plot_GIF_start();
//	void plot_GIF(std::vector<std::vector<float>> data_abs, int tune, std::vector<std::vector<int>>targets_pos);
//	void plot_GIF_stop();
//};
//
//void PLOT_GIF::plot_GIF_start()
//{
//    gr.StartGIF(str);
//}
//
//void PLOT_GIF::plot_GIF(std::vector<std::vector<float>> data_abs, int tune, std::vector<std::vector<int>>targets_pos)
//{
//    float max_data = 20 * log10(data_abs[0][0]);
//    float min_data = 20 * log10(data_abs[0][0]);
//
//    // ����һ����ά���飬������һ��10x10������
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
//    gr.NewFrame();    //ˢ��ͼ��
//    gr.SetRanges(0, velocity, 0, range, 0, PLOT_Z_MAX); // ���������᷶Χ
//    gr.Box();
//    gr.Surf(mat, "bcyr");
//
//    if (!targets_pos[0].empty()) {
//        for (int i = 0; i < targets_pos[0].size(); i++) {
//            mglPoint p = { (double)targets_pos[1][i], (double)targets_pos[0][i] ,PLOT_Z_MAX };
//            gr.FaceZ(p, 4, 2, "r"); // ��Ŀ�괦��һ�����Σ���ɫΪ��ɫ
//        }
//    }
//
//    gr.EndFrame();
//
//    return;
//}
//
//void PLOT_GIF::plot_GIF_stop()
//{
//    gr.CloseGIF();
//}


void writeCSV(const std::vector<std::vector<float>>& data, const std::string& filename);
std::vector<std::vector<float>> readCSV(const std::string& filename);
void my_plot(std::vector<std::vector<float>> data_abs, int tune, int order, std::vector<std::vector<int>>targets_pos);

