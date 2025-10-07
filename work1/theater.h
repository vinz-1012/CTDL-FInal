#ifndef THEATER_H
#define THEATER_H
#define ADMIN_PASSWORD "quanli123"


#include "Ticket.h"
#include "queue.h"
#include "hashtable.h"
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <iomanip> 
#include <cfloat> 
#include <climits> 
#include"color.h"
using namespace std;

class Theater {
private:
    int rows;
    int cols;
    bool seats[10][10];
    HashTable tickets;
    Queue waitingList;
    string movieId;
    string showtime;

public:
    Theater(const string& mId = "", const string& st = "")
        : movieId(mId), showtime(st)
    {
        rows = cols = 10;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) seats[i][j] = false;
    }

    string getMovieId() const { return movieId; }
    string getShowtime() const { return showtime; }
    void setMovieId(const string& id) { movieId = id; }
    void setShowtime(const string& st) { showtime = st; }
    void displaySeats() {
        try {
                cout << "\n";
                cout << BOLD << BLACK << WHITE_BG;
                cout << "               M A N   H I N H               " << RESET << "\n\n";
                cout << setw(4) << "";
                for (int j = 0; j < cols; j++) {
                    cout << setw(4) << j + 1;
                }
                cout << "\n";

            for (int i = 0; i < rows; i++) {
                cout << setw(3) << char('A' + i);
                for (int j = 0; j < cols; j++) {
                    string symbol;
                    if (seats[i][j]) {
                        symbol = string(GREEN) + "[X]" + string(RESET);
                    }
                    else if ((i >= 3 && i <= 6) && (j >= 3 && j <= 6)) {
                        symbol = string(RED) + "[ ]" + string(RESET);
                    }
                    else {
                        symbol = string(YELLOW) + "[ ]" + string(RESET);
                    }
                    cout << ' ' << symbol;
                }
                cout << "\n";
            }

            cout << "\n"
                << YELLOW << "[ ]" << RESET << " : Ghe thuong (100,000 VND)\n"
                << RED << "[ ]" << RESET << " : Ghe VIP (150,000 VND)\n"
                << GREEN << "[X]" << RESET << " : Da duoc dat\n";
        }
        catch (const exception& e) {
            cout << RED << "Loi khi hien thi ghe: " << e.what() << RESET << endl;
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

    void reserveSeat(string name, string phone, string seatCode, string code, string password,
        const string& mId, const string& st)
    {
        int row, col;
        if (!parseSeatCode(seatCode, row, col)) {
            throw invalid_argument("Ma ghe khong hop le.");
        }

        int price = Price(row, col);
        if (seats[row][col]) {
            Ticket t = { code, name, phone, password, row, col, price, mId, st };
            waitingList.enqueue(t);
            throw runtime_error("Ghe da duoc dat, da them vao danh sach cho.");
        }

        seats[row][col] = true;
        Ticket t = { code, name, phone, password, row, col, price, mId, st };
        tickets.insert(t);
    }

    void cancelSeat(string name, string phone, string code, string password) {
        Ticket* t = tickets.find(code);
        if (!t) throw runtime_error("Khong tim thay ve.");

        if (password != ADMIN_PASSWORD) {
            if (t->phone != phone || t->password != password) {
                throw runtime_error("Thong tin xac thuc khong dung. Khong the huy ve.");
            }
        }

        seats[t->row][t->col] = false;
        int refund = t->price;
        tickets.remove(code);

        cout << RED << ">>> Huy ve thanh cong. Hoan tien: " << refund << " VND\n" << RESET;

        if (!waitingList.isEmpty()) {
            Ticket next = waitingList.dequeue();
            string seatCode = string(1, 'A' + next.row) + to_string(next.col + 1);
            reserveSeat(next.name, next.phone, seatCode, next.code, next.password, next.movieId, next.showtime);
        }
    }


    void displayTicketsByPhone(const string& phone, const string& password) {
        int count = 0;
        Ticket* results = tickets.findByPhone(phone, count);
        if (!results || count == 0) {
            cout << RED << "Khong tim thay ve voi SDT nay.\n" << RESET;
            return;
        }

        cout << BOLD << BLUE<<WHITE_BG;
        cout << "+---------+----------------------+---------------+-------+-----------+------------+------------+\n";
        cout << "| Mave    | Ten                  | SDT           | Ghe   | Gia(VND)  | Ma phim    | Suat chieu |\n";
        cout << "+---------+----------------------+---------------+-------+-----------+------------+------------+\n" << RESET;

        int sum = 0;
        for (int i = 0; i < count; i++) {
            if (password == ADMIN_PASSWORD || results[i].password == password) {
                string seatCode = string(1, 'A' + results[i].row) + to_string(results[i].col + 1);
                cout << "| " << setw(6) << left << results[i].code
                    << " | " << setw(20) << left << results[i].name
                    << " | " << setw(13) << left << results[i].phone
                    << " | " << YELLOW << setw(5) << left << seatCode << RESET
                    << " | " << RED << setw(9) << right << results[i].price << RESET
                    << " | " << setw(10) << left << results[i].movieId
                    << " | " << setw(10) << left << results[i].showtime
                    << " |" << endl;
                sum += results[i].price;
            }
        }

        cout << "+---------+----------------------+---------------+-------+-----------+------------+------------+\n";
        cout << GREEN << ">>> Tong tien ve: " << sum << " VND" << RESET << endl;

        delete[] results;
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

            cout << BOLD << BLUE<<WHITE_BG;
            cout << "+---------+----------------------+---------------+------+-----------+\n";
            cout << "| Mave    | Ten                  | SDT           | Ghe  | Gia(VND)  |\n";
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
    bool checkCustomerLogin(const string& phone, const string& password) {
        int n;
        try {
            Ticket* ticketsArr = getAllTickets(n);
            for (int i = 0; i < n; i++) {
                if (ticketsArr[i].phone == phone && ticketsArr[i].password == password) {
                    delete[] ticketsArr;
                    return true;
                }
            }
            delete[] ticketsArr;
        }
        catch (const runtime_error&) {
            
        }

        return false;
    }
    bool isPhoneExists(const string& phone) {
        int n;
        try {
            Ticket* ticketsArr = getAllTickets(n);
            for (int i = 0; i < n; i++) {
                if (ticketsArr[i].phone == phone) {
                    delete[] ticketsArr;
                    return true;
                }
            }
            delete[] ticketsArr;
        }
        catch (const runtime_error&) {
            
        }

        return false;
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

        int d[10][10];
        int prefix_d[10][10];
        int prefix_s[10][10];

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
        if ((row >= 3 && row <= 6) && (col >= 3 && col <= 6)) {
            return 150000;
        }
        return 100000;
    }
};
#endif