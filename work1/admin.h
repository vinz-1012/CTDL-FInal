#ifndef ADMIN_H
#define ADMIN_H

#include "theater.h"
#include "moviemanager.h"
#include <iostream>
using namespace std;

void runAdminMenu(MovieManager& movieManager) {
    int choiceAdmin;
    do {
        cout << "\n" << BOLD << BLACK << WHITE_BG
            << "================ MENU QUAN LY ================"
            << RESET << "\n";

        cout << "1. Hien thi danh sach tat ca ve cua mot suat\n";
        cout << "2. Hien thi so do ghe cua mot suat\n";
        cout << "3. Tim ve theo SDT\n";
        cout << "4. Huy ve\n";
        cout << "5. Quan ly phim\n";
        cout << "0. Thoat che do quan ly\n";
        cout << "Lua chon: ";
        cin >> choiceAdmin;
        cin.ignore();
        system("cls");

        switch (choiceAdmin) {
        case 1: { 
            Movie* selectedMovie = movieManager.selectMovie();
            if (!selectedMovie) break;
            int showIndex = movieManager.selectShowtime(selectedMovie);
            if (showIndex == -1) break;
            Theater* theater = movieManager.getTheater(selectedMovie, showIndex);
            if (!theater) break;

            theater->displaySortedBySeat();
            break;
        }
        case 2: { 
            Movie* selectedMovie = movieManager.selectMovie();
            if (!selectedMovie) break;
            int showIndex = movieManager.selectShowtime(selectedMovie);
            if (showIndex == -1) break;
            Theater* theater = movieManager.getTheater(selectedMovie, showIndex);
            if (!theater) break;

            theater->displaySeats();
            break;
        }
        case 3: {
            string searchPhone;
            cout << "Nhap so dien thoai can tim: ";
            getline(cin, searchPhone);
            movieManager.displayTicketsByPhone(searchPhone, ADMIN_PASSWORD);
            break;
        }
        case 4: { 
            string code;
            cout << "Nhap ma ve can huy: ";
            cin >> code;
            cin.ignore();
            bool found = false;

            for (int i = 0; i < movieManager.countMovies(); i++) {
                Movie* m = movieManager.getMovie(i);
                for (int j = 0; j < m->showtimeCount; j++) {
                    Theater* t = movieManager.getTheater(m, j);
                    Ticket* tk = t->getTickets().find(code);
                    if (tk) {
                        string seatCode = string(1, 'A' + tk->row) + to_string(tk->col + 1);
                        int refund = tk->price;
                        string name = tk->name;
                        string movieId = tk->movieId;
                        string showtime = tk->showtime;
                        t->cancelSeat(tk->name, tk->phone, tk->code, ADMIN_PASSWORD);
                        cout << RED_BG << WHITE
                            << ">>> Da huy ve: " << code
                            << " (Ghe " << seatCode << ")\n"
                            << ">>> Khach: " << name
                            << " | Phim: " << movieId
                            << " | Suat chieu: " << showtime
                            << " | So tien hoan: " << refund << " VND\n"
                            << RESET;
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (!found)
                cout << RED << "Khong tim thay ve voi ma: " << code << "\n" << RESET;
            break;
        }
        case 5: { 
            int subChoice;
            do {
                cout <<BOLD<< BLACK << WHITE_BG << "\n======= QUAN LY PHIM =======\n" << RESET;
                cout << "1. Xem danh sach phim\n";
                cout << "2. Them phim\n";
                cout << "3. Sua phim\n";
                cout << "4. Xoa phim\n";
                cout << "0. Quay lai\n";
                cout << "Lua chon: ";
                cin >> subChoice;
                cin.ignore();
                system("cls");

                switch (subChoice) {
                case 1: movieManager.displayMovies(); break;
                case 2: movieManager.addMovie(); break;
                case 3: movieManager.editMovie(); break;
                case 4: movieManager.deleteMovie(); break;
                case 0: break;
                default: cout << RED << "Lua chon khong hop le!\n" << RESET;
                }
            } while (subChoice != 0);
            break;
        }
        case 0:
            cout << "Thoat che do quan ly.\n";
            break;
        default:
            cout << RED << "Lua chon khong hop le!\n" << RESET;
        }
    } while (choiceAdmin != 0);
}

#endif
