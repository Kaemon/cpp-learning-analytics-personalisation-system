#include "LinkedList.hpp"
#include <iostream>

void insertAtHead(Node*& head, Student s) {
    Node* newNode = new Node(s);
    newNode->next = head;
    head = newNode;
}

void insertAtTail(Node*& head, Student s) {
    Node* newNode = new Node(s);
    if (!head) { head = newNode; return; }
    Node* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
}

bool removeFromHead(Node*& head) {
    if (!head) return false;
    Node* temp = head;
    head = head->next;
    delete temp;
    return true;
}

Node* findStudent(Node* head, string id) {
    Node* temp = head;
    while (temp) {
        if (temp->data.id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void displayList(Node* head) {
    Node* temp = head;
    while (temp) {
        std::cout << "ID: " << temp->data.id << " | Name: " << temp->data.name << std::endl;
        temp = temp->next;
    }
}

// ---------------------------------------------------------
// for Task 2 Question 
// ---------------------------------------------------------

void insertAtHead(Node*& head, Question q) {
    Node* newNode = new Node(q); 
    newNode->next = head;
    head = newNode;
}

void insertAtTail(Node*& head, Question q) {
    Node* newNode = new Node(q); 
    if (head == nullptr) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}