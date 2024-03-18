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
        cerr << "�޷����ļ�" << endl;
        return;
    }

    file.clear();  // ����ļ����Ĵ���/EOF���
    file.seekg(0, ios::beg);  // ���ļ�ָ���ƶ����ļ���ʼ

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
                    // ȡ����
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
                        cout << (j - 1000) / 100 + 1 << "��: " << temp[0][0];
                        //FFT_2D���ڼ��ضˣ�ƽ�������߳�ʱ��
                        temp = FFT_2D(temp);

                        unique_lock<mutex> lock(mtx);
                        dataQueue.push(move(temp));
                        cv.notify_all();    //֪ͨ���п����߳�

                        cout << "\t\tʣ��" << dataQueue.size() << "�����ݰ�������" << endl;

                        cnt -= 100;
                        temp.clear();
                    }

                    if (j >= 9600 - 100) {
                        load_flag.store(true);
                        file.close();

                        cout << "data_load �� FFD_2D���������" << "ʣ��" << dataQueue.size() << "�����ݰ�������" << endl;
                        std::cout << "data_load �� FFD_2D: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
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

            // �������ݶ�
            for (size_t i = 0; i < velocity_index_size; i++)
                for (size_t j = 0; j < range_index_size; j++)
                    R_temp[i][j] = pow(std::abs(segment[i][j]), 2);

            CFAR(R_temp);
        }
    }

    cout << "���ݴ����������" << endl;
    std::cout << "CFAR: " << (clock() - start_time) / 1000.0 << "s" << std::endl;
}


int main() {
    
    clock_t start_time = clock();

    queue<vector<vector<Complex>>> dataQueue;
    mutex mtx;
    condition_variable cv;

    string path = "D:/�ɻ�/MATLAB-C++/FFT_CFAR_demo/data_14_tune_6.csv";  // �滻�����CSV�ļ�·��

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