#pragma once
#include "Seat.h"
#include <string>
#include <fstream>
using namespace std;

class Show {
private:
    int showId_;
    int movieId_;
    string time_;
    int rows_;
    int cols_;
    Seat seats_[10][10];
public:
    Show();
    Show(int sid, int mid, const string &time, int rows, int cols);
    int getShowId() const;
    int getMovieId() const;
    string getTime() const;
    int getRows() const;
    int getCols() const;

    bool isValidSeat(int r, int c) const;
    bool isSeatAvailable(int r, int c) const;
    bool bookSeat(int r, int c);
    bool cancelSeat(int r, int c);
    double seatPrice(int r, int c) const;
    int availableCount() const;
    int totalCount() const;
    void displayLayout() const;

    void saveToStream(ofstream &ofs) const;
    void loadFromStream(ifstream &ifs);
    void displaySummary() const;
};
