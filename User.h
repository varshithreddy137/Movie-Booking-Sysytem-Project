#pragma once
#include <string>
#include "FileManager.h"
#include "Person.h"
using namespace std;

class User : public Person {
private:
    FileManager *fm_;
public:
    User(int uid, const string &name, FileManager *fm);
    void menu() override;
};
