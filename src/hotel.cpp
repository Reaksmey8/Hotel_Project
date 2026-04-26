#include "hotel.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

HotelInfo::HotelInfo()
    : hotelName("Grand Hotel"), totalFloors(5),
      totalRooms(50), address("123 Main Street"), phone("012-345-6789")
{
    loadFromFile();
}

string HotelInfo::getHotelName() const { return hotelName; }
int HotelInfo::getTotalFloors() const { return totalFloors; }
int HotelInfo::getTotalRooms() const { return totalRooms; }
string HotelInfo::getAddress() const { return address; }
string HotelInfo::getPhone() const { return phone; }

void HotelInfo::setHotelName(const string &n) { hotelName = n; }
void HotelInfo::setTotalFloors(int f) { totalFloors = f; }
void HotelInfo::setTotalRooms(int r) { totalRooms = r; }
void HotelInfo::setAddress(const string &a) { address = a; }
void HotelInfo::setPhone(const string &p) { phone = p; }

void HotelInfo::loadFromFile()
{
    ifstream fin("hotel_info.txt");
    if (!fin.is_open())
        return;
    getline(fin, hotelName);
    fin >> totalFloors >> totalRooms;
    fin.ignore();
    getline(fin, address);
    getline(fin, phone);
    fin.close();
}

void HotelInfo::saveToFile() const
{
    ofstream fout("hotel_info.txt");
    fout << hotelName << "\n"
         << totalFloors << "\n"
         << totalRooms << "\n"
         << address << "\n"
         << phone << "\n";
    fout.close();
}

void HotelInfo::displayInfo() const
{
    printTitle("HOTEL INFORMATION");
    Table t;
    t.add_row({"Field", "Value"});
    t[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    t.add_row({"Hotel Name", hotelName});
    t.add_row({"Total Floors", to_string(totalFloors)});
    t.add_row({"Total Rooms", to_string(totalRooms)});
    t.add_row({"Address", address});
    t.add_row({"Phone", phone});
    cout << "\n"
         << t << "\n";
    pauseScreen();
}

void HotelInfo::editInfo()
{
    printTitle("EDIT HOTEL INFORMATION");
    cout << "\n  Current name: " << hotelName << "\n";
    string n = getStringInput("New Hotel Name (Enter to keep): ");
    if (!n.empty())
        hotelName = n;

    cout << "\n  Current floors: " << totalFloors << "\n";
    string f = getStringInput("New Total Floors (Enter to keep): ");
    if (!f.empty())
        totalFloors = stoi(f);

    cout << "\n  Current rooms: " << totalRooms << "\n";
    string r = getStringInput("New Total Rooms (Enter to keep): ");
    if (!r.empty())
        totalRooms = stoi(r);

    cout << "\n  Current address: " << address << "\n";
    string a = getStringInput("New Address (Enter to keep): ");
    if (!a.empty())
        address = a;

    cout << "\n  Current phone: " << phone << "\n";
    string p = getStringInput("New Phone (Enter to keep): ");
    if (!p.empty())
        phone = p;

    saveToFile();
    printSuccess("Hotel information updated!");
    pauseScreen();
}