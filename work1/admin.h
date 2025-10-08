#ifndef ADMIN_H
#define ADMIN_H

#include "theater.h"
#include "moviemanager.h"
#include <iostream>
using namespace std;

void runAdminMenu(MovieManager& movieManager) {
    int choiceAdmin;
    do {
        try {
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

            if (!(cin >> choiceAdmin)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << RED << "Vui long nhap so hop le!\n" << RESET;
                continue;
            }
            cin.ignore();
            system("cls");

            switch (choiceAdmin) {
            case 1: { 
                try {
                    Movie* selectedMovie = movieManager.selectMovie();
                    if (!selectedMovie) break;

                    int showIndex = movieManager.selectShowtime(selectedMovie);
                    if (showIndex == -1) break;

                    Theater* theater = movieManager.getTheater(selectedMovie, showIndex);
                    if (!theater) {
                        cout << RED << "Khong tim thay rap cho suat nay!\n" << RESET;
                        break;
                    }

                    theater->displaySortedBySeat();
                }
                catch (exception& e) {
                    cout << RED << "Loi khi hien thi danh sach ve: " << e.what() << RESET << endl;
                }
                break;
            }

            case 2: { 
                try {
                    Movie* selectedMovie = movieManager.selectMovie();
                    if (!selectedMovie) break;

                    int showIndex = movieManager.selectShowtime(selectedMovie);
                    if (showIndex == -1) break;

                    Theater* theater = movieManager.getTheater(selectedMovie, showIndex);
                    if (!theater) {
                        cout << RED << "Khong tim thay rap cho suat nay!\n" << RESET;
                        break;
                    }

                    theater->displaySeats();
                }
                catch (exception& e) {
                    cout << RED << "Loi khi hien thi so do ghe: " << e.what() << RESET << endl;
                }
                break;
            }

            case 3: {
                try {
                    string searchPhone;
                    cout << "Nhap so dien thoai can tim: ";
                    getline(cin, searchPhone);

                    if (searchPhone.empty()) {
                        cout << RED << "So dien thoai khong duoc de trong!\n" << RESET;
                        break;
                    }

                    movieManager.displayTicketsByPhone(searchPhone, ADMIN_PASSWORD);
                }
                catch (exception& e) {
                    cout << RED << "Loi khi tim ve: " << e.what() << RESET << endl;
                }
                break;
            }

            case 4: {
                try {
                    string code;
                    cout << "Nhap ma ve can huy: ";
                    cin >> code;
                    cin.ignore();

                    bool found = false;

                    for (int i = 0; i < movieManager.countMovies(); i++) {
                        Movie* m = movieManager.getMovie(i);
                        for (int j = 0; j < m->showtimeCount; j++) {
                            Theater* t = movieManager.getTheater(m, j);
                            if (!t) continue;

                            Ticket* tk = t->getTickets().find(code);
                            if (tk) {
                                string seatCode = string(1, 'A' + tk->row) + to_string(tk->col + 1);
                                int refund = tk->price;
                                string name = tk->name;
                                string movieId = tk->movieId;
                                string showtime = tk->showtime;

                                try {
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
                                }
                                catch (const exception& e) {
                                    cout << RED << "Loi khi huy ve: " << e.what() << RESET << endl;
                                    found = true;
                                }
                                break;
                            }
                        }
                        if (found) break;
                    }

                    if (!found)
                        cout << RED << "Khong tim thay ve voi ma: " << code << "\n" << RESET;
                }
                catch (exception& e) {
                    cout << RED << "Loi trong chuc nang huy ve: " << e.what() << RESET << endl;
                }
                break;
            }

            case 5: { 
                try {
                    int subChoice;
                    do {
                        cout << BOLD << BLACK << WHITE_BG << "\n======= QUAN LY PHIM =======\n" << RESET;
                        cout << "1. Xem danh sach phim\n";
                        cout << "2. Them phim\n";
                        cout << "3. Sua phim\n";
                        cout << "4. Xoa phim\n";
                        cout << "0. Quay lai\n";
                        cout << "Lua chon: ";

                        if (!(cin >> subChoice)) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << RED << "Vui long nhap so hop le!\n" << RESET;
                            continue;
                        }
                        cin.ignore();
                        system("cls");

                        switch (subChoice) {
                        case 1:
                            movieManager.displayMovies();
                            break;
                        case 2:
                            movieManager.addMovie();
                            break;
                        case 3:
                            movieManager.editMovie();
                            break;
                        case 4:
                            movieManager.deleteMovie();
                            break;
                        case 0:
                            break;
                        default:
                            cout << RED << "Lua chon khong hop le!\n" << RESET;
                        }
                    } while (subChoice != 0);
                }
                catch (exception& e) {
                    cout << RED << "Loi trong quan ly phim: " << e.what() << RESET << endl;
                }
                break;
            }

            case 0:
                cout << "Thoat che do quan ly.\n";
                break;

            default:
                cout << RED << "Lua chon khong hop le!\n" << RESET;
            }
        }
        catch (exception& e) {
            cout << RED << "Loi trong menu quan ly: " << e.what() << RESET << endl;
        }
    } while (choiceAdmin != 0);
}

#endif
