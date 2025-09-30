#ifndef THEATER_H
#define THEATER_H

#include "ticket.h"
#include "queue.h"
#include "hashtable.h"
#include <stdexcept>
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
        try {
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
        catch (const exception& e) {
            cout << "Loi khi hien thi ghe: " << e.what() << endl;
        }
    }

    bool parseSeatCode(string seatCode, int& row, int& col) {
        if (seatCode.length() < 2) return false;
        char r = toupper(seatCode[0]);
        if (r < 'A' || r > 'A' + rows - 1) return false;
        string cStr = seatCode.substr(1);
        int c = stoi(cStr);
        if (c < 1 || c > cols) return false;
        row = r - 'A';
        col = c - 1;
        return true;
    }

    void reserveSeat(string name, string phone, string seatCode, string code) {
        int row, col;
        if (!parseSeatCode(seatCode, row, col)) {
            throw invalid_argument("Ma ghe khong hop le.");
        }
        if (seats[row][col]) {
            Ticket t = { code,name,phone,row,col };
            waitingList.enqueue(t);
            throw runtime_error("Ghe da duoc dat, da them vao danh sach cho.");
        }
        seats[row][col] = true;
        Ticket t = { code,name,phone,row,col };
        tickets.insert(t);
    }

    void cancelSeat(string name, string phone, string code) {
        Ticket* t = tickets.find(code);
        if (!t) {
            throw runtime_error("Khong tim thay ve.");
        }
        seats[t->row][t->col] = false;
        tickets.remove(code);
        if (!waitingList.isEmpty()) {
            Ticket next = waitingList.dequeue();
            string seatCode = string(1, 'A' + next.row) + to_string(next.col + 1);
            reserveSeat(next.name, next.phone, seatCode, next.code);
        }
    }

    void searchByPhone(string phone) {
        try {
            int count = 0;
            Ticket* results = tickets.findByPhone(phone, count);
            for (int i = 0; i < count; i++) {
                string seatCode = string(1, 'A' + results[i].row) + to_string(results[i].col + 1);
                cout << "Code: " << results[i].code
                    << ", Name: " << results[i].name
                    << ", Ghe " << seatCode << endl;
            }
            delete[] results;
        }
        catch (const exception& e) {
            cout << "Loi tim kiem: " << e.what() << endl;
        }
    }

    void displaySortedBySeat() {
        try {
            int n = tickets.countTickets();
            if (n == 0) {
                throw runtime_error("Chua co ve nao.");
            }
            Ticket* arr = new Ticket[n];
            int idx = 0;
            for (int i = 0; i < TABLE_SIZE; i++) {
                Node* current = tickets.getTable()[i].getHead();
                while (current) {
                    if (idx < n) {             
                        arr[idx++] = current->ticket;
                    }
                    current = current->next;
                }
            }
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (arr[j].row > arr[j + 1].row ||
                        (arr[j].row == arr[j + 1].row && arr[j].col > arr[j + 1].col)) {
                        Ticket temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
            cout << "Danh sach ve da dat :\n";
            for (int i = 0; i < n; i++) {
                string seatCode = string(1, 'A' + arr[i].row) + to_string(arr[i].col + 1);
                cout << "Code: " << arr[i].code
                    << ", Name: " << arr[i].name
                    << ", Phone: " << arr[i].phone
                    << ", Ghe " << seatCode << endl;
            }
            delete[] arr;
        }
        catch (const exception& e) {
            cout << "Loi hien thi danh sach: " << e.what() << endl;
        }
    }

    Ticket* getAllTickets(int& n) {
        n = tickets.countTickets();
        if (n <= 0) {
            throw runtime_error("Chua co ve nao.");
        }
        Ticket* arr = new Ticket[n];
        int idx = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = tickets.getTable()[i].getHead();
            while (current) {
                if (idx < n) {                 
                    arr[idx++] = current->ticket;
                }
                current = current->next;
            }
        }
        return arr;
    }

    HashTable& getTickets() {
        return tickets;
    }
};

#endif
