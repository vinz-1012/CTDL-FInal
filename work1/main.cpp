#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <algorithm>
#include "theater.h"
#include "utils.h"
#include "admin.h"
#include "customer.h"

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Theater t;
    int choice;

    do {
        cout << "\n===== HE THONG GHE RAP CHIEU PHIM =====\n";
        cout << "1. Hien thi so do ghe\n";
        cout << "2. Dang nhap khach hang\n";
        cout << "3. Dang nhap quan ly\n";
        cout << "0. Thoat\n"; 
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            t.displaySeats();
            break;

        case 2: {
            string phone, password;
            cout << "Nhap so dien thoai: ";
            getline(cin, phone);
            cout << "Nhap mat khau: ";
            getline(cin, password);
            runCustomerMenu(t, phone, password);  
            break;
        }

        case 3: {
            string pass;
            cout << "Nhap mat khau quan ly: ";
            getline(cin, pass);
            if (pass == "quanli123") {
                runAdminMenu(t);  
            }
            else {
                cout << "Sai mat khau quan ly!\n";
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
