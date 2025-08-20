#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
using namespace std;

bool EmailVerifier(string);
bool PasswordVerifier(string);
bool GenderVerifier(string);
bool MobileVerifier(string);
bool DOBVerifier(string);
string getHiddenInput();
void clearScreen();
void Line();
struct tm* Time();
int x_coord(string);
void gotoxy(int, int);
void Title(string);
void Message(string);

#endif 