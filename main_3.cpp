#include <iostream>
#include <string>
#include <Windows.h>

std::string wstringToString(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        return ""; // 转换失败
    }

    std::string str(bufferSize, 0);
    if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr) == 0) {
        return ""; // 转换失败
    }

    return str;
}

int main() {
    std::wstring wstr = L"你好世界";
    std::string str = wstringToString(wstr);
    std::cout << "转换后的字符串：" << str << std::endl;
    return 0;
}
