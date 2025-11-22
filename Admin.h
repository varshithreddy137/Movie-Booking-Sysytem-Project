#pragma once
#include <string>
#include "FileManager.h"
#include "Person.h"
using namespace std;

class Admin : public Person {
private:
    string password_;
    FileManager *fm_;
public:
    Admin(FileManager *fm);
    bool login(const string &pwd) const;
    void menu() override;
};
