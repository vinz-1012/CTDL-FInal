#ifndef THEATER_H
#define THEATER_H
#define ADMIN_PASSWORD "quanli123"
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define BOLD    "\x1b[1m"

#include "Ticket.h"
#include "queue.h"
#include "hashtable.h"
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <iomanip> 
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
            for (int j = 0; j < cols; j++)
                cout << "[" << j + 1 << "]";
            cout << endl;

            for (int i = 0; i < rows; i++) {
                cout << char('A' + i) << " ";
                for (int j = 0; j < cols; j++) {
                    if (seats[i][j]) {
                        cout << GREEN << "[X]" << RESET;
                    }
                    else if ((i >= 1 && i <= 3) && (j >= 1 && j <= 3)) {
                        cout << RED << "[ ]" << RESET;
                    }
                    else {
                        cout << YELLOW << "[ ]" << RESET;
                    }
                }
                cout << endl;
            }
            cout << YELLOW << "[ ]" << RESET << " : Ghe thuong (100,000 VND)\n";
            cout << RED << "[ ]" << RESET << " : Ghe VIP (150,000 VND)\n";
            cout << GREEN << "[X]" << RESET << " : Da duoc dat\n";
        }
        catch (const exception& e) {
            cout << RED << "Loi khi hien thi ghe: " << e.what() << endl;
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

    void reserveSeat(string name, string phone, string seatCode, string code, string password) {
        int row, col;
        if (!parseSeatCode(seatCode, row, col)) {
            throw invalid_argument("Ma ghe khong hop le.");
        }
        int price = Price(row, col);
        if (seats[row][col]) {
            Ticket t = { code, name, phone, password, row, col, price };
            waitingList.enqueue(t);
            throw runtime_error("Ghe da duoc dat, da them vao danh sach cho.");
        }
        seats[row][col] = true;
        Ticket t = { code, name, phone, password, row, col, price };
        tickets.insert(t);
    }


    void cancelSeat(string name, string phone, string code, string password) {
        Ticket* t = tickets.find(code);
        if (!t) {
            throw runtime_error("Khong tim thay ve.");
        }

        if (password != ADMIN_PASSWORD) {
            if (t->phone != phone || t->password != password) {
                throw runtime_error("Thong tin xac thuc khong dung. Khong the huy ve.");
            }
        }

        seats[t->row][t->col] = false;
        int refund = t->price;   
        tickets.remove(code);

        cout << RED << ">>> Huy ve thanh cong. Hoan tien: " << refund << " VND\n"<<RESET;

        if (!waitingList.isEmpty()) {
            Ticket next = waitingList.dequeue();
            string seatCode = string(1, 'A' + next.row) + to_string(next.col + 1);
            reserveSeat(next.name, next.phone, seatCode, next.code, next.password);
        }
    }

    void displayTickets_cus(string phone) {
        try {
            int count = 0;
            Ticket* results = tickets.findByPhone(phone, count);

            cout << BOLD << BLUE;
            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << "| Mave    | Ten                  | Sdt           | Ghe  | Gia(VND)  |\n";
            cout << "+---------+----------------------+---------------+------+-----------+\n" << RESET;

            int sum = 0;
            for (int i = 0; i < count; i++) {
                string seatCode = string(1, 'A' + results[i].row) + to_string(results[i].col + 1);

                cout << "| " << setw(6) << left << results[i].code
                    << " | " << setw(20) << left << results[i].name
                    << " | " << setw(13) << left << results[i].phone
                    << " | " << YELLOW << setw(4) << left << seatCode << RESET
                    << " | " << RED << setw(9) << right << results[i].price << RESET << " |"
                    << endl;

                sum += results[i].price;
            }

            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << GREEN << ">>> Tong tien ve cua ban: " << sum << " VND" << RESET << endl;

            delete[] results;
        }
        catch (const exception& e) {
            cout<<RED << "Loi tim kiem: " << e.what() << RESET << endl;
        }
    }
    void searchByPhone(string phone) {
        try {
            int count = 0;
            Ticket* results = tickets.findByPhone(phone, count);
      
            cout << BOLD << BLUE;
            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << "| Mave    | Ten                  | Sdt           | Ghe  | Gia(VND)  |\n";
            cout << "+---------+----------------------+---------------+------+-----------+\n" << RESET;

            int sum = 0;
            for (int i = 0; i < count; i++) {
                string seatCode = string(1, 'A' + results[i].row) + to_string(results[i].col + 1);

                cout << "| " << setw(6) << left << results[i].code
                    << " | " << setw(20) << left << results[i].name
                    << " | " << setw(13) << left << results[i].phone
                    << " | " << YELLOW << setw(4) << left << seatCode << RESET
                    << " | " << RED << setw(9) << right << results[i].price << RESET << " |"
                    << endl;

                sum += results[i].price;
            }

            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << GREEN << ">>> Tong tien ve cua SDT: " << sum << " VND" << RESET << endl;

            delete[] results;
        }
        catch (const exception& e) {
            cout<<RED << "Loi tim kiem: " << e.what() << RESET << endl;
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
                    if (idx < n) arr[idx++] = current->ticket;
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

            cout << BOLD << BLUE;
            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << "| Mave    | Ten                  | Sdt           | Ghe  | Gia(VND)  |\n";
            cout << "+---------+----------------------+---------------+------+-----------+\n" << RESET;

            int sum = 0;
            for (int i = 0; i < n; i++) {
                string seatCode = string(1, 'A' + arr[i].row) + to_string(arr[i].col + 1);

                cout << "| " << setw(6) << left << arr[i].code
                    << " | " << setw(20) << left << arr[i].name
                    << " | " << setw(13) << left << arr[i].phone
                    << " | " << YELLOW << setw(4) << left << seatCode << RESET
                    << " | " << RED << setw(9) << right << arr[i].price << RESET << " |"
                    << endl;

                sum += arr[i].price;
            }

            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << GREEN << ">>> DOANH THU: " << sum << " VND" << RESET << endl;

            delete[] arr;
        }
        catch (const exception& e) {
            cout << "Loi hien thi danh sach: " << e.what() << RESET << endl;
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
    struct BestSeatChoice {
        int row;
        int startCol;
        double score;
        int sum;
    };

    bool isBetter(double dAvg, int dSum, int i, int j, const BestSeatChoice& best, int centerRow, int centerCol, int numSeats) {
        if (dAvg != best.score) return dAvg < best.score;
        if (dSum != best.sum) return dSum < best.sum;

        int dRowCur = abs(i - centerRow);
        int dRowBest = abs(best.row - centerRow);
        if (dRowCur != dRowBest) return dRowCur < dRowBest;

        double centerGroupCur = j + (numSeats - 1) / 2.0;
        double centerGroupBest = best.startCol + (numSeats - 1) / 2.0;
        double dColCur = abs(centerGroupCur - centerCol);
        double dColBest = abs(centerGroupBest - centerCol);
        if (dColCur != dColBest) return dColCur < dColBest;

        return i < best.row;
    }
    void findBestSeats(int numSeats) {
        if (numSeats <= 0 || numSeats > cols) {
            throw runtime_error("Rat tiec, so luong ghe ban can khong du.");
        }

        BestSeatChoice best = { -1, -1, DBL_MAX, INT_MAX };
        int centerRow = rows / 2;
        int centerCol = cols / 2;

        int d[5][5];
        int prefix_d[5][5];
        int prefix_s[5][5];

        for (int i = 0; i < rows; i++) {
            int runD = 0;
            int runS = 0;
            for (int j = 0; j < cols; j++) {
                d[i][j] = abs(i - centerRow) + abs(j - centerCol);
                runD += d[i][j];
                prefix_d[i][j] = runD;

                runS += seats[i][j];
                prefix_s[i][j] = runS;
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j <= cols - numSeats; j++) {

                int leftSeat;
                if (j > 0) {
                    leftSeat = prefix_s[i][j - 1]; 
                }
                else {
                    leftSeat = 0; 
                }

                int rightSeat = prefix_s[i][j + numSeats - 1]; 
                int occupied = rightSeat - leftSeat;          
                if (occupied > 0) continue;   

                int leftDist;
                if (j > 0) {
                    leftDist = prefix_d[i][j - 1]; 
                }
                else {
                    leftDist = 0;
                }

                int rightDist = prefix_d[i][j + numSeats - 1]; 
                int dSum = rightDist - leftDist;              
                double dAvg = (double)dSum / numSeats;    

                if (isBetter(dAvg, dSum, i, j, best, centerRow, centerCol, numSeats)) {
                    best = { i, j, dAvg, dSum };
                }
            }
        }

        if (best.row == -1) {
            throw runtime_error("Khong tim thay cum ghe lien ke phu hop.");
        }
        cout << "GHE DEP: ";
        for (int k = 0; k < numSeats; k++) {
            string seatCode = string(1, 'A' + best.row) + to_string(best.startCol + k + 1);
            cout << seatCode << " ";
        }
        cout << endl;
    }
    int Price(int row, int col) {
        if ((row >= 1 && row <= 3) && (col >= 1 && col <= 3)) {
            return 150000;
        }
        return 100000;
    }
};

#endif
