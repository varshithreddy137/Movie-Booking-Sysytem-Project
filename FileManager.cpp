#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdio>
#include "Logger.h"
#include "Utils.h"
#include "IDGenerator.h"
using namespace std;

static const char *MOVIE_FILE = "movies_db.csv";
static const char *SHOW_FILE = "shows_db.csv";
static const char *BOOKING_FILE = "bookings_db.csv";
static const char *USER_LOG = "user_log.csv";

FileManager::FileManager() : movieCount(0), showCount(0), bookingCount(0) {}

void FileManager::loadAll() {
    ifstream mif(MOVIE_FILE);
    movieCount = 0;
    if (mif) {
        string line;
        while (getline(mif, line)) {
            if (line.empty()) continue;
            try {
                stringstream ss(line);
                string tid, name, genre;
                getline(ss, tid, ','); int id = stoi(tid);
                getline(ss, name, ','); getline(ss, genre, ',');
                movies_[movieCount++] = Movie(id, name, genre);
                if (movieCount >= 50) break;
            } catch (const exception &e) {
                stringstream msg; msg << "Failed to parse movie line: '" << line << "' : " << e.what();
                logActivity(msg.str());
                continue;
            }
        }
        mif.close();
    }
    // initialize next movie id to avoid collisions with loaded data
    if (movieCount > 0) {
        int maxId = 0;
        for (int i = 0; i < movieCount; ++i) if (movies_[i].getId() > maxId) maxId = movies_[i].getId();
        setNextMovieId(maxId + 1);
    }
    ifstream sif(SHOW_FILE);
    showCount = 0;
    if (sif) {
        while (!sif.eof()) {
            try {
                Show s;
                s.loadFromStream(sif);
                if (s.getShowId() != 0) {
                    shows_[showCount++] = s;
                    if (showCount >= 100) break;
                } else break;
            } catch (const exception &e) {
                string line; // attempt to read/skip one header line
                getline(sif, line);
                stringstream msg; msg << "Failed to load show: " << e.what();
                logActivity(msg.str());
                continue;
            }
        }
        sif.close();
    }
    // initialize next show id to avoid collisions with loaded data
    if (showCount > 0) {
        int maxId = 0;
        for (int i = 0; i < showCount; ++i) if (shows_[i].getShowId() > maxId) maxId = shows_[i].getShowId();
        setNextShowId(maxId + 1);
    }
    ifstream bif(BOOKING_FILE);
    bookingCount = 0;
    if (bif) {
        string line;
        while (getline(bif, line)) {
            if (line.empty()) continue;
            try {
                Booking b;
                b.loadFromString(line);
                bookings_[bookingCount++] = b;
                if (bookingCount >= 500) break;
            } catch (const exception &e) {
                stringstream msg; msg << "Skipping malformed booking line: '" << line << "' : " << e.what();
                logActivity(msg.str());
                continue;
            }
        }
        bif.close();
    }
}

void FileManager::saveAll() {
    try {
        ofstream mof(MOVIE_FILE);
        if (!mof) throw runtime_error("Cannot open movie file for writing");
        for (int i=0;i<movieCount;++i)
            mof << movies_[i].getId() << "," << movies_[i].getName() << "," << movies_[i].getGenre() << "\n";
        mof.close();

        ofstream sof(SHOW_FILE);
        if (!sof) throw runtime_error("Cannot open show file for writing");
        for (int i=0;i<showCount;++i) shows_[i].saveToStream(sof);
        sof.close();

        ofstream bof(BOOKING_FILE);
        if (!bof) throw runtime_error("Cannot open booking file for writing");
        for (int i=0;i<bookingCount;++i) bookings_[i].saveToStream(bof);
        bof.close();
    } catch (const exception &e) {
        stringstream ss; ss << "Failed to save data: " << e.what();
        logActivity(ss.str());
        throw; // let caller handle
    }
}

