#ifndef TICKET_H
#define TICKET_H

#include <string>
using namespace std;

struct Ticket {
    string code;
    string name;
    string phone;
    int row = 0, col = 0;
};

#endif
