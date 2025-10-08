#ifndef CUSTOMERMENU_H
#define CUSTOMERMENU_H

#include "theater.h"
#include "utils.h"
#include <iostream>
using namespace std;

void runCustomerMenu(MovieManager& manager, const string& phone, const string& password) {
    int choiceCus;
    string name, seatLine, ticketCode;
    Theater* currentTheater = nullptr;

    do {
        try {
            cout << "\n" << BOLD << WHITE_BG << BLACK << "======= MENU KHACH HANG =======" << RESET << "\n";
            cout << "1. Dat ghe\n";
            cout << "2. Huy ghe\n";
            cout << "3. Xem ve da dat\n";
            cout << "0. Thoat che do khach hang\n";
            cout << "Lua chon: ";

            if (!(cin >> choiceCus)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << RED << "Vui long nhap so hop le!\n" << RESET;
                continue;
            }
            cin.ignore();
            system("cls");

            switch (choiceCus) {
            case 1: { // Đặt ghế
                try {
                    Movie* selectedMovie = manager.selectMovie();
                    if (!selectedMovie) break;

                    int showtimeIndex = manager.selectShowtime(selectedMovie);
                    if (showtimeIndex == -1) break;

                    currentTheater = manager.getTheater(selectedMovie, showtimeIndex);
                    if (!currentTheater) {
                        cout << RED << "Khong tim thay rap cho suat chieu nay.\n" << RESET;
                        break;
                    }

                    currentTheater->displaySeats();
                    cout << "Nhap ten khach hang: ";
                    getline(cin, name);

                    int opt;
                    cout << "Ban co muon he thong goi y GHE DEP? (1=Co, 0=Khong): ";
                    if (!(cin >> opt)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << RED << "Lua chon khong hop le!\n" << RESET;
                        break;
                    }
                    cin.ignore();

                    if (opt == 1) {
                        int numSeats;
                        cout << "Nhap so luong ghe muon dat: ";
                        if (!(cin >> numSeats)) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << RED << "Vui long nhap so hop le!\n" << RESET;
                            break;
                        }
                        cin.ignore();

                        try {
                            currentTheater->findBestSeats(numSeats);
                        }
                        catch (const exception& e) {
                            cout << RED << e.what() << RESET << endl;
                        }
                    }

                    cout << "Nhap cac ma ghe (vd A1,A2,B3): ";
                    getline(cin, seatLine);

                    try {
                        reserveMultipleSeats(*currentTheater, name, phone, password, seatLine);
                    }
                    catch (const exception& e) {
                        cout << RED << e.what() << RESET << endl;
                    }
                }
                catch (exception& e) {
                    cout << RED << "Loi trong dat ghe: " << e.what() << RESET << endl;
                }
                break;
            }

            case 2: { // Hủy ghế
                try {
                    cout << "Nhap ma ve: ";
                    cin >> ticketCode;
                    cin.ignore();

                    bool canceled = false;
                    string code, seatCode, movieId, showtime;
                    int refund = 0;

                    for (int i = 0; i < manager.countMovies(); i++) {
                        Movie* m = manager.getMovie(i);
                        for (int j = 0; j < m->showtimeCount; j++) {
                            Theater* t = manager.getTheater(m, j);
                            if (!t) continue;

                            Ticket* tk = t->getTickets().find(ticketCode);
                            if (tk && tk->phone == phone && tk->password == password) {
                                code = tk->code;
                                seatCode = string(1, 'A' + tk->row) + to_string(tk->col + 1);
                                movieId = tk->movieId;
                                showtime = tk->showtime;
                                refund = tk->price;

                                try {
                                    t->cancelSeat("", phone, ticketCode, password);
                                    canceled = true;
                                }
                                catch (const exception& e) {
                                    cout << RED << e.what() << RESET << endl;
                                    canceled = true;
                                }
                                break;
                            }
                        }
                        if (canceled) break;
                    }

                    if (!canceled)
                        cout << RED << "Khong tim thay ve hoac thong tin khong khop.\n" << RESET;
                    else
                        cout << GREEN
                        << ">>> Da huy ve thanh cong!\n"
                        << ">>> Ma ve: " << code
                        << " | Ghe : " << seatCode
                        << " | Phim : " << movieId
                        << " | Suat chieu: " << showtime << "\n"
                        << ">>> So tien hoan: " << refund << " VND\n"
                        << RESET;
                }
                catch (exception& e) {
                    cout << RED << "Loi trong huy ghe: " << e.what() << RESET << endl;
                }
                break;
            }

            case 3: { 
                try {
                    manager.displayTicketsByPhone(phone, password);
                }
                catch (exception& e) {
                    cout << RED << "Loi khi hien thi danh sach ve: " << e.what() << RESET << endl;
                }
                break;
            }

            case 0:
                cout << "Thoat che do khach hang.\n";
                break;

            default:
                cout << RED << "Lua chon khong hop le!\n" << RESET;
            }
        }
        catch (exception& e) {
            cout << RED << "Loi trong menu khach hang: " << e.what() << RESET << endl;
        }
    } while (choiceCus != 0);
}

#endif
