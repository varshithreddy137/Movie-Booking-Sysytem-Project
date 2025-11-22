#pragma once
#include <string>
using namespace std;

class Person {
protected:
    int id_;
    string name_;
public:
    Person() : id_(0), name_() {}
    Person(int id, const string &name) : id_(id), name_(name) {}
    virtual ~Person() {}
    virtual void menu() = 0;
};

