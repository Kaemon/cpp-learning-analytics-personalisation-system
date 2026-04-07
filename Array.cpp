#include "Array.hpp"
#include <iostream>

using namespace std;

void initArray(ArrayContainer& cq) {
    cq.front = 0;
    cq.rear = -1;
    cq.count = 0;
}

void addLogToArray(ArrayContainer& cq, string sId, string qId, bool correct) {
    cq.rear = (cq.rear + 1) % MAX_LOGS;
    
    cq.logs[cq.rear].studentId = sId;
    cq.logs[cq.rear].questionId = qId;
    cq.logs[cq.rear].isCorrect = correct;

    if (cq.count < MAX_LOGS) {
        cq.count++; 
    } else {
        cq.front = (cq.front + 1) % MAX_LOGS; 
    }
}
void removeLastLog(ArrayContainer& cq) {
    if (cq.count == 0) return; 
    
    cq.rear = (cq.rear - 1 + MAX_LOGS) % MAX_LOGS;
    
    cq.count--; 
}

