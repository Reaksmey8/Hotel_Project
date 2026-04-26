#pragma once
#include <string>
#include <vector>
using namespace std;

// ── Booking Status ────────────────────────────────────────────
enum BookingStatus
{
    CHECKED_IN,
    CHECKED_OUT,
    CANCELLED
};

// ── Booking Record ────────────────────────────────────────────
class Booking
{
private:
    int bookingId;
    string guestName;
    string username; // who booked it
    int roomNumber;
    string roomType;
    double pricePerNight;
    string checkInDate;
    string checkOutDate;
    int nights;
    double totalAmount;
    BookingStatus status;

public:
    Booking(int id, string guest, string uname, int roomNum,
            string rType, double price, string checkIn,
            string checkOut, int nights);

    // Getters
    int getBookingId() const;
    string getGuestName() const;
    string getUsername() const;
    int getRoomNumber() const;
    string getRoomType() const;
    double getPricePerNight() const;
    string getCheckInDate() const;
    string getCheckOutDate() const;
    int getNights() const;
    double getTotalAmount() const;
    BookingStatus getStatus() const;
    string getStatusStr() const;

    void setStatus(BookingStatus s);
    void setCheckOutDate(const string &d);
    void setNights(int n);
    void setTotalAmount(double a);
};

// ── Booking Manager ───────────────────────────────────────────
class BookingManager
{
private:
    vector<Booking> bookings;
    string filename;
    int nextId;

    void showBookingTable(const vector<Booking> &list) const;

public:
    BookingManager(const string &file = "bookings.xlsx");

    void loadFromFile();
    void saveToFile() const;

    // User actions
    void bookRoom(const string &username);
    void checkOut(const string &username);
    void viewMyHistory(const string &username) const;

    // Admin actions
    void viewAllBookings() const;
    void viewRevenue() const;

    // Helper
    bool isRoomBooked(int roomNum) const;
    vector<Booking> &getBookings();
};