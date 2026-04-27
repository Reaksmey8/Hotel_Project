#include "user.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

AuthManager::AuthManager() : currentIndex(-1), filename("users.txt")
{
    // Add admin
    users.push_back({"admin", "admin123", "Administrator", ADMIN});
    // Load saved users
    loadUsers();
}

void AuthManager::loadUsers()
{
    ifstream fin(filename);
    if (!fin.is_open())
        return;
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string uname, pwd, name;
        getline(ss, uname, ',');
        getline(ss, pwd, ',');
        getline(ss, name, ',');
        bool exists = false;
        for (auto &u : users)
            if (u.username == uname)
            {
                exists = true;
                break;
            }
        if (!exists)
            users.push_back({uname, pwd, name, NORMAL_USER});
    }
    fin.close();
}

bool AuthManager::registerUser()
{
    printTitle("REGISTER");
    string name = getStringInput("Full Name : ");
    if (name.empty())
    {
        printError("Name empty!");
        pauseScreen();
        return false;
    }

    string uname = getStringInput("Username  : ");
    if (uname.empty())
    {
        printError("Username empty!");
        pauseScreen();
        return false;
    }

    for (auto &u : users)
        if (u.username == uname)
        {
            printError("Username already exists!");
            pauseScreen();
            return false;
        }

    cout << "  Password  : ";
    string pwd;
    getline(cin, pwd);
    if (pwd.empty())
    {
        printError("Password empty!");
        pauseScreen();
        return false;
    }

    cout << "  Confirm   : ";
    string pwd2;
    getline(cin, pwd2);
    if (pwd != pwd2)
    {
        printError("Passwords don't match!");
        pauseScreen();
        return false;
    }

    // Save to file
    ofstream fout(filename, ios::app);
    fout << uname << "," << pwd << "," << name << "\n";
    fout.close();

    // Add to list and auto-login
    users.push_back({uname, pwd, name, NORMAL_USER});
    currentIndex = (int)users.size() - 1;

    printSuccess("Welcome, " + name + "! You are now logged in.");
    pauseScreen();
    return true;
}

bool AuthManager::login()
{
    printTitle("LOGIN");
    string uname = getStringInput("Username : ");
    cout << "  Password : ";
    string pwd;
    getline(cin, pwd);

    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i].username == uname && users[i].password == pwd)
        {
            currentIndex = i;
            string role = (users[i].role == ADMIN) ? "ADMIN" : "USER";
            printSuccess("Welcome, " + users[i].fullName + "! [" + role + "]");
            return true;
        }
    }
    printError("Wrong username or password!");
    return false;
}

void AuthManager::logout()
{
    if (currentIndex >= 0)
        printInfo(users[currentIndex].username + " logged out.");
    currentIndex = -1;
}

bool AuthManager::isLoggedIn() const { return currentIndex >= 0; }
bool AuthManager::isAdmin() const { return currentIndex >= 0 && users[currentIndex].role == ADMIN; }
string AuthManager::getUsername() const { return currentIndex >= 0 ? users[currentIndex].username : ""; }
string AuthManager::getFullName() const { return currentIndex >= 0 ? users[currentIndex].fullName : ""; }