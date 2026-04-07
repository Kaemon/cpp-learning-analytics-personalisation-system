#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
using namespace std;

struct Student {
    string id;
    string name;
    int score;
    int failedAttempts;
    string riskLevel;
    string state;
    int questionsCompleted;

    Student(string i = "", string n = "", int s = 0) 
        : id(i), name(n), score(s), failedAttempts(0), 
          riskLevel("Low"), state("Waiting"), questionsCompleted(0) {}
};

#endif