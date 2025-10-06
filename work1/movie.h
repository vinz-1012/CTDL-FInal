#pragma once
#ifndef MOVIE_H
#define MOVIE_H

#include "theater.h"
#include <string>
#include <iostream>
using namespace std;

class Movie {
private:
    string title;  
    string showtime;  
    Theater theater;  
public:
    Movie() {}
    Movie(string t, string s) : title(t), showtime(s) {}

    string getTitle() const { return title; }
    string getShowtime() const { return showtime; }
    Theater& getTheater() { return theater; }

    void setTitle(const string& t) { title = t; }
    void setShowtime(const string& s) { showtime = s; }

    void displayInfo() const {
        cout << "Phim: " << title << " | Suat chieu: " << showtime << endl;
    }
};

#endif
#pragma once

