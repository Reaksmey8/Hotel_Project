#pragma once
#include <string>
#include <vector>
#include "tabulate/table.hpp"
using namespace std;
using namespace tabulate;

void clearScreen();
void pauseScreen();
void printTitle(const string &title);
void printSuccess(const string &msg);
void printError(const string &msg);
void printInfo(const string &msg);
void printMenu(const string &title, vector<string> items);
int getIntInput(const string &prompt, int min, int max);
string getStringInput(const string &prompt);