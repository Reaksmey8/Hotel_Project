#include "booking.hpp"
#include "room.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace xlnt;

// ── Helper: get today's date ──────────────────────────────────
static string getToday()
{
    ::time_t t = ::time(nullptr);
    struct tm *now = ::localtime(&t);
    ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << setw(2) << setfill('0') << (now->tm_mon + 1) << "-"
        << setw(2) << setfill('0') << now->tm_mday;
    return oss.str();
}

// ════════════════════════════════════════════════════════════
//  Booking
// ════════════════════════════════════════════════════════════
Booking::Booking(int id, string guest, string uname, int roomNum,
                 string rType, double price, string checkIn,
                 string checkOut, int n)
    : bookingId(id), guestName(guest), username(uname),
      roomNumber(roomNum), roomType(rType), pricePerNight(price),
      checkInDate(checkIn), checkOutDate(checkOut),
      nights(n), totalAmount(price * n), status(CHECKED_IN) {}

int Booking::getBookingId() const { return bookingId; }
string Booking::getGuestName() const { return guestName; }
string Booking::getUsername() const { return username; }
int Booking::getRoomNumber() const { return roomNumber; }
string Booking::getRoomType() const { return roomType; }
double Booking::getPricePerNight() const { return pricePerNight; }
string Booking::getCheckInDate() const { return checkInDate; }
string Booking::getCheckOutDate() const { return checkOutDate; }
int Booking::getNights() const { return nights; }
double Booking::getTotalAmount() const { return totalAmount; }
BookingStatus Booking::getStatus() const { return status; }

string Booking::getStatusStr() const
{
    if (status == CHECKED_IN)
        return "Checked In";
    if (status == CHECKED_OUT)
        return "Checked Out";
    return "Cancelled";
}

void Booking::setStatus(BookingStatus s) { status = s; }
void Booking::setCheckOutDate(const string &d) { checkOutDate = d; }
void Booking::setNights(int n) { nights = n; }
void Booking::setTotalAmount(double a) { totalAmount = a; }

// ════════════════════════════════════════════════════════════
//  BookingManager
// ════════════════════════════════════════════════════════════
BookingManager::BookingManager(const string &file)
    : filename(file), nextId(1)
{
    loadFromFile();
}

void BookingManager::loadFromFile()
{
    bookings.clear();
    ifstream check(filename);
    if (!check.good())
    {
        check.close();
        return;
    }
    check.close();

    workbook wb;
    try
    {
        wb.load(filename);
        worksheet ws = wb.active_sheet();
        int maxId = 0;
        for (auto row : ws.rows(false))
        {
            if (row[0].to_string() == "BookingID")
                continue;
            if (row[0].to_string().empty())
                continue;

            int id = stoi(row[0].to_string());
            string guest = row[1].to_string();
            string uname = row[2].to_string();
            int roomNum = stoi(row[3].to_string());
            string rType = row[4].to_string();
            double price = stod(row[5].to_string());
            string checkIn = row[6].to_string();
            string checkOut = row[7].to_string();
            int nights = stoi(row[8].to_string());

            Booking b(id, guest, uname, roomNum, rType,
                      price, checkIn, checkOut, nights);
            b.setTotalAmount(stod(row[9].to_string()));

            int st = stoi(row[10].to_string());
            b.setStatus((BookingStatus)st);

            bookings.push_back(b);
            maxId = max(maxId, id);
        }
        nextId = maxId + 1;
    }
    catch (...)
    {
        bookings.clear();
    }
}

void BookingManager::saveToFile() const
{
    workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Bookings");

    // Header
    ws.cell("A1").value("BookingID");
    ws.cell("B1").value("GuestName");
    ws.cell("C1").value("Username");
    ws.cell("D1").value("RoomNumber");
    ws.cell("E1").value("RoomType");
    ws.cell("F1").value("PricePerNight");
    ws.cell("G1").value("CheckInDate");
    ws.cell("H1").value("CheckOutDate");
    ws.cell("I1").value("Nights");
    ws.cell("J1").value("TotalAmount");
    ws.cell("K1").value("Status");

    int row = 2;
    for (const Booking &b : bookings)
    {
        ws.cell("A" + to_string(row)).value(to_string(b.getBookingId()));
        ws.cell("B" + to_string(row)).value(b.getGuestName());
        ws.cell("C" + to_string(row)).value(b.getUsername());
        ws.cell("D" + to_string(row)).value(to_string(b.getRoomNumber()));
        ws.cell("E" + to_string(row)).value(b.getRoomType());
        ostringstream price;
        price << fixed << setprecision(2) << b.getPricePerNight();
        ws.cell("F" + to_string(row)).value(price.str());
        ws.cell("G" + to_string(row)).value(b.getCheckInDate());
        ws.cell("H" + to_string(row)).value(b.getCheckOutDate());
        ws.cell("I" + to_string(row)).value(to_string(b.getNights()));
        ostringstream total;
        total << fixed << setprecision(2) << b.getTotalAmount();
        ws.cell("J" + to_string(row)).value(total.str());
        ws.cell("K" + to_string(row)).value(to_string((int)b.getStatus()));
        row++;
    }
    wb.save(filename);
}

