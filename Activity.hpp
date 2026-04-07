#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include "Question.hpp"
#include "LinkedList.hpp"
#include "Student.hpp"
#include "Helper.hpp"
#include "Array.hpp"

// Task 2 helpers (NO STL containers)
void clearList(Node*& head);
void loadQuestionsFromCSV(Node*& activityQueue);
void displayCurrentQuestion(Node* activityQueue);
void moveToNextQuestion(Node*& activityQueue, Node*& historyStack);
bool recordAndSave(Student& s, Question q, string ans);
void undoPreviousQuestion(Node*& activityQueue, Node*& historyStack, Student& currentStudent, ArrayContainer& systemLog);
void task2(Node*& activeSession, Node*& activityQueue, Node*& historyStack, ArrayContainer& systemLog);
#endif