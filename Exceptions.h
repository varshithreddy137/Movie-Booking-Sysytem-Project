#pragma once
#include <exception>
#include <string>

using namespace std;
class NotFoundException : public exception {
private:
    string msg_;
public:
    NotFoundException(const string &m) : msg_(m) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class OopsException : public exception {
private:
    string msg_;
public:
    OopsException(const string &m) : msg_(m) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};
