#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "theater.h"
using namespace std;

string generateTicketCode() {
    string code = "VE";
    for (int i = 0; i < 5; i++) {
        code += char('0' + rand() % 10);
    }
    return code;
}
void reserveMultipleSeats(Theater& t, const string& name, const string& phone, const string& seatLine) {
    stringstream ss(seatLine);
    string seatCode;
    while (getline(ss, seatCode, ',')) {
        seatCode.erase(remove_if(seatCode.begin(), seatCode.end(), ::isspace), seatCode.end());
        string ticketCode = generateTicketCode();
        t.reserveSeat(name, phone, seatCode, ticketCode);
        cout << ">>> Ghe " << seatCode << " dat thanh cong. Ma ve: " << ticketCode << "\\n";
    }
}

#endif
#pragma once
