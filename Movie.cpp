#include "Movie.h"
#include <iostream>
#include <iomanip>
using namespace std;

Movie::Movie() : id_(0), name_(), genre_() {}
Movie::Movie(int id, const string &name, const string &genre)
    : id_(id), name_(name), genre_(genre) {}
int Movie::getId() const { return id_; }
string Movie::getName() const { return name_; }
string Movie::getGenre() const { return genre_; }
void Movie::displaySummary() const {
    cout << left << setw(6) << id_ << setw(30) << name_ << setw(15) << genre_ << "\n";
}
bool operator==(const Movie &a, const Movie &b) { return a.id_ == b.id_; }
bool operator<(const Movie &a, const Movie &b) { return a.name_ < b.name_; }
