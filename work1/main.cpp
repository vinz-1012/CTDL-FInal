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
        system("cls");

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
            if (t.isPhoneExists(phone)) {
             
                if (t.checkCustomerLogin(phone, password)) {
                    cout << "Dang nhap thanh cong!\n";
                    runCustomerMenu(t, phone, password);
                }
                else {
                    cout << "So dien thoai da ton tai nhung mat khau khong dung. Khong the tao tai khoan moi!\n";
                }
            }
            else {
                cout << "Tai khoan moi se duoc tao khi ban dat ve lan dau.\n";
                runCustomerMenu(t, phone, password);
            }
            break;
        }

        case 3: {
            string pass;
            cout << "Nhap mat khau quan ly: ";
            getline(cin, pass);

            if (pass == "quanli123") {
                cout << "Ban dang lam viec voi che do quan li.\n";
                runAdminMenu(t);
            }
            else {
                cout << RED << "Sai mat khau quan ly! Quay lai menu chinh.\n" << RESET;
            }
            break;
        }


        case 0:
            cout << "Thoat chuong trinh.\n";
            break;

        default:
            cout<<RED << "Lua chon khong hop le!\n"<<RESET;
        }
    } while (choice != 0);

    return 0;
}
