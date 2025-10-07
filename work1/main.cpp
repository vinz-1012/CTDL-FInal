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
#include "moviemanager.h"

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    MovieManager* movieManager = new MovieManager();
    int choice;

    do {
        cout << "\n"<< BOLD << BLACK << WHITE_BG<< "===== HE THONG RAP CHIEU PHIM =====" << RESET << "\n";
        cout << "1. Xem danh sach phim\n";
        cout << "2. Dang nhap khach hang\n";
        cout << "3. Dang nhap quan ly\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore();
        system("cls");

        switch (choice) {
        case 1: {
            movieManager->displayMovies();
            break;
        }

        case 2: {
            string phone, password;
            cout << "Nhap so dien thoai: ";
            getline(cin, phone);
            cout << "Nhap mat khau: ";
            getline(cin, password);

            bool isExisting = false;
            bool loggedIn = false;
            Theater* foundTheater = nullptr;

            for (int i = 0; i < movieManager->countMovies(); i++) {
                Movie* m = movieManager->getMovie(i);
                for (int j = 0; j < m->showtimeCount; j++) {
                    Theater* t = movieManager->getTheater(m, j);
                    if (t->isPhoneExists(phone)) {
                        isExisting = true;
                        foundTheater = t;
                        if (t->checkCustomerLogin(phone, password)) {
                            loggedIn = true;
                        }
                        break;
                    }
                }
                if (isExisting) break;
            }

            if (isExisting && !loggedIn) {
                cout << RED << "So dien thoai da ton tai hoac mat khau khong dung!\n" << RESET;
                break;
            }

            if (!isExisting) {
                cout << GREEN << "Tai khoan moi se duoc tao khi ban dat ve lan dau.\n" << RESET;
            }
            else {
                cout << GREEN << "Dang nhap thanh cong!\n" << RESET;
            }

            runCustomerMenu(*movieManager, phone, password);
            break;
        }

        case 3: {
            string pass;
            cout << "Nhap mat khau quan ly: ";
            getline(cin, pass);

            if (pass == "quanli123") {
                cout << "Ban dang lam viec voi che do quan ly.\n";
                runAdminMenu(*movieManager);
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
            cout << RED << "Lua chon khong hop le!\n" << RESET;
        }
    } while (choice != 0);

    delete movieManager;
    return 0;
}
