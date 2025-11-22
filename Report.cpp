#include "Report.h"
#include <iostream>
#include <iomanip>
using namespace std;

void RevenueReport::generate(const FileManager &fm) {
    double totalRevenue = 0.0;
    int ticketsSold = 0;
    for (int si = 0; si < fm.showCount; ++si) {
        const Show &s = fm.shows_[si];
        for (int r = 1; r <= s.getRows(); ++r) {
            for (int c = 1; c <= s.getCols(); ++c) {
                if (!s.isSeatAvailable(r, c)) {
                    ++ticketsSold;
                    totalRevenue += s.seatPrice(r, c);
                }
            }
        }
    }
    cout << "\n=== Revenue Summary ===\n";
    cout << "Total Tickets Sold: " << ticketsSold << "\n";
    cout << "Total Revenue: Rs. " << fixed << setprecision(2) << totalRevenue << "\n";
}

void SeatReport::generate(const FileManager &fm) {
    cout << "\n=== Seat Availability per Show ===\n";
    for (int i=0;i<fm.showCount;++i) {
        const Show &s = fm.shows_[i];
        cout << "ShowID: " << s.getShowId() << " MovieID: " << s.getMovieId()
             << " Time: " << s.getTime() << " -> Available: " << s.availableCount()
             << " / " << s.totalCount() << "\n";
    }
}
