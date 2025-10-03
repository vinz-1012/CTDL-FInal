#ifndef CUSTOMERMENU_H
#define CUSTOMERMENU_H

#include "theater.h"
#include "utils.h"
#include <iostream>
using namespace std;

void runCustomerMenu(Theater& t, const string& phone, const string& password) {
    int choiceCus;
    string name, seatLine, ticketCode;

    do {
        cout << "\n===== MENU KHACH HANG =====\n";
        cout << "1. Dat ghe\n";
        cout << "2. Huy ghe\n";
        cout << "3. Xem ve da dat\n";
        cout << "0. Thoat che do khach hang\n";
        cout << "Lua chon: ";
        cin >> choiceCus;
        cin.ignore();

        switch (choiceCus) {
        case 1: {
            t.displaySeats();
            cout << "Nhap ten khach hang: ";
            getline(cin, name);
            int opt;
            cout << "Ban co muon he thong goi y GHE DEP? (1=Co, 0=Khong): ";
            cin >> opt;
            cin.ignore();
            if (opt == 1) {
                int numSeats;
                cout << "Nhap so luong ghe muon dat: ";
                cin >> numSeats;
                cin.ignore();
                try {
                    cout << "GHE DEP: ";
                     t.findBestSeats(numSeats);
                   
                }
                catch (const exception& e) {
                    cout<<RED << e.what() <<RESET<< endl;
                }
            }
            cout << "Nhap cac ma ghe (vd A1,A2,B3): ";
            getline(cin, seatLine);
            reserveMultipleSeats(t, name, phone, password, seatLine);
            break;
        }
        case 2: {
            cout << "Nhap ma ve: ";
            cin >> ticketCode;
            cin.ignore();
            try {
                t.cancelSeat("", phone, ticketCode, password);
            }
            catch (const exception& e) {
                cout <<RED<< "Loi khi huy ve: " << e.what()<<RESET << endl;
            }
            break;
        }
        case 3: {
            t.displayTickets_cus(phone);
            break;
        }
        case 0:
            cout << "Thoat che do khach hang.\n";
            break;
        default:
            cout<<RED << "Lua chon khong hop le!\n"<<RESET;
        }
    } while (choiceCus != 0);
}

#endif
#pragma once
