#include "room.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace xlnt;

Room::Room(int num, RoomType t, double price, RoomStatus s, string guest, int f)
    : roomNumber(num), type(t), pricePerNight(price),
      status(s), guestName(guest), floor(f) {}

int Room::getRoomNumber() const { return roomNumber; }
RoomType Room::getType() const { return type; }
double Room::getPrice() const { return pricePerNight; }
RoomStatus Room::getStatus() const { return status; }
string Room::getGuestName() const { return guestName; }
int Room::getFloor() const { return floor; }

string Room::getTypeStr() const
{
    if (type == SINGLE)
        return "Single";
    if (type == DOUBLE_ROOM)
        return "Double";
    if (type == SUITE)
        return "Suite";
    return "VIP";
}
string Room::getStatusStr() const
{
    return (status == AVAILABLE) ? "Available" : "Booked";
}
void Room::setStatus(RoomStatus s) { status = s; }
void Room::setGuestName(const string &n) { guestName = n; }
void Room::setPrice(double p) { pricePerNight = p; }
void Room::setFloor(int f) { floor = f; }

// ════════════════════════════════════════════════════════════
//  RoomManager
// ════════════════════════════════════════════════════════════
RoomManager::RoomManager(const string &file) : filename(file)
{
    loadFromFile();
}

vector<Room> &RoomManager::getRooms() { return rooms; }

void RoomManager::loadFromFile()
{
    rooms.clear();
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
        for (auto row : ws.rows(false))
        {
            if (row[0].to_string() == "RoomNumber")
                continue;
            if (row[0].to_string().empty())
                continue;
            int num = stoi(row[0].to_string());
            RoomType type = (RoomType)stoi(row[1].to_string());
            double price = stod(row[2].to_string());
            RoomStatus status = (RoomStatus)stoi(row[3].to_string());
            string guest = row[4].to_string();
            int fl = (row[5].to_string().empty()) ? 1 : stoi(row[5].to_string());
            rooms.emplace_back(num, type, price, status, guest, fl);
        }
    }
    catch (...)
    {
        rooms.clear();
    }
}

void RoomManager::saveToFile() const
{
    workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Rooms");
    ws.cell("A1").value("RoomNumber");
    ws.cell("B1").value("Type");
    ws.cell("C1").value("PricePerNight");
    ws.cell("D1").value("Status");
    ws.cell("E1").value("GuestName");
    ws.cell("F1").value("Floor");

    int row = 2;
    for (const Room &r : rooms)
    {
        ostringstream price;
        price << fixed << setprecision(2) << r.getPrice();
        ws.cell("A" + to_string(row)).value(to_string(r.getRoomNumber()));
        ws.cell("B" + to_string(row)).value(to_string((int)r.getType()));
        ws.cell("C" + to_string(row)).value(price.str());
        ws.cell("D" + to_string(row)).value(to_string((int)r.getStatus()));
        ws.cell("E" + to_string(row)).value(r.getGuestName());
        ws.cell("F" + to_string(row)).value(to_string(r.getFloor()));
        row++;
    }
    wb.save(filename);
    printSuccess("Data saved to " + filename);
}

void RoomManager::showTable(const vector<Room> &list) const
{
    Table table;
    table.add_row({"No.", "Room#", "Floor", "Type", "Price/Night", "Status", "Guest"});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    for (int i = 0; i < (int)list.size(); i++)
    {
        const Room &r = list[i];
        ostringstream price;
        price << "$" << fixed << setprecision(2) << r.getPrice();
        string guest = r.getGuestName().empty() ? "-" : r.getGuestName();
        table.add_row({to_string(i + 1),
                       to_string(r.getRoomNumber()),
                       "Floor " + to_string(r.getFloor()),
                       r.getTypeStr(),
                       price.str(),
                       r.getStatusStr(),
                       guest});
        if (r.getStatus() == AVAILABLE)
            table[i + 1][5].format().font_color(Color::green);
        else
            table[i + 1][5].format().font_color(Color::yellow);
    }
    cout << "\n"
         << table << "\n";
    cout << "  Total: " << list.size() << " room(s)\n";
}

void RoomManager::displayAll() const
{
    printTitle("ALL ROOMS");
    if (rooms.empty())
    {
        printInfo("No rooms found.");
        pauseScreen();
        return;
    }
    const int PER_PAGE = 5;
    int total = (int)rooms.size();
    int totalPages = (total + PER_PAGE - 1) / PER_PAGE;
    int page = 1;
    while (true)
    {
        clearScreen();
        printTitle("ALL ROOMS");
        int start = (page - 1) * PER_PAGE, end = min(start + PER_PAGE, total);
        showTable(vector<Room>(rooms.begin() + start, rooms.begin() + end));
        cout << "\n  Page " << page << "/" << totalPages << "\n";
        printMenu("Navigation", {"[N] Next", "[P] Previous", "[Q] Quit"});
        string ch = getStringInput("Choice: ");
        if (ch == "N" || ch == "n")
        {
            if (page < totalPages)
                page++;
            else
                printInfo("Last page.");
        }
        else if (ch == "P" || ch == "p")
        {
            if (page > 1)
                page--;
            else
                printInfo("First page.");
        }
        else if (ch == "Q" || ch == "q")
            break;
    }
}

