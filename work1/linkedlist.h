#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ticket.h"
#include <iostream>
using namespace std;

struct Node {
    Ticket ticket;
    Node* next;
};

class LinkedList {
private:
    Node* head;
public:
    LinkedList() { head = nullptr; }
    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(Ticket t) {
        Node* node = new Node{ t, head };
        head = node;
    }

    bool remove(string code) {
        Node* current = head;
        Node* prev = nullptr;
        while (current) {
            if (current->ticket.code == code) {
                if (prev) prev->next = current->next;
                else head = current->next;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    Node* find(string code) {
        Node* current = head;
        while (current) {
            if (current->ticket.code == code) return current;
            current = current->next;
        }
        return nullptr;
    }
    bool findByPhone(string phone) {
        Node* current = head;
        bool found = false;
        while (current) {
            if (current->ticket.phone == phone) {
                string seatCode = string(1, 'A' + current->ticket.row) + to_string(current->ticket.col + 1);
                cout << "Code: " << current->ticket.code
                    << ", Name: " << current->ticket.name
                    << ", Ghe " << seatCode << endl;
                found = true;
            }
            current = current->next;
        }
        return found;
    }

    void print() {
        Node* current = head;
        while (current) {
            string seatCode = string(1, 'A' + current->ticket.row) + to_string(current->ticket.col + 1);
            cout << "Code: " << current->ticket.code
                << ", Name: " << current->ticket.name
                << ", Phone: " << current->ticket.phone
                << ", Ghe " << seatCode << endl;
            current = current->next;
        }
    }
};

#endif
