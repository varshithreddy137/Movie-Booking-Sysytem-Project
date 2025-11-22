#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
using namespace std;

inline string nowTimestamp() {
    time_t t = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", localtime(&t));
    return string(buf);
}

// Small template utilities to show template usage
template<typename T>
T maxOfTwo(const T &a, const T &b) {
    return (a > b) ? a : b;
}

template<typename T>
void printArray(const T *arr, int n) {
    for (int i = 0; i < n; ++i) {
        // rely on << being defined for T
        std::cout << arr[i];
        if (i < n-1) std::cout << ", ";
    }
    std::cout << std::endl;
}
