#include <iostream>
#include "util.hpp"
#include "user.hpp"
#include "room.hpp"
#include "booking.hpp"
#include "hotel.hpp"
using namespace std;

void adminMenu(RoomManager &rm, BookingManager &bm,
               HotelInfo &hi, AuthManager &auth);
void userMenu(RoomManager &rm, BookingManager &bm, AuthManager &auth);
void showWelcomeBanner(const HotelInfo &hi);

int main()
{
    AuthManager auth;
    RoomManager rm("rooms.xlsx");
    BookingManager bm("bookings.xlsx");
    HotelInfo hi;

    while (true)
    {
        clearScreen();
        showWelcomeBanner(hi);
        printMenu("MAIN MENU", {"[1] Login", "[2] Exit"});
        int choice = getIntInput("Select: ", 1, 2);

        if (choice == 2)
        {
            printInfo("Goodbye! Thank you!");
            break;
        }

        int attempts = 0;
        bool loggedIn = false;
        while (attempts < 3)
        {
            clearScreen();
            if (auth.login())
            {
                loggedIn = true;
                break;
            }
            attempts++;
            if (attempts < 3)
                printInfo("Attempts remaining: " + to_string(3 - attempts));
        }
        if (!loggedIn)
        {
            printError("Too many failed attempts.");
            pauseScreen();
            continue;
        }

        if (auth.isAdmin())
            adminMenu(rm, bm, hi, auth);
        else
            userMenu(rm, bm, auth);
    }
    return 0;
}

// ── Admin Menu ────────────────────────────────────────────────
void adminMenu(RoomManager &rm, BookingManager &bm,
               HotelInfo &hi, AuthManager &auth)
{
    while (true)
    {
        clearScreen();
        printMenu("ADMIN MENU  [ " + auth.getUsername() + " ]", {"[1] View All Rooms",
                                                                 "[2] Add  Room",
                                                                 "[3] Delete Room",
                                                                 "[4] Sort Rooms",
                                                                 "[5] Search Rooms",
                                                                 "[6] View All Bookings",
                                                                 "[7] Revenue Report",
                                                                 "[8] Hotel Information",
                                                                 "[9] Edit Hotel Info",
                                                                 "[0] Logout"});
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
            pauseScreen();
            break;
        }
    }
}

// ── User Menu ─────────────────────────────────────────────────
void userMenu(RoomManager &rm, BookingManager &bm, AuthManager &auth)
{
    while (true)
    {
        clearScreen();
        printMenu("USER MENU  [ " + auth.getUsername() + " ]", {"[1] View Available Rooms",
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
            pauseScreen();
            break;
        }
    }
}

// ── Welcome Banner ────────────────────────────────────────────
void showWelcomeBanner(const HotelInfo &hi)
{
    Table banner;
    banner.add_row({hi.getHotelName() + " - Management System"});
    banner.add_row({"Address: " + hi.getAddress()});
    banner.add_row({"Phone: " + hi.getPhone()});
    banner.add_row({"Floors: " + to_string(hi.getTotalFloors()) +
                    "  |  Total Rooms: " + to_string(hi.getTotalRooms())});
    banner.add_row({"Accounts: admin/admin123  student/stu123  teacher/tea123"});
    banner[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << "\n"
         << banner << "\n";
}