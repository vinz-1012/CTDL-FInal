#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "theater.h"
using namespace std;
string generateTicketCode() {
    string code = "VE";
    for (int i = 0; i < 5; i++) {
        code += char('0' + rand() % 10);
    }
    return code;
}

int main() {
    srand(time(0));

    Theater t; 
    int choice;
    string name, phone, ticketCode;
    int row, col;

    do {
        cout << "\n===== QUAN LY GHE RAP CHIEU PHIM =====\n";
        cout << "1. Hien thi so do ghe\n";
        cout << "2. Dat ghe\n";
        cout << "3. Huy ghe\n";
        cout << "4. Tim kiem theo SDT\n";
        cout << "5. Hien thi danh sach dat ve\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
        case 1:
            t.displaySeats();
            break;
        case 2: {
            cout << "Nhap ten khach hang: ";
            getline(cin, name);
            cout << "Nhap so dien thoai: ";
            getline(cin, phone);
            cout << "Nhap ma ghe (vd A1, B3): ";
            string seatCode;
            getline(cin, seatCode);

            ticketCode = generateTicketCode();
            t.reserveSeat(name, phone, seatCode, ticketCode);
            cout << ">>> Ma ve cua ban: " << ticketCode << "\n";
            break;
        }

        case 3: {
            cout << "Nhap ten khach hang: ";
            getline(cin, name);
            cout << "Nhap so dien thoai: ";
            getline(cin, phone);
            cout << "Nhap ma ve: ";
            cin >> ticketCode;
            cin.ignore();
            t.cancelSeat(name, phone, ticketCode);
            break;
        }
        case 4: {
            cout << "Nhap so dien thoai: ";
            getline(cin, phone);
            t.searchByPhone(phone);
            break;
        }
        case 5:
            t.displayAllReservations();
            break;
        case 0:
            cout << "Thoat chuong trinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}