bool FileManager::addMovie(const Movie &m) {
    if (movieCount >= 50) return false;
    movies_[movieCount++] = m;
    stringstream ss; ss << "Added movie: " << m.getId() << " - " << m.getName();
    logActivity(ss.str());
    return true;
}
int FileManager::findMovieIndexById(int mid) const {
    for (int i=0;i<movieCount;++i) if (movies_[i].getId() == mid) return i;
    return -1;
}
void FileManager::listMovies() const {
    if (movieCount == 0) { cout << "No movies available.\n"; return; }
    cout << "\nMovies:\n" << left << setw(6) << "ID" << setw(30) << "Name" << setw(15) << "Genre" << "\n";
    for (int i=0;i<movieCount;++i) movies_[i].displaySummary();
}
void FileManager::searchMoviesByName(const string &keyword) const {
    bool found = false;
    for (int i=0;i<movieCount;++i) {
        string name = movies_[i].getName();
        if (name.find(keyword) != string::npos) {
            if (!found) cout << "Search results:\n";
            found = true; movies_[i].displaySummary();
        }
    }
    if (!found) cout << "No movies match '" << keyword << "'.\n";
}
void FileManager::sortMoviesByName() {
    for (int i=0;i<movieCount-1;++i)
        for (int j=0;j<movieCount-1-i;++j)
            if (movies_[j].getName() > movies_[j+1].getName()) {
                Movie tmp = movies_[j];
                movies_[j] = movies_[j+1];
                movies_[j+1] = tmp;
            }
    logActivity("Sorted movies by name");
}

bool FileManager::addShow(const Show &s) {
    if (showCount >= 100) return false;
    for (int i=0;i<showCount;++i) if (shows_[i].getShowId() == s.getShowId()) return false;
    shows_[showCount++] = s;
    stringstream ss; ss << "Added show: " << s.getShowId() << " for movie " << s.getMovieId();
    logActivity(ss.str());
    return true;
}
int FileManager::findShowIndexById(int sid) const {
    for (int i=0;i<showCount;++i) if (shows_[i].getShowId() == sid) return i;
    return -1;
}
void FileManager::listShows() const {
    if (showCount == 0) { cout << "No shows available.\n"; return; }
    cout << "\nShows Summary:\n";
    cout << left << setw(8) << "ShowID" << setw(30) << "Movie" << setw(12) << "Time" << setw(12) << "Available" << "\n";
    for (int i=0;i<showCount;++i) {
        const Show &s = shows_[i];
        int mid = s.getMovieId();
        string mname = "<unknown>";
        int midx = findMovieIndexById(mid);
        if (midx != -1) mname = movies_[midx].getName();
        string avail = to_string(s.availableCount()) + "/" + to_string(s.totalCount());
        cout << left << setw(8) << s.getShowId() << setw(30) << mname << setw(12) << s.getTime() << setw(12) << avail << "\n";
    }
}

bool FileManager::addBooking(const Booking &b) {
    if (bookingCount >= 500) return false;
    bookings_[bookingCount++] = b;
    stringstream ss; ss << "Booking added: " << b.getBookingId() << " user " << b.getUserId() << " show " << b.getShowId();
    logActivity(ss.str());
    return true;
}
int FileManager::findBookingIndexById(int bid) const {
    for (int i=0;i<bookingCount;++i) if (bookings_[i].getBookingId() == bid) return i;
    return -1;
}
void FileManager::listBookingsForUser(int userId, const string &userName) const {
    bool any = false;
    for (int i=0;i<bookingCount;++i) if (bookings_[i].getUserId() == userId) { bookings_[i].display(); any = true; }
    if (!any) {
        // Try to find bookings from receipts.csv (in case bookings_db is empty or out-of-sync)
        const string rpath = "receipts.csv";
        ifstream rif(rpath);
        if (rif) {
            string header;
            if (getline(rif, header)) {
                string line;
                bool foundInReceipts = false;
                auto parseCsv = [](const string &s) {
                    vector<string> out; string cur; bool inQuotes = false;
                    for (size_t i = 0; i < s.size(); ++i) {
                        char ch = s[i];
                        if (ch == '"') { inQuotes = !inQuotes; continue; }
                        if (ch == ',' && !inQuotes) { out.push_back(cur); cur.clear(); }
                        else cur.push_back(ch);
                    }
                    out.push_back(cur);
                    return out;
                };

                while (getline(rif, line)) {
                    if (line.empty()) continue;
                    vector<string> toks = parseCsv(line);
                    if (toks.size() < 3) continue;
                    int uid = 0;
                    try { uid = stoi(toks[1]); } catch(...) { continue; }
                    string uname = toks[2];
                    if (uid == userId && uname == userName) {
                        // tokens: Booking Id,User Id,User Name,Show Id,Seats,Amount,No of tickets,Time stamp
                        cout << "Booking ID: " << toks[0] << " | ShowID: " << (toks.size()>3? toks[3] : "")
                             << " | Seats: " << (toks.size()>4? toks[4] : "")
                             << " | Amount: " << (toks.size()>5? toks[5] : "")
                             << " | Tickets: " << (toks.size()>6? toks[6] : "1")
                             << " | Time: " << (toks.size()>7? toks[7] : "") << "\n";
                        foundInReceipts = true; any = true;
                    }
                }
                if (!foundInReceipts) cout << "No bookings found for user " << userId << "\n";
            } else {
                cout << "No bookings found for user " << userId << "\n";
            }
            rif.close();
        } else {
            cout << "No bookings found for user " << userId << "\n";
        }
    }
}
void FileManager::removeBookingAtIndex(int idx) {
    if (idx < 0 || idx >= bookingCount) return;
    stringstream ss; ss << "Removing booking: " << bookings_[idx].getBookingId(); logActivity(ss.str());
    for (int i=idx;i<bookingCount-1;++i) bookings_[i] = bookings_[i+1];
    --bookingCount;
}

