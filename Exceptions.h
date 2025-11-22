#pragma once
#include <exception>
#include <string>

class NotFoundException : public std::exception {
private:
    std::string msg_;
public:
    NotFoundException(const std::string &m) : msg_(m) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class OopsException : public std::exception {
private:
    std::string msg_;
public:
    OopsException(const std::string &m) : msg_(m) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};
