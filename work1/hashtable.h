#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "linkedlist.h"

#define TABLE_SIZE 100

class HashTable {
private:
    LinkedList table[TABLE_SIZE];
    int hash(string code) {
        int sum = 0;
        for (char c : code) sum += c;
        return sum % TABLE_SIZE;
    }
public:
    void insert(Ticket t) {
        int idx = hash(t.code);
        table[idx].insert(t);
    }

    bool remove(string code) {
        int idx = hash(code);
        return table[idx].remove(code);
    }

    Ticket* find(string code) {
        int idx = hash(code);
        Node* node = table[idx].find(code);
        if (node) return &node->ticket;
        return nullptr;
    }
    void findByPhone(string phone) {
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].findByPhone(phone)) found = true;
        }
        if (!found) cout << "Khong tim thay ve nao voi SDT nay.\n";
    }

    void printAll() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i].print();
    }
};

#endif