void RoomManager::addRoom()
{
    printTitle("ADD NEW ROOM");
    int num = getIntInput("Room Number (1-999): ", 1, 999);
    for (const Room &r : rooms)
    {
        if (r.getRoomNumber() == num)
        {
            printError("Room already exists!");
            pauseScreen();
            return;
        }
    }
    int fl = getIntInput("Floor number: ", 1, 99);
    printMenu("Room Type", {"1. Single", "2. Double", "3. Suite", "4. VIP"});
    int typeChoice = getIntInput("Select (1-4): ", 1, 4);
    int price = getIntInput("Price per night ($): ", 1, 99999);
    rooms.emplace_back(num, (RoomType)(typeChoice - 1), (double)price,
                       AVAILABLE, "", fl);
    saveToFile();
    pauseScreen();
}

void RoomManager::deleteRoom()
{
    printTitle("DELETE ROOM");
    if (rooms.empty())
    {
        printInfo("No rooms.");
        pauseScreen();
        return;
    }
    showTable(rooms);
    int num = getIntInput("Room Number to delete: ", 1, 999);
    for (auto it = rooms.begin(); it != rooms.end(); ++it)
    {
        if (it->getRoomNumber() == num)
        {
            string c = getStringInput("Sure? (yes/no): ");
            if (c == "yes" || c == "YES")
            {
                rooms.erase(it);
                saveToFile();
                printSuccess("Deleted!");
            }
            else
                printInfo("Cancelled.");
            pauseScreen();
            return;
        }
    }
    printError("Room not found.");
    pauseScreen();
}

void RoomManager::sortRooms()
{
    printTitle("SORT ROOMS");
    printMenu("Sort by", {"1. Room Number", "2. Price (low-high)",
                          "3. Price (high-low)", "4. Type", "5. Status", "6. Floor"});
    int ch = getIntInput("Select (1-6): ", 1, 6);
    if (ch == 1)
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getRoomNumber() < b.getRoomNumber(); });
    else if (ch == 2)
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getPrice() < b.getPrice(); });
    else if (ch == 3)
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getPrice() > b.getPrice(); });
    else if (ch == 4)
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getType() < b.getType(); });
    else if (ch == 5)
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getStatus() < b.getStatus(); });
    else
        sort(rooms.begin(), rooms.end(), [](const Room &a, const Room &b)
             { return a.getFloor() < b.getFloor(); });
    saveToFile();
    printSuccess("Sorted!");
    pauseScreen();
}

void RoomManager::searchRooms() const
{
    printTitle("SEARCH ROOMS");
    printMenu("Search by", {"1. Room Number", "2. Type",
                            "3. Status", "4. Guest Name", "5. Floor"});
    int ch = getIntInput("Select (1-5): ", 1, 5);
    vector<Room> results;
    if (ch == 1)
    {
        int num = getIntInput("Room Number: ", 1, 999);
        for (const Room &r : rooms)
            if (r.getRoomNumber() == num)
                results.push_back(r);
    }
    else if (ch == 2)
    {
        printMenu("Type", {"1.Single", "2.Double", "3.Suite", "4.VIP"});
        int t = getIntInput("Type: ", 1, 4);
        for (const Room &r : rooms)
            if (r.getType() == (RoomType)(t - 1))
                results.push_back(r);
    }
    else if (ch == 3)
    {
        cout << "  1.Available  2.Booked\n";
        int s = getIntInput("Status: ", 1, 2);
        RoomStatus rs = (s == 1) ? AVAILABLE : BOOKED;
        for (const Room &r : rooms)
            if (r.getStatus() == rs)
                results.push_back(r);
    }
    else if (ch == 4)
    {
        string name = getStringInput("Guest Name: ");
        string nl = name;
        transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
        for (const Room &r : rooms)
        {
            string g = r.getGuestName();
            transform(g.begin(), g.end(), g.begin(), ::tolower);
            if (g.find(nl) != string::npos)
                results.push_back(r);
        }
    }
    else
    {
        int fl = getIntInput("Floor number: ", 1, 99);
        for (const Room &r : rooms)
            if (r.getFloor() == fl)
                results.push_back(r);
    }
    if (results.empty())
        printInfo("No rooms found.");
    else
    {
        printSuccess(to_string(results.size()) + " room(s) found:");
        showTable(results);
    }
    pauseScreen();
}