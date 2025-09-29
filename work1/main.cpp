#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <algorithm>
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
    srand(static_cast<unsigned int>(time(nullptr)));

    Theater t;
    int choice;
    string name, phone, ticketCode;

    do {
        cout << "\n===== QUAN LY GHE RAP CHIEU PHIM =====\n";
        cout << "1. Hien thi so do ghe\n";
        cout << "2. Dat ghe (co the nhap nhieu ghe, vd: A1,A2,B3)\n";
        cout << "3. Huy ghe\n";
        cout << "4. Tim kiem theo SDT\n";
        cout << "5. Hien thi danh sach ve\n";
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
            cout << "Nhap cac ma ghe (vd A1,A2,B3): ";
            string seatLine;
            getline(cin, seatLine);

            stringstream ss(seatLine);
            string seatCode;
            while (getline(ss, seatCode, ',')) {
                seatCode.erase(remove_if(seatCode.begin(), seatCode.end(), ::isspace), seatCode.end());
                string ticketCode = generateTicketCode();
                t.reserveSeat(name, phone, seatCode, ticketCode);
                cout << ">>> Ghe " << seatCode << " dat thanh cong. Ma ve: " << ticketCode << "\n";
            }
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
        case 5: {
            cout << "Nhap password: ";
            string pass;
            cin >> pass;
            if (pass == "quanli123") {
                t.displaySortedBySeat();
            }
            break;
        }
        case 0:
            cout << "Thoat chuong trinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}