int FileManager::removeBookingsByBookingId(int bookingId, int userId, double &refund) {
    refund = 0.0;
    int removed = 0;
    for (int i = 0; i < bookingCount; ) {
        if (bookings_[i].getBookingId() == bookingId && bookings_[i].getUserId() == userId) {
            // release seat in show if present
            int sid = bookings_[i].getShowId();
            int r = bookings_[i].getRow();
            int c = bookings_[i].getCol();
            double amt = bookings_[i].getAmount();
            int sidx = findShowIndexById(sid);
            if (sidx != -1) shows_[sidx].cancelSeat(r, c);
            refund += amt;
            // remove booking at index i by shifting
            for (int j = i; j < bookingCount - 1; ++j) bookings_[j] = bookings_[j+1];
            --bookingCount;
            ++removed;
            // do not increment i, since new element at i needs checking
        } else {
            ++i;
        }
    }
    if (removed > 0) {
        stringstream ss; ss << "Removed " << removed << " booking(s) for bookingId " << bookingId << " user " << userId;
        logActivity(ss.str());
    }
    return removed;
}

int FileManager::removeBookingsByShowId(int showId, double &refund) {
    refund = 0.0;
    int removed = 0;
    for (int i = 0; i < bookingCount; ) {
        if (bookings_[i].getShowId() == showId) {
            double amt = bookings_[i].getAmount();
            int uid = bookings_[i].getUserId();
            // try to release seat if show exists
            int sidx = findShowIndexById(showId);
            if (sidx != -1) {
                shows_[sidx].cancelSeat(bookings_[i].getRow(), bookings_[i].getCol());
            }
            refund += amt;
            for (int j = i; j < bookingCount - 1; ++j) bookings_[j] = bookings_[j+1];
            --bookingCount; ++removed;
        } else ++i;
    }
    if (removed > 0) {
        stringstream ss; ss << "Removed " << removed << " booking(s) for show " << showId << " (refund " << refund << ")";
        logActivity(ss.str());
    }
    return removed;
}

int FileManager::removeShowsByMovieId(int movieId, double &refund) {
    refund = 0.0;
    int removedShows = 0;
    for (int i = 0; i < showCount; ) {
        if (shows_[i].getMovieId() == movieId) {
            int sid = shows_[i].getShowId();
            double showRefund = 0.0;
            int removedBookings = removeBookingsByShowId(sid, showRefund);
            refund += showRefund;
            // remove show at index i
            for (int j = i; j < showCount - 1; ++j) shows_[j] = shows_[j+1];
            --showCount; ++removedShows;
            stringstream ss; ss << "Removed show " << sid << " for movie " << movieId << "; removed " << removedBookings << " bookings.";
            logActivity(ss.str());
            // do not increment i; new element now at i
        } else ++i;
    }
    if (removedShows > 0) {
        stringstream ss; ss << "Removed " << removedShows << " show(s) for movie " << movieId << "; total refund " << refund;
        logActivity(ss.str());
    }
    return removedShows;
}

