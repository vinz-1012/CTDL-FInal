#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "linkedlist.h"
#include <stdexcept>

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

    
    Ticket* findByPhone(string phone, int& count) {
        count = 0;

        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i].getHead();
            while (current) {
                if (current->ticket.phone == phone) {
                    count++;
                }
                current = current->next;
            }
        }

        if (count == 0) {
            throw std::runtime_error("Khong tim thay ve voi SDT nay.");
        }

      
        Ticket* results = new Ticket[count];
        int idx = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i].getHead();
            while (current) {
                if (current->ticket.phone == phone) {
                    results[idx++] = current->ticket;
                }
                current = current->next;
            }
        }

        return results;
    }

    int countTickets() {
        int count = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i].getHead();
            while (current) {
                count++;
                current = current->next;
            }
        }
        return count;
    }

    LinkedList* getTable() { return table; }
};

#endif
