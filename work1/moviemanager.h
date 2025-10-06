#pragma once
#ifndef MOVIEMANAGER_H
#define MOVIEMANAGER_H

#include <iostream>
#include <string>
#include <iomanip>
#include "theater.h"
#include"color.h"
using namespace std;
 

struct Movie {
    string id;
    string title;
    int showtimeCount = 0;
    string showtimes[5];
    Theater theaters[5];
};

class MovieManager {
private:
    Movie movies[10];
    int count;

public:
    MovieManager() {
        count = 3;
        movies[0] = { "P001", "Mua Do", 3, {"7:00", "9:00", "13:00"} };
        movies[1] = { "P002", "Conan", 3, {"8:00", "10:00", "15:00"} };
        movies[2] = { "P003", "Doraemon", 3, {"9:00", "11:00", "17:00"} };

        
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < movies[i].showtimeCount; j++) {
                movies[i].theaters[j] = Theater(movies[i].id, movies[i].showtimes[j]);
            }
        }
    }

    
    void displayMovies() {
        cout << "\n"
            << BOLD << BLACK << WHITE_BG
            << "============== DANH SACH PHIM ================="
            << RESET << "\n";
        cout << left << setw(8) << "Ma"
            << setw(20) << "Ten phim"
            << "Suat chieu" << RESET << "\n";

        cout << "-----------------------------------------------\n";

        for (int i = 0; i < count; i++) {
            cout << left << setw(8) << movies[i].id
                << setw(20) << movies[i].title;
            for (int j = 0; j < movies[i].showtimeCount; j++) {
                cout << movies[i].showtimes[j];
                if (j < movies[i].showtimeCount - 1) cout << ", ";
            }
            cout << "\n";
        }
    }

    
    Movie* selectMovie() {
        displayMovies();
        string movieId;
        cout << "Nhap ma phim: ";
        cin >> movieId;
        cin.ignore();
        for (int i = 0; i < count; i++) {
            if (movies[i].id == movieId) return &movies[i];
        }
        cout << RED << "Khong tim thay phim!\n" << RESET;
        return nullptr;
    }

    
    int selectShowtime(Movie* m) {
        if (!m) return -1;
        cout << "Danh sach suat chieu:\n";
        for (int i = 0; i < m->showtimeCount; i++) {
            cout << i + 1 << ". " << m->showtimes[i] << endl;
        }
        int choice;
        cout << "Chon suat chieu: ";
        cin >> choice;
        cin.ignore();
        if (choice < 1 || choice > m->showtimeCount) {
            cout << RED << "Suat chieu khong hop le!\n" << RESET;
            return -1;
        }
        return choice - 1;
    }

    
    Theater* getTheater(Movie* m, int showIndex) {
        if (!m || showIndex < 0 || showIndex >= m->showtimeCount) return nullptr;
        return &m->theaters[showIndex];
    }

    
    int countMovies() const {
        return count;
    }

    
    Movie* getMovie(int index) {
        if (index < 0 || index >= count) return nullptr;
        return &movies[index];
    }

    
    void addMovie() {
        if (count >= 10) {
            cout << RED << "Khong the them phim moi (toi da 10 phim)!\n" << RESET;
            return;
        }
        Movie m;
        cout << "Nhap ma phim: "; getline(cin, m.id);
        for (int i = 0; i < count; i++) {
            if (movies[i].id == m.id) {
                cout << RED << "Ma phim da ton tai!\n" << RESET;
                return;
            }
        }
        cout << "Nhap ten phim: "; getline(cin, m.title);
        cout << "Nhap so luong suat chieu (1-5): ";
        cin >> m.showtimeCount; cin.ignore();
        if (m.showtimeCount < 1 || m.showtimeCount > 5) {
            cout << RED << "So suat chieu khong hop le!\n" << RESET;
            return;
        }
        for (int i = 0; i < m.showtimeCount; i++) {
            cout << "Nhap suat chieu thu " << i + 1 << ": ";
            getline(cin, m.showtimes[i]);
            
            m.theaters[i] = Theater(m.id, m.showtimes[i]);
        }
        movies[count++] = m;
        cout << GREEN << ">>> Da them phim thanh cong!\n" << RESET;
    }

    
    void editMovie() {
        Movie* m = selectMovie();
        if (!m) return;
        cout << "Nhap ten phim moi (bo trong de giu nguyen): ";
        string title; getline(cin, title);
        if (!title.empty()) m->title = title;

        cout << "Nhap so luong suat chieu moi (1-5, -1 de giu nguyen): ";
        int sc; cin >> sc; cin.ignore();
        if (sc >= 1 && sc <= 5) {
            m->showtimeCount = sc;
            for (int i = 0; i < sc; i++) {
                cout << "Nhap suat chieu thu " << i + 1 << ": ";
                getline(cin, m->showtimes[i]);
                
                m->theaters[i] = Theater(m->id, m->showtimes[i]);
            }
        }
        cout << GREEN << ">>> Da sua phim thanh cong!\n" << RESET;
    }

    
    void deleteMovie() {
        Movie* m = selectMovie();
        if (!m) return;
        string id = m->id;
        long long idx = m - movies;
        for (long long i = idx; i < count - 1; i++) {
            movies[i] = movies[i + 1];
        }
        count--;
        cout << GREEN << ">>> Da xoa phim " << id << "!\n" << RESET;
    }
};

#endif