bool FileManager::removeMovieById(int movieId, double &refund) {
    int idx = findMovieIndexById(movieId);
    if (idx == -1) return false;
    refund = 0.0;
    int removedShows = removeShowsByMovieId(movieId, refund);
    // shift movies array to remove movie at idx
    for (int i = idx; i < movieCount - 1; ++i) movies_[i] = movies_[i+1];
    --movieCount;
    stringstream ss; ss << "Removed movie " << movieId << " and " << removedShows << " shows; refund " << refund;
    logActivity(ss.str());
    return true;
}

void FileManager::appendUserLog(int userId, const Booking &b) {
    ofstream ofs(USER_LOG, ios::app);
    if (!ofs) return;
    ofs << userId << "," << b.getBookingId() << "," << b.getShowId() << "," << b.getRow() << "," << b.getCol() << "," << b.getAmount() << "\n";
    ofs.close();
}

void FileManager::writeReceipt(const Booking& booking, int noOfTickets, const string &seatList, const string &userName) {
    const std::string csvPath = "receipts.csv";

    // Desired header for receipts.csv
    const string newHeader = "Booking Id,User Id,User Name,Show Id,Seats,Amount,No of tickets,Time stamp";

    // Check if file exists and whether header needs upgrade
    bool writeHeader = false;
    {
        std::ifstream ifs(csvPath);
        if (!ifs.good() || ifs.peek() == std::ifstream::traits_type::eof()) {
            writeHeader = true;
        } else {
            string first;
            getline(ifs, first);
            // If existing header does not contain our new columns, attempt a safe upgrade
            if (first.find("No of tickets") == string::npos || first.find("Seats") == string::npos || first.find("User Name") == string::npos) {
                // read remaining lines
                vector<string> oldLines;
                string line;
                while (getline(ifs, line)) {
                    if (!line.empty()) oldLines.push_back(line);
                }
                ifs.close();

                // write to temp file with new header and transformed old lines
                string tmpPath = csvPath + ".tmp";
                ofstream tmp(tmpPath);
                if (tmp) {
                    tmp << newHeader << "\n";
                    for (const auto &l : oldLines) {
                        // attempt to parse old rows which were in format: BookingId,UserId,ShowId,SeatRow,SeatCol,Amount,Time
                        stringstream ss(l);
                        vector<string> toks;
                        string tok;
                        while (getline(ss, tok, ',')) toks.push_back(tok);
                        if (toks.size() >= 7) {
                            string seats = "(" + toks[3] + "," + toks[4] + ")";
                            string amount = toks[5];
                            string time = toks[6];
                            // insert empty user name for older entries
                            tmp << toks[0] << ',' << toks[1] << ',' << '"' << "" << '"' << ',' << toks[2] << ','
                                << '"' << seats << '"' << ',' << amount << ',' << 1 << ',' << time << "\n";
                        } else {
                            // fallback: append ","" ,1" to preserve old line
                            tmp << l << ',' << '"' << '"' << ',' << 1 << "\n";
                        }
                    }
                    tmp.close();
                    // replace original
                    std::remove(csvPath.c_str());
                    std::rename(tmpPath.c_str(), csvPath.c_str());
                    writeHeader = false; // header now present in new file
                } else {
                    // can't create tmp, fall back to appending header at end on next open
                    writeHeader = true;
                }
            }
        }
    }

    std::ofstream ofs(csvPath, std::ios::app);
    if (!ofs) return;

    if (writeHeader) {
        ofs << newHeader << "\n";
    }

    // Build seats string: either provided seatList (for multi-seat booking) or single seat from booking
    string seatsOut = seatList;
    if (seatsOut.empty()) {
        // single-seat booking: format as "(r,c)"
        stringstream sss; sss << "(" << booking.getRow() << "," << booking.getCol() << ")";
        seatsOut = sss.str();
    }

    string timestamp = nowTimestamp();

    ofs << booking.getBookingId() << ','
        << booking.getUserId() << ','
        << '"' << userName << '"' << ','
        << booking.getShowId() << ','
        << '"' << seatsOut << '"' << ','
        << booking.getAmount() << ','
        << noOfTickets << ','
        << timestamp << "\n";
}
