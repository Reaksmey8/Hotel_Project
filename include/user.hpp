#pragma once
#include <string>
#include <vector>
using namespace std;

enum Role
{
    ADMIN,
    NORMAL_USER
};

struct User
{
    string username;
    string password;
    string fullName;
    Role role;
};

class AuthManager
{
private:
    vector<User> users;
    int currentIndex; // -1 = not logged in
    string filename;
    void loadUsers();

public:
    AuthManager();
    bool registerUser();
    bool login();
    void logout();
    bool isLoggedIn() const;
    bool isAdmin() const;
    string getUsername() const;
    string getFullName() const;
};