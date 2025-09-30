#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "theater.h"
#include <iostream>
using namespace std;

void runAdminMenu(Theater& t) {
    int choiceAdmin;
    do {
        cout << "\n===== MENU QUAN LY =====\n";
        cout << "1. Hien thi danh sach tat ca ve\n";
        cout << "2. Hien thi so do ghe\n";
        cout << "3. Huy ve\n";
        cout << "0. Thoat che do quan ly\n";
        cout << "Lua chon: ";
        cin >> choiceAdmin;
        cin.ignore();

        switch (choiceAdmin) {
        case 1:
            t.displaySortedBySeat();
            break;
        case 2:
            t.displaySeats();
            break;
        case 3: {
            string code;
            cout << "Nhap ma ve can huy: ";
            cin >> code;
            cin.ignore();

            Ticket* tk = t.getTickets().find(code);
            if (!tk) {
                cout << "Khong tim thay ve.\n";
            }
            else {
                string seatCode = string(1, 'A' + tk->row) + to_string(tk->col + 1);
                t.cancelSeat(tk->name, tk->phone, tk->code, ADMIN_PASSWORD);
                cout << ">>> Quan ly da huy ve: " << code
                    << " (Ghe " << seatCode << ")\n"; 
            }
            break;
        }


        case 0:
            cout << "Thoat che do quan ly.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (choiceAdmin != 0);
}

#endif
#pragma once
