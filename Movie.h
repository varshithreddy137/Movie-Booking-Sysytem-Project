#pragma once
#include <string>
using namespace std;

class Movie {
private:
    int id_;
    string name_;
    string genre_;
public:
    Movie();
    Movie(int id, const string &name, const string &genre);
    int getId() const;
    string getName() const;
    string getGenre() const;
    void displaySummary() const;
    friend bool operator==(const Movie &a, const Movie &b);
    friend bool operator<(const Movie &a, const Movie &b);
};
