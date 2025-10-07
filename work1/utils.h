#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "theater.h"
#include "color.h"


using namespace std;

string generateTicketCode() {
    string code = "VE";
    for (int i = 0; i < 5; i++) {
        code += char('0' + rand() % 10);
    }
    return code;
}

string generateUniqueTicketCode(HashTable& tickets) {
    string code;
    do {
        code = generateTicketCode();
    } while (tickets.find(code) != nullptr);
    return code;
}


void reserveMultipleSeats(Theater& t, const string& name, const string& phone, const string& password, const string& seatLine) {
    stringstream ss(seatLine);
    string seatCode;
    while (getline(ss, seatCode, ',')) {
        seatCode.erase(remove_if(seatCode.begin(), seatCode.end(), ::isspace), seatCode.end());
        string ticketCode = generateUniqueTicketCode(t.getTickets());
        
        t.reserveSeat(name, phone, seatCode, ticketCode, password, t.getMovieId(), t.getShowtime());
        cout << GREEN << ">>> Ghe " << seatCode << " dat thanh cong." << RESET << "\n";

        cout << RED_BG << WHITE << "-------------------------------" << RESET << "\n";

        cout << RED_BG << WHITE << " Ma ve     : " << ticketCode
            << " " << string(17 - ticketCode.length(), ' ') << "" << RESET << "\n";
        cout << RED_BG << WHITE << " Ma phim   : " << t.getMovieId()
            << " " << string(17 - t.getMovieId().length(), ' ') << "" << RESET << "\n";
        cout << RED_BG << WHITE << " Suat chieu: " << t.getShowtime()
            << " " << string(17 - t.getShowtime().length(), ' ') << "" << RESET << "\n";
        cout << RED_BG << WHITE << " Ghe       : " << seatCode
            << " " << string(17 - seatCode.length(), ' ') << RESET << "\n";
        cout << RED_BG << WHITE << "-------------------------------" << RESET << "\n";
    }
}

#endif
