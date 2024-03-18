#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <complex>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <time.h>
#include <FFT_2D.h>
#include <CFAR.h>

using namespace std;
using Complex = complex<float>;

std::atomic<bool> load_flag(false);

void data_loading_thread(const string& path, queue<vector<vector<Complex>>>& dataQueue, mutex& mtx, condition_variable& cv) {
    clock_t start_time = clock();

    ifstream file(path);
    if (!file.is_open()) {
        cerr << "无法打开文件" << endl;
        return;
    }

    file.clear();  // 清除文件流的错误/EOF标记
    file.seekg(0, ios::beg);  // 将文件指针移动到文件开始

    vector<vector<Complex>> tempSegment(9600, vector<Complex>(792));
    int length = 9600;

    while (true) {
        string line;
        int cnt = 0, j = 0, k = 0;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string cell;
            vector<vector<Complex>> temp;

            float real_part, imag_part;
            char plus_sign, imaginary_unit;

            while (std::getline(ss, cell, ',')) {

                std::stringstream cell_ss(cell);
                cell_ss >> real_part >> plus_sign >> imag_part >> imaginary_unit;

                if ((plus_sign == '+' || plus_sign == '-') && imaginary_unit == 'i') {
                    // 取共轭
                    tempSegment[j][k] = Complex(real_part, (plus_sign == '+') ? -imag_part : +imag_part);
                }
                else {
                    std::cerr << "Error: Invalid complex number format in CSV file" << std::endl;
                }

                k++;

                if (k == 792) {
                    k = 0;
                    cnt++;
                    j++;
                    if (cnt == 1000) {
                        temp.insert(temp.end(), tempSegment.begin() + j - 1000, tempSegment.begin() + j);
                        cout << (j - 1000) / 100 + 1 << "次: " << temp[0][0];
                        //FFT_2D放在加载端，平衡两个线程时间
                        temp = FFT_2D(temp);

                        unique_lock<mutex> lock(mtx);
                        dataQueue.push(move(temp));
                        cv.notify_all();    //通知所有空闲线程

                        cout << "\t\t剩余" << dataQueue.size() << "个数据包待处理" << endl;

                        cnt -= 100;
                        temp.clear();
                    }

                    if (j >= 9600 - 100) {
                        load_flag.store(true);
                        file.close();

                        cout << "data_load 及 FFD_2D部分已完成" << "剩余" << dataQueue.size() << "个数据包待处理" << endl;
                        std::cout << "data_load 及 FFD_2D: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
                        return;
                    }
                }
            }
        }
    }
}


void consume_data_segments(queue<vector<vector<Complex>>>& dataQueue, mutex& mtx, condition_variable& cv) {
    clock_t start_time = clock();
    std::vector<std::vector<float>> R_temp(velocity_index_size, std::vector<float>(range_index_size));

    while (!load_flag.load() || !dataQueue.empty()) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&dataQueue] { return !dataQueue.empty(); });

        if (!dataQueue.empty()) {
            vector<vector<Complex>> segment = move(dataQueue.front());
            dataQueue.pop();
            lock.unlock();

            // 处理数据段
            for (size_t i = 0; i < velocity_index_size; i++)
                for (size_t j = 0; j < range_index_size; j++)
                    R_temp[i][j] = pow(std::abs(segment[i][j]), 2);

            CFAR(R_temp);
        }
    }

    cout << "数据处理部分已完成" << endl;
    std::cout << "CFAR: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
}


int main() {
    
    clock_t start_time = clock();

    queue<vector<vector<Complex>>> dataQueue;
    mutex mtx;
    condition_variable cv;

    string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";  // 替换成你的CSV文件路径

    //thread consumer(consume_data_segments, ref(dataQueue), ref(mtx), ref(cv));
    thread data_loader(data_loading_thread, path, ref(dataQueue), ref(mtx), ref(cv));

    //consumer.join();
    //data_loader.join();


    std::vector<std::thread> consumer;
    for (int i = 0; i < 3; ++i) {
        consumer.push_back(std::thread(consume_data_segments, std::ref(dataQueue), std::ref(mtx), std::ref(cv)));
    }

    for (auto& t : consumer) {
        t.join();
    }

    data_loader.join();

    std::cout << "end: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    return 0;
}