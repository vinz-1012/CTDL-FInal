#ifndef QUEUE_H
#define QUEUE_H

#include "ticket.h"

struct QueueNode {
    Ticket ticket;
    QueueNode* next;
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
public:
    Queue() { front = rear = nullptr; }
    ~Queue() {
        while (!isEmpty()) dequeue();
    }

    void enqueue(Ticket t) {
        QueueNode* node = new QueueNode{ t, nullptr };
        if (rear) rear->next = node;
        rear = node;
        if (!front) front = node;
    }

    Ticket dequeue() {
        if (isEmpty()) return Ticket{};
        QueueNode* temp = front;
        Ticket t = temp->ticket;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return t;
    }

    bool isEmpty() { return front == nullptr; }
};

#endif
