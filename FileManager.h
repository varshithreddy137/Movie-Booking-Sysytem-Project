#pragma once
#include "Movie.h"
#include "Show.h"
#include "Booking.h"
using namespace std;

class FileManager {
public:
    Movie movies_[50];
    Show shows_[100];
    Booking bookings_[500];
    int movieCount;
    int showCount;
    int bookingCount;

    FileManager();
    void loadAll();
    void saveAll();

    bool addMovie(const Movie &m);
    int findMovieIndexById(int mid) const;
    void listMovies() const;
    void searchMoviesByName(const string &keyword) const;
    void sortMoviesByName();

    bool addShow(const Show &s);
    int findShowIndexById(int sid) const;
    void listShows() const;

    bool addBooking(const Booking &b);
    int findBookingIndexById(int bid) const;
    void listBookingsForUser(int userId, const string &userName) const;
    void removeBookingAtIndex(int idx);
    int removeBookingsByBookingId(int bookingId, int userId, double &refund);
    int removeBookingsByShowId(int showId, double &refund);
    int removeShowsByMovieId(int movieId, double &refund);

    void appendUserLog(int userId, const Booking &b);

    void writeReceipt(const Booking &b, int noOfTickets = 1, const string &seatList = "", const string &userName = "") ;
    bool removeMovieById(int movieId, double &refund);
};

