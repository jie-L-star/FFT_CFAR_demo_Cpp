#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

int main() {

    char str[100] = {};
    for (int i = 1; i <= 8; i++)
    {
        vector<string> pngFiles;
        for (int j = 1; j <= 1; j++) {
            sprintf(str, "D:/FFT_CFAR_demo_Cpp/result_image/channel_%d_%d.png", i, j);
            pngFiles.emplace_back(str);
        }
        // Read the first PNG image to get its size
        Mat firstImage = imread(pngFiles[0], IMREAD_UNCHANGED);
        if (firstImage.empty()) {
            cerr << "Error reading image: " << pngFiles[0] << endl;
            return -1;
        }

        sprintf(str, "D:/FFT_CFAR_demo_Cpp/result_image/channel_%d.gif", i);
        VideoWriter gifWriter(str, VideoWriter::fourcc('G', 'I', 'F', ' '), 10, Size(firstImage.cols, firstImage.rows), true);

        for (const auto& pngFile : pngFiles) {
            Mat image = imread(pngFile, IMREAD_UNCHANGED);
            if (image.empty()) {
                cerr << "Error reading image: " << pngFile << endl;
                continue;
            }
            gifWriter.write(image);
        }

        gifWriter.release();
        cout << "GIF created successfully!" << endl;

    }
    return 0;
}
