#include "user.hpp"
#include "util.hpp"
#include <iostream>
using namespace std;

static User accounts[] = {
    User("admin", "admin123", ADMIN),
    User("student", "stu123", NORMAL_USER),
    User("teacher", "tea123", NORMAL_USER)};
static const int ACCOUNT_COUNT = 3;

User::User(string uname, string pwd, Role r)
    : username(uname), password(pwd), role(r) {}

string User::getUsername() const { return username; }
Role User::getRole() const { return role; }
bool User::checkPassword(const string &pwd) const { return password == pwd; }

AuthManager::AuthManager() : currentUser(nullptr) {}
AuthManager::~AuthManager() {}

bool AuthManager::login()
{
    printTitle("LOGIN");
    string uname = getStringInput("Username : ");
    cout << "  Password : ";
    string pwd;
    getline(cin, pwd);

    for (int i = 0; i < ACCOUNT_COUNT; i++)
    {
        if (accounts[i].getUsername() == uname &&
            accounts[i].checkPassword(pwd))
        {
            currentUser = &accounts[i];
            string role = (accounts[i].getRole() == ADMIN) ? "ADMIN" : "USER";
            printSuccess("Welcome, " + uname + "!  [" + role + "]");
            return true;
        }
    }
    printError("Invalid username or password.");
    return false;
}

void AuthManager::logout()
{
    if (currentUser)
    {
        printInfo(currentUser->getUsername() + " has logged out.");
        currentUser = nullptr;
    }
}

bool AuthManager::isLoggedIn() const { return currentUser != nullptr; }
bool AuthManager::isAdmin() const { return currentUser && currentUser->getRole() == ADMIN; }
string AuthManager::getUsername() const { return currentUser ? currentUser->getUsername() : ""; }