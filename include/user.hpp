#pragma once
#include <string>
using namespace std;

enum Role
{
    ADMIN,
    NORMAL_USER
};

class User
{
private:
    string username;
    string password;
    Role role;

public:
    User(string uname, string pwd, Role r);
    string getUsername() const;
    Role getRole() const;
    bool checkPassword(const string &pwd) const;
};

class AuthManager
{
private:
    User *currentUser;

public:
    AuthManager();
    ~AuthManager();
    bool login();
    void logout();
    bool isLoggedIn() const;
    bool isAdmin() const;
    string getUsername() const;
};