// ── Show Booking Table ────────────────────────────────────────
void BookingManager::showBookingTable(const vector<Booking> &list) const
{
    if (list.empty())
    {
        printInfo("No bookings found.");
        return;
    }
    Table table;
    table.add_row({"ID", "Guest", "Room", "Type", "Check-In",
                   "Check-Out", "Nights", "Total($)", "Status"});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});

    for (const Booking &b : list)
    {
        ostringstream total;
        total << fixed << setprecision(2) << b.getTotalAmount();
        table.add_row({to_string(b.getBookingId()),
                       b.getGuestName(),
                       to_string(b.getRoomNumber()),
                       b.getRoomType(),
                       b.getCheckInDate(),
                       b.getCheckOutDate().empty() ? "-" : b.getCheckOutDate(),
                       to_string(b.getNights()),
                       "$" + total.str(),
                       b.getStatusStr()});
    }
    cout << "\n"
         << table << "\n";
}

// ── Book a Room ───────────────────────────────────────────────
void BookingManager::bookRoom(const string &username)
{
    printTitle("BOOK A ROOM");

    // Load available rooms to show user
    RoomManager rm("rooms.xlsx");
    vector<Room> available;
    for (auto &r : rm.getRooms())
    {
        if (r.getStatus() == AVAILABLE && !isRoomBooked(r.getRoomNumber()))
            available.push_back(r);
    }

    if (available.empty())
    {
        printInfo("Sorry! No rooms available right now.");
        pauseScreen();
        return;
    }

    // Show available rooms
Table table;
table.add_row({"Room#", "Type", "Price/Night", "Floor"});
table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
for (auto &r : available)
{
    ostringstream price;
    price << "$" << fixed << setprecision(2) << r.getPrice();
    string floor = "Floor " + to_string((r.getRoomNumber() / 100));
    table.add_row({to_string(r.getRoomNumber()),
                   r.getTypeStr(),
                   price.str(),
                   floor});
}
cout << "\n" << table << "\n";

    // Get booking details
    int roomNum = getIntInput("Enter Room Number to book: ", 1, 999);

    // Check room exists and available
    Room *selectedRoom = nullptr;
    for (auto &r : available)
    {
        if (r.getRoomNumber() == roomNum)
        {
            selectedRoom = &r;
            break;
        }
    }
    if (!selectedRoom)
    {
        printError("Room not available or does not exist!");
        pauseScreen();
        return;
    }

    string guestName = getStringInput("Guest Name: ");
    int nights = getIntInput("Number of nights: ", 1, 365);

    string checkIn = getToday();
    double total = selectedRoom->getPrice() * nights;

    // Show booking summary
    printTitle("BOOKING SUMMARY");
    Table summary;
    summary.add_row({"Field", "Details"});
    summary[0].format().font_style({FontStyle::bold});
    summary.add_row({"Guest Name", guestName});
    summary.add_row({"Room Number", to_string(roomNum)});
    summary.add_row({"Room Type", selectedRoom->getTypeStr()});
    summary.add_row({"Check-In Date", checkIn});
    summary.add_row({"Nights", to_string(nights)});
    ostringstream price, totalStr;
    price << "$" << fixed << setprecision(2) << selectedRoom->getPrice();
    totalStr << "$" << fixed << setprecision(2) << total;
    summary.add_row({"Price/Night", price.str()});
    summary.add_row({"Total Amount", totalStr.str()});
    cout << "\n"
         << summary << "\n";

    string confirm = getStringInput("Confirm booking? (yes/no): ");
    if (confirm != "yes" && confirm != "YES")
    {
        printInfo("Booking cancelled.");
        pauseScreen();
        return;
    }

    // Create booking
    Booking b(nextId++, guestName, username, roomNum,
              selectedRoom->getTypeStr(), selectedRoom->getPrice(),
              checkIn, "", nights);
    b.setTotalAmount(total);
    bookings.push_back(b);
    saveToFile();

    // Update room status
    for (auto &r : rm.getRooms())
    {
        if (r.getRoomNumber() == roomNum)
        {
            r.setStatus(BOOKED);
            r.setGuestName(guestName);
        }
    }
    rm.saveToFile();

    printSuccess("Room " + to_string(roomNum) + " booked successfully!");
    printSuccess("Total amount: " + totalStr.str());
    pauseScreen();
}

