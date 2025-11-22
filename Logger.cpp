#include "Logger.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
using namespace std;

void logActivity(const string &line) {
    const string path = "activity_log.csv";

    // check whether header needs to be written
    bool writeHeader = false;
    {
        ifstream ifs(path);
        if (!ifs.good() || ifs.peek() == ifstream::traits_type::eof()) writeHeader = true;
    }

    ofstream ofs(path, ios::app);
    if (!ofs) return;

    if (writeHeader) {
        ofs << "Time,Message\n";
    }

    string ts = nowTimestamp();
    // quote message to preserve commas
    ofs << ts << ',' << '"' << line << '"' << "\n";
    ofs.close();
}

void logException(const std::exception &ex) {
    try {
        stringstream ss; ss << "Exception: " << ex.what();
        logActivity(ss.str());
    } catch(...) {
        // best-effort logging, swallow any exceptions here
    }
}
