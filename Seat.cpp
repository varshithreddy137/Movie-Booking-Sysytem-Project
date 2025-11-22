#include "Seat.h"
using namespace std;

Seat::Seat() : booked_(false), type_(REGULAR) {}
Seat::Seat(SeatType t) : booked_(false), type_(t) {}
bool Seat::isBooked() const { return booked_; }
void Seat::book() { booked_ = true; }
void Seat::cancel() { booked_ = false; }
SeatType Seat::getType() const { return type_; }
double Seat::price() const { return (type_ == PREMIUM) ? 250.0 : 150.0; }
char Seat::symbol() const {
    if (booked_) return 'X';
    return (type_ == PREMIUM) ? 'P' : 'R';
}
