#include <iostream>
#include <string>
#include <Windows.h>

std::string wstringToString(const std::wstring& wstr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0) {
        return ""; // ת��ʧ��
    }

    std::string str(bufferSize, 0);
    if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr) == 0) {
        return ""; // ת��ʧ��
    }

    return str;
}

int main() {
    std::wstring wstr = L"�������";
    std::string str = wstringToString(wstr);
    std::cout << "ת������ַ�����" << str << std::endl;
    return 0;
}
