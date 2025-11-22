#pragma once
#include <iostream>
using namespace std;

class Payment {
public:
    static bool process(double amount) {
        cout << "Processing payment of Rs. " << amount << " ...\n";
        cout << "Payment successful.\n";
        return true;
    }
};
