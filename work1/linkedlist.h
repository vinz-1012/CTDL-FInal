#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Ticket.h"
#include <stdexcept>

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
    Ticket* findByPhone(string phone, int& n) {
        n = 0;
        Node* current = head;
        while (current) {
            if (current->ticket.phone == phone) n++;
            current = current->next;
        }
        if (n == 0) {
            throw std::runtime_error("Khong tim thay ve voi SDT nay.");
        }

        Ticket* results = new Ticket[n];
        int idx = 0;
        current = head;
        while (current) {
            if (current->ticket.phone == phone) {
                results[idx++] = current->ticket;
            }
            current = current->next;
        }
        return results;
    }
    Node* getHead() { return head; }
};

#endif
