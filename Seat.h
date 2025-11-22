#pragma once
using namespace std;

enum SeatType { REGULAR, PREMIUM };

class Seat {
private:
    bool booked_;
    SeatType type_;
public:
    Seat();
    Seat(SeatType t);
    bool isBooked() const;
    void book();
    void cancel();
    SeatType getType() const;
    double price() const;
    char symbol() const;
};
