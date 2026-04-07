#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Array.hpp"
#include <string>
using namespace std;

// ============================================================
// Task 3: Recent Activity Logging & Performance History
// Data Structure: Circular Queue (Fixed-Size Array)
// ============================================================

void addActivityLog(ArrayContainer& ac, string studentID, string questionID, bool isCorrect);
void deleteLastLog(ArrayContainer& ac);
void viewAllActivityLogs(ArrayContainer& ac);
void filterLogsByLearner(ArrayContainer& ac, string studentID);
void exportLogsToFile(ArrayContainer& ac);
void saveLogsToCSV(ArrayContainer& ac);
void task3(ArrayContainer& systemLog);

#endif