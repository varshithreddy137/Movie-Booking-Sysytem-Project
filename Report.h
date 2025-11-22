#pragma once
#include "FileManager.h"
using namespace std;

class ReportBase {
public:
    virtual ~ReportBase() {}
    virtual void generate(const FileManager &fm) = 0;
};

class RevenueReport : public ReportBase {
public:
    void generate(const FileManager &fm) override;
};

class SeatReport : public ReportBase {
public:
    void generate(const FileManager &fm) override;
};
