#pragma once
#include <string>
#include <vector>
using namespace std;

enum RoomType
{
    SINGLE,
    DOUBLE_ROOM,
    SUITE,
    VIP
};
enum RoomStatus
{
    AVAILABLE,
    BOOKED
};

class Room
{
private:
    int roomNumber;
    RoomType type;
    double pricePerNight;
    RoomStatus status;
    string guestName;
    int floor;

public:
    Room(int num, RoomType t, double price,
         RoomStatus s = AVAILABLE, string guest = "", int floor = 1);

    int getRoomNumber() const;
    RoomType getType() const;
    double getPrice() const;
    RoomStatus getStatus() const;
    string getGuestName() const;
    string getTypeStr() const;
    string getStatusStr() const;
    int getFloor() const;

    void setStatus(RoomStatus s);
    void setGuestName(const string &name);
    void setPrice(double p);
    void setFloor(int f);
};

class RoomManager
{
private:
    vector<Room> rooms;
    string filename;
    void showTable(const vector<Room> &list) const;

public:
    RoomManager(const string &file = "rooms.xlsx");

    void loadFromFile();
    void saveToFile() const;

    void addRoom();
    void deleteRoom();
    void displayAll() const;
    void sortRooms();
    void searchRooms() const;

    vector<Room> &getRooms(); // needed by BookingManager
};