// ── Check Out ─────────────────────────────────────────────────
void BookingManager::checkOut(const string &username)
{
    printTitle("CHECK OUT");

    // Find active bookings for this user
    vector<Booking *> myBookings;
    for (auto &b : bookings)
    {
        if (b.getUsername() == username && b.getStatus() == CHECKED_IN)
            myBookings.push_back(&b);
    }

    if (myBookings.empty())
    {
        printInfo("You have no active bookings.");
        pauseScreen();
        return;
    }

    // Show active bookings
    Table table;
    table.add_row({"ID", "Room", "Type", "Check-In", "Nights", "Total($)"});
    table[0].format().font_style({FontStyle::bold});
    for (auto *b : myBookings)
    {
        ostringstream total;
        total << fixed << setprecision(2) << b->getTotalAmount();
        table.add_row({to_string(b->getBookingId()),
                       to_string(b->getRoomNumber()),
                       b->getRoomType(),
                       b->getCheckInDate(),
                       to_string(b->getNights()),
                       "$" + total.str()});
    }
    cout << "\n"
         << table << "\n";

    int id = getIntInput("Enter Booking ID to check out: ", 1, 99999);

    Booking *target = nullptr;
    for (auto &b : bookings)
    {
        if (b.getBookingId() == id && b.getUsername() == username && b.getStatus() == CHECKED_IN)
        {
            target = &b;
            break;
        }
    }

    if (!target)
    {
        printError("Booking not found!");
        pauseScreen();
        return;
    }

    // Show checkout summary
    ostringstream total;
    total << fixed << setprecision(2) << target->getTotalAmount();
    printTitle("CHECKOUT SUMMARY");
    Table summary;
    summary.add_row({"Field", "Details"});
    summary[0].format().font_style({FontStyle::bold});
    summary.add_row({"Guest Name", target->getGuestName()});
    summary.add_row({"Room Number", to_string(target->getRoomNumber())});
    summary.add_row({"Check-In", target->getCheckInDate()});
    summary.add_row({"Check-Out", getToday()});
    summary.add_row({"Nights", to_string(target->getNights())});
    summary.add_row({"Total Amount", "$" + total.str()});
    cout << "\n"
         << summary << "\n";

    string confirm = getStringInput("Confirm checkout? (yes/no): ");
    if (confirm != "yes" && confirm != "YES")
    {
        printInfo("Cancelled.");
        pauseScreen();
        return;
    }

    target->setStatus(CHECKED_OUT);
    target->setCheckOutDate(getToday());
    saveToFile();

    // Free the room
    RoomManager rm("rooms.xlsx");
    for (auto &r : rm.getRooms())
    {
        if (r.getRoomNumber() == target->getRoomNumber())
        {
            r.setStatus(AVAILABLE);
            r.setGuestName("");
        }
    }
    rm.saveToFile();

    printSuccess("Checked out successfully!");
    printSuccess("Total paid: $" + total.str());
    pauseScreen();
}

// ── View My History ───────────────────────────────────────────
void BookingManager::viewMyHistory(const string &username) const
{
    printTitle("MY BOOKING HISTORY");
    vector<Booking> myBookings;
    for (const auto &b : bookings)
        if (b.getUsername() == username)
            myBookings.push_back(b);

    if (myBookings.empty())
    {
        printInfo("No booking history found.");
        pauseScreen();
        return;
    }
    showBookingTable(myBookings);

    // Show total spent
    double totalSpent = 0;
    for (auto &b : myBookings)
        if (b.getStatus() == CHECKED_OUT)
            totalSpent += b.getTotalAmount();
    ostringstream ts;
    ts << fixed << setprecision(2) << totalSpent;
    cout << "\n  Total spent: $" << ts.str() << "\n";
    pauseScreen();
}

// ── Admin: View All Bookings ──────────────────────────────────
void BookingManager::viewAllBookings() const
{
    printTitle("ALL BOOKINGS");
    showBookingTable(bookings);
    pauseScreen();
}

// ── Admin: Revenue Report ─────────────────────────────────────
void BookingManager::viewRevenue() const
{
    printTitle("REVENUE REPORT");

    double totalRevenue = 0;
    int totalBookings = 0;
    int activeBookings = 0;

    for (const auto &b : bookings)
    {
        if (b.getStatus() == CHECKED_OUT)
        {
            totalRevenue += b.getTotalAmount();
            totalBookings++;
        }
        if (b.getStatus() == CHECKED_IN)
            activeBookings++;
    }

    ostringstream rev;
    rev << fixed << setprecision(2) << totalRevenue;

    Table t;
    t.add_row({"Report", "Value"});
    t[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    t.add_row({"Total Revenue", "$" + rev.str()});
    t.add_row({"Completed Bookings", to_string(totalBookings)});
    t.add_row({"Active Bookings", to_string(activeBookings)});
    t.add_row({"Total Bookings", to_string((int)bookings.size())});
    cout << "\n"
         << t << "\n";
    pauseScreen();
}

// ── Check if room is booked ───────────────────────────────────
bool BookingManager::isRoomBooked(int roomNum) const
{
    for (const auto &b : bookings)
        if (b.getRoomNumber() == roomNum && b.getStatus() == CHECKED_IN)
            return true;
    return false;
}

vector<Booking> &BookingManager::getBookings() { return bookings; }