#ifndef THEATER_H
#define THEATER_H
#define ADMIN_PASSWORD "quanli123"
#include "Ticket.h"
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

    void reserveSeat(string name, string phone, string seatCode, string code, string password) {
        int row, col;
        if (!parseSeatCode(seatCode, row, col)) {
            throw invalid_argument("Ma ghe khong hop le.");
        }
        if (seats[row][col]) {
            Ticket t = { code, name, phone, password, row, col, 100000 };
            waitingList.enqueue(t);
            throw runtime_error("Ghe da duoc dat, da them vao danh sach cho.");
        }
        seats[row][col] = true;
        Ticket t = { code, name, phone, password, row, col, 100000 };
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

        cout << ">>> Huy ve thanh cong. Hoan tien: " << refund << " VND\n";

        if (!waitingList.isEmpty()) {
            Ticket next = waitingList.dequeue();
            string seatCode = string(1, 'A' + next.row) + to_string(next.col + 1);
            reserveSeat(next.name, next.phone, seatCode, next.code, next.password);
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
            throw runtime_error("So luong ghe khong hop le.");
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

        for (int k = 0; k < numSeats; k++) {
            string seatCode = string(1, 'A' + best.row) + to_string(best.startCol + k + 1);
            cout << seatCode << " ";
        }
        cout << endl;
    }

};

#endif
