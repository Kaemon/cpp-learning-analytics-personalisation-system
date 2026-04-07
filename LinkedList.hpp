#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Student.hpp"
#include "Question.hpp" 

using namespace std;

struct Node {
    Student data;       
    Question qData;     
    Node* next;         

    Node(Student s) : data(s), next(nullptr) {}
    
    Node(Question q) : qData(q), next(nullptr) {}
};

void insertAtHead(Node*& head, Student s);  
void insertAtTail(Node*& head, Student s);  

void insertAtHead(Node*& head, Question q);
void insertAtTail(Node*& head, Question q);

bool removeFromHead(Node*& head);            
Node* findStudent(Node* head, string id);    
void displayList(Node* head);                

#endif