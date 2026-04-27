#include <iostream>
#include <windows.h>
#include "util.hpp"
#include "user.hpp"
#include "room.hpp"
#include "booking.hpp"
#include "hotel.hpp"
using namespace std;

void adminMenu(RoomManager &rm, BookingManager &bm, HotelInfo &hi, AuthManager &auth);
void userMenu(RoomManager &rm, BookingManager &bm, AuthManager &auth);
void showBanner(const HotelInfo &hi);

int main()
{
    // Set working directory to exe location
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    string dir = string(path).substr(0, string(path).find_last_of("\\/"));
    SetCurrentDirectoryA(dir.c_str());

    AuthManager auth;
    RoomManager rm("rooms.xlsx");
    BookingManager bm("bookings.xlsx");
    HotelInfo hi;

    while (true)
    {
        clearScreen();
        showBanner(hi);
        printMenu("MAIN MENU", {"[1] Login", "[2] Register", "[3] Exit"});
        int ch = getIntInput("Select: ", 1, 3);

        if (ch == 3)
        {
            printInfo("Goodbye!");
            break;
        }

        if (ch == 2)
        {
            clearScreen();
            bool ok = auth.registerUser();
            if (ok)
            {
                // Go straight to user menu after register
                if (auth.isAdmin())
                    adminMenu(rm, bm, hi, auth);
                else
                    userMenu(rm, bm, auth);
            }
            continue;
        }

        // ch == 1: Login
        int tries = 0;
        bool ok = false;
        while (tries < 3)
        {
            clearScreen();
            if (auth.login())
            {
                ok = true;
                break;
            }
            tries++;
            if (tries < 3)
            {
                printInfo("Attempts left: " + to_string(3 - tries));
                pauseScreen();
            }
        }
        if (!ok)
        {
            printError("Too many failed attempts!");
            pauseScreen();
            continue;
        }

        pauseScreen();
        if (auth.isAdmin())
            adminMenu(rm, bm, hi, auth);
        else
            userMenu(rm, bm, auth);
    }
    return 0;
}

void adminMenu(RoomManager &rm, BookingManager &bm, HotelInfo &hi, AuthManager &auth)
{
    while (true)
    {
        clearScreen();
        rm.loadFromFile();
        bm.loadFromFile();
        printMenu("ADMIN MENU [ " + auth.getUsername() + " ]", {"[1] View All Rooms", "[2] Add Room",
                                                                "[3] Delete Room", "[4] Sort Rooms",
                                                                "[5] Search Rooms", "[6] View All Bookings",
                                                                "[7] Revenue Report", "[8] Hotel Information",
                                                                "[9] Edit Hotel Info", "[0] Logout"});
        int ch = getIntInput("Select: ", 0, 9);
        clearScreen();
        if (ch == 1)
            rm.displayAll();
        else if (ch == 2)
            rm.addRoom();
        else if (ch == 3)
            rm.deleteRoom();
        else if (ch == 4)
            rm.sortRooms();
        else if (ch == 5)
            rm.searchRooms();
        else if (ch == 6)
            bm.viewAllBookings();
        else if (ch == 7)
            bm.viewRevenue();
        else if (ch == 8)
            hi.displayInfo();
        else if (ch == 9)
            hi.editInfo();
        else
        {
            auth.logout();
            break;
        }
    }
}

void userMenu(RoomManager &rm, BookingManager &bm, AuthManager &auth)
{
    while (true)
    {
        clearScreen();
        rm.loadFromFile();
        bm.loadFromFile();
        printMenu("USER MENU [ " + auth.getUsername() + " ]", {"[1] View Available Rooms",
                                                               "[2] Book a Room",
                                                               "[3] Check Out",
                                                               "[4] My Booking History",
                                                               "[5] Search Rooms",
                                                               "[6] Logout"});
        int ch = getIntInput("Select: ", 1, 6);
        clearScreen();
        if (ch == 1)
            rm.displayAll();
        else if (ch == 2)
            bm.bookRoom(auth.getUsername());
        else if (ch == 3)
            bm.checkOut(auth.getUsername());
        else if (ch == 4)
            bm.viewMyHistory(auth.getUsername());
        else if (ch == 5)
            rm.searchRooms();
        else
        {
            auth.logout();
            break;
        }
    }
}

void showBanner(const HotelInfo &hi)
{
    Table t;
    t.add_row({hi.getHotelName() + " - Management System"});
    t.add_row({"Address: " + hi.getAddress() + "  |  Phone: " + hi.getPhone()});
    t.add_row({"Floors: " + to_string(hi.getTotalFloors()) +
               "  |  Rooms: " + to_string(hi.getTotalRooms())});
    t.add_row({"Admin: admin/admin123  |  New? Press [2] to Register"});
    t[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << "\n"
         << t << "\n";
}