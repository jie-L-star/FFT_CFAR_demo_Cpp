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

// 从CSV文件中读取数据并转换为792x1000的二维数组
vector<vector<Complex>> load_data_segment(ifstream& file, int startCol, int length) {
    vector<vector<Complex>> segment(length, vector<Complex>(792));

    file.clear();  // 清除文件流的错误/EOF标记
    file.seekg(0, ios::beg);  // 将文件指针移动到文件开始

    string line;

    //for (int i = 0; i < startCol*792; ++i) {
    //    if (!getline(file, line)) {
    //        cerr << "Error: Not enough data in CSV file" << endl;
    //        return {};  // 返回空容器
    //    }
    //}

    int op = 0, j = 0, k = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        float real_part, imag_part;
        char plus_sign, imaginary_unit;

        while (std::getline(ss, cell, ',')) {

            std::stringstream cell_ss(cell);
            cell_ss >> real_part >> plus_sign >> imag_part >> imaginary_unit;

            if ((plus_sign == '+' || plus_sign == '-') && imaginary_unit == 'i') {
                segment[j][k] = Complex(real_part, (plus_sign == '+') ? imag_part : -imag_part);
            }
            else {
                std::cerr << "Error: Invalid complex number format in CSV file" << std::endl;
                return{};  // 返回空容器
            }

            k++;
            if (k == 792) {
                k = 0;
                j++;
                if (j == length) {
                    return segment;
                }
            }
        }
    }

    return segment;
}

void enqueue_data_segments(const string& path, queue<vector<vector<Complex>>>& dataQueue, mutex& mtx, condition_variable& cv) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "无法打开文件" << endl;
        return;
    }

    int currentStartCol = 0;
    int segmentLength = 1000;  // 数据段长度固定为1000
    while (true) {
        vector<vector<Complex>> segment = load_data_segment(file, currentStartCol, segmentLength);

        {
            unique_lock<mutex> lock(mtx);
            dataQueue.push(move(segment));
            cv.notify_one();
        }

        currentStartCol += 600;  // 每次向前移动100

        if (currentStartCol >= 8600) {
            load_flag.store(true);
            break;
        }
    }

    file.close();
    cout << load_flag.load() << endl;
}


void process_data_segment(vector<vector<Complex>> &segment) {
    // 在这里处理你的数据段
    // 作为示例，我们只打印出每个数据段的第一个复数的实部和虚部
    cout << "Processing data segment, first complex number: " << real(segment[0][0]) << "+" << imag(segment[0][0]) << "i" << endl;
    vector<vector<Complex>> temp = FFT_2D(segment);

    std::vector<std::vector<float>> R_temp(velocity_index_size, std::vector<float>(range_index_size));
    for (size_t i = 0; i < velocity_index_size; i++)
        for (size_t j = 0; j < range_index_size; j++)
            R_temp[i][j] = pow(std::abs(temp[i][j]), 2);

    CFAR(R_temp);
}


void consume_data_segments(queue<vector<vector<Complex>>>& dataQueue, mutex& mtx, condition_variable& cv) {
    vector<vector<Complex>> temp;
    while (!load_flag.load()) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&dataQueue] { return !dataQueue.empty(); });

        if (!dataQueue.empty()) {
            vector<vector<Complex>> segment = move(dataQueue.front());
            dataQueue.pop();
            dataQueue.push(segment);
            lock.unlock();

            process_data_segment(segment);
        }
        //else if () {
        //    lock.unlock();
        //    break;  // 数据加载完毕标志为真，退出循环
        //}

    }
}

int main() {

    clock_t start_time = clock();

    queue<vector<vector<Complex>>> dataQueue;
    mutex mtx;
    condition_variable cv;

    string path = "D:/干活/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";  // 你的CSV文件路径

    thread producer(enqueue_data_segments, path, ref(dataQueue), ref(mtx), ref(cv));
    thread consumer(consume_data_segments, ref(dataQueue), ref(mtx), ref(cv));

    producer.join();
    consumer.join();

    std::cout << "end: " << (clock() - start_time) / 1000.0 << "s" << std::endl;

    return 0;
}
