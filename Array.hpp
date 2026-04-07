#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <string>
using namespace std;

const int MAX_LOGS = 5; 

struct LogRecord {
    string studentId;
    string questionId;
    bool isCorrect;
};

struct ArrayContainer {
    LogRecord logs[MAX_LOGS]; 
    int front;                
    int rear;                 
    int count;                
};

void initArray(ArrayContainer& cq);
void addLogToArray(ArrayContainer& cq, string sId, string qId, bool correct);
void removeLastLog(ArrayContainer& cq);

#endif