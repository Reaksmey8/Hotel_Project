#pragma once
#include <string>
#include <vector>
using namespace std;

// ── Hotel Info ────────────────────────────────────────────────
class HotelInfo
{
private:
    string hotelName;
    int totalFloors;
    int totalRooms;
    string address;
    string phone;

public:
    HotelInfo();

    string getHotelName() const;
    int getTotalFloors() const;
    int getTotalRooms() const;
    string getAddress() const;
    string getPhone() const;

    void setHotelName(const string &name);
    void setTotalFloors(int floors);
    void setTotalRooms(int rooms);
    void setAddress(const string &addr);
    void setPhone(const string &phone);

    void loadFromFile();
    void saveToFile() const;
    void displayInfo() const;
    void editInfo();
};