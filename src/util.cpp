#include "util.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// ── Clear & Pause ─────────────────────────────────────────────
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printLine(char ch, int width)
{
    cout << "  +";
    for (int i = 0; i < width; i++)
        cout << ch;
    cout << "+\n";
}

// ── Title Box ─────────────────────────────────────────────────
void printTitle(const string &title)
{
    int width = 58;
    int pad = (width - (int)title.size()) / 2;
    cout << "\n  +";
    for (int i = 0; i < width; i++)
        cout << "=";
    cout << "+\n";
    cout << "  |";
    for (int i = 0; i < pad; i++)
        cout << " ";
    cout << title;
    for (int i = 0; i < width - pad - (int)title.size(); i++)
        cout << " ";
    cout << "|\n";
    cout << "  +";
    for (int i = 0; i < width; i++)
        cout << "=";
    cout << "+\n\n";
}

// ── Colored Messages ──────────────────────────────────────────
void printSuccess(const string &msg)
{
    cout << "\n  \033[1;32m[SUCCESS]\033[0m " << msg << "\n";
}

void printError(const string &msg)
{
    cout << "\n  \033[1;31m[ERROR]\033[0m " << msg << "\n";
}

void printInfo(const string &msg)
{
    cout << "\n  \033[1;36m[INFO]\033[0m " << msg << "\n";
}

// ── Menu Box ──────────────────────────────────────────────────
void printMenu(const string &title, vector<string> items)
{
    int width = 40;
    // top
    cout << "\n  +";
    for (int i = 0; i < width; i++)
        cout << "-";
    cout << "+\n";
    // title
    int pad = (width - (int)title.size()) / 2;
    cout << "  |";
    for (int i = 0; i < pad; i++)
        cout << " ";
    cout << "\033[1m" << title << "\033[0m";
    for (int i = 0; i < width - pad - (int)title.size(); i++)
        cout << " ";
    cout << "|\n";
    // divider
    cout << "  +";
    for (int i = 0; i < width; i++)
        cout << "-";
    cout << "+\n";
    // items
    for (auto &item : items)
    {
        cout << "  | " << left << setw(width - 1) << item << "|\n";
    }
    // bottom
    cout << "  +";
    for (int i = 0; i < width; i++)
        cout << "-";
    cout << "+\n";
}

// ── Room Table ────────────────────────────────────────────────
void printTableHeader()
{
    cout << "\n  +-----+--------+----------+-------------+-------------+------------------+\n";
    cout << "  | " << left
         << setw(3) << "No" << " | "
         << setw(6) << "Room#" << " | "
         << setw(8) << "Type" << " | "
         << setw(11) << "Price/Night" << " | "
         << setw(11) << "Status" << " | "
         << setw(16) << "Guest Name" << " |\n";
    cout << "  +-----+--------+----------+-------------+-------------+------------------+\n";
}

void printTableRow(int no, int roomNum, const string &type,
                   double price, const string &status,
                   const string &guest)
{
    string color = (status == "Available") ? "\033[1;32m" : "\033[1;33m";
    string gname = guest.empty() ? "-" : guest;

    cout << "  | " << left
         << setw(3) << no << " | "
         << setw(6) << roomNum << " | "
         << setw(8) << type << " | $"
         << setw(10) << fixed << setprecision(2) << price << " | "
         << color << setw(11) << status << "\033[0m" << " | "
         << setw(16) << gname << " |\n";
}

void printTableFooter(int total)
{
    cout << "  +-----+--------+----------+-------------+-------------+------------------+\n";
    cout << "  | Total: " << total << " room(s)\n";
    cout << "  +-----+--------+----------+-------------+-------------+------------------+\n";
}

// ── Input ─────────────────────────────────────────────────────
int getIntInput(const string &prompt, int min, int max)
{
    int value;
    while (true)
    {
        cout << "  " << prompt;
        if (cin >> value && value >= min && value <= max)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printError("Please enter a number between " + to_string(min) + " and " + to_string(max));
    }
}

string getStringInput(const string &prompt)
{
    string value;
    cout << "  " << prompt;
    getline(cin, value);
    return value;
}