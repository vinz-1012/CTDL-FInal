#ifndef THEATER_H
#define THEATER_H

#include "ticket.h"
#include "queue.h"
#include "hashtable.h"
#include <iostream>
#include <cctype>
using namespace std;

class Theater {
private:
    int rows;
    int cols;
    bool seats[5][5];
    HashTable tickets;
    Queue waitingList;
public:
    Theater() {
        rows = cols = 5;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) seats[i][j] = false;
    }
    void displaySeats() {
        cout << "So do ghe:\n   ";
        for (int j = 0; j < cols; j++) cout << "[" << j + 1 << "]";
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << char('A' + i) << " ";
            for (int j = 0; j < cols; j++)
                cout << "[" << (seats[i][j] ? "X" : " ") << "]";
            cout << endl;
        }
    }
    bool parseSeatCode(string seatCode, int& row, int& col) {
        if (seatCode.length() < 2) return false;
        char r = toupper(seatCode[0]);
        if (r < 'A' || r> 'A' + rows - 1) return false;
        string cStr = seatCode.substr(1);
        int c = stoi(cStr);
        if (c<1 || c>cols) return false;
        row = r - 'A';
        col = c - 1;
        return true;
    }

    void reserveSeat(string name, string phone, string seatCode, string code) {
        int row, col;
        if (!parseSeatCode(seatCode, row, col)) {
            cout << "Ma ghe khong hop le.\n";
            return;
        }
        if (seats[row][col]) {
            cout << "Ghe da duoc dat. Them vao danh sach cho.\n";
            Ticket t = { code,name,phone,row,col };
            waitingList.enqueue(t);
            return;
        }
        seats[row][col] = true;
        Ticket t = { code,name,phone,row,col };
        tickets.insert(t);
        cout << "Dat ghe thanh cong.\n";
    }

    void cancelSeat(string name, string phone, string code) {
        Ticket* t = tickets.find(code);
        if (!t) {
            cout << "Khong tim thay ve.\n";
            return;
        }
        seats[t->row][t->col] = false;
        tickets.remove(code);
        cout << "Huy ghe thanh cong.\n";
        if (!waitingList.isEmpty()) {
            Ticket next = waitingList.dequeue();
            string seatCode = string(1, 'A' + next.row) + to_string(next.col + 1);
            reserveSeat(next.name, next.phone, seatCode, next.code);
        }
    }

    void searchByPhone(string phone) {
        tickets.findByPhone(phone);
    }

    void displayAllReservations() {
        cout << "Danh sach tat ca ve da dat:\n";
        tickets.printAll();
    }
};

#endif
