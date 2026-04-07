#include "Logger.hpp"
#include "Helper.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// ============================================================
// TASK 3: Recent Activity Logging & Performance History
// Data Structure: Circular Queue (Fixed-Size Array)
//
// WHY Circular Queue?
//   - Fixed memory (MAX_LOGS = 5): no dynamic allocation needed
//   - When full, the OLDEST log is automatically overwritten
//   - O(1) enqueue and dequeue via front/rear index arithmetic
//   - Modulo (% MAX_LOGS) handles the circular wrap perfectly
// ============================================================


// ============================================================
// 1. Add Activity Log
//    - Calls the Array foundation function to store the entry
//    - Then immediately saves to logger.csv for persistence
//    - This function is also called by Task 2 after each answer
// ============================================================
void addActivityLog(ArrayContainer& ac, string studentID, string questionID, bool isCorrect) {
    addLogToArray(ac, studentID, questionID, isCorrect);
    saveLogsToCSV(ac);
}


// ============================================================
// 2. View All Activity Logs
//    - Starts at ac.front, loops exactly ac.count times
//    - Uses (current + 1) % MAX_LOGS to wrap around circularly
// ============================================================
void viewAllActivityLogs(ArrayContainer& ac) {
    cout << "\n===  Task 3: All Activity Logs ===" << endl;
    cout << "  [Circular Queue | Capacity: " << MAX_LOGS
         << " | Current Logs: " << ac.count << "]" << endl;
    cout << "------------------------------------" << endl;

    if (ac.count == 0) {
        cout << "  [Info] No activity logs found. Complete a learning activity first!" << endl;
        return;
    }

    int current = ac.front;
    for (int i = 0; i < ac.count; i++) {
        LogRecord log = ac.logs[current];

        cout << "  [" << (i + 1) << "] "
             << "Learner: " << left << setw(12) << log.studentId
             << "| Q-ID: "  << setw(8)  << log.questionId
             << "| Result: " << (log.isCorrect ? "Correct" : "Wrong")
             << endl;

        // Circular wrap: moves to next slot, resets to 0 after index 4
        current = (current + 1) % MAX_LOGS;
    }
    cout << "------------------------------------" << endl;
}


// ============================================================
// 3. Filter Logs by Learner ID
//    - Same circular traversal as viewAllActivityLogs
//    - Only prints entries where studentId matches the search
//    - Shows a summary (correct/wrong count) at the end
// ============================================================
void filterLogsByLearner(ArrayContainer& ac, string studentID) {
    cout << "\n===  Task 3: Filtered Logs for Learner [" << studentID << "] ===" << endl;
    cout << "---------------------------------------------------" << endl;

    ifstream file("logger.csv");
    if (!file.is_open()) {
        cout << "  [Info] No logs in the system yet." << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    int found   = 0;
    int correct = 0;
    int wrong   = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, qid, result;
        getline(ss, sid, ',');
        getline(ss, qid, ',');
        getline(ss, result, ',');

        if (sid == studentID) {
            bool isCorrect = (result == "True");
            cout << "  [" << (found + 1) << "] "
                 << "Q-ID: " << left << setw(8) << qid
                 << "| Result: " << (isCorrect ? "Correct" : "Wrong")
                 << endl;

            if (isCorrect) correct++;
            else           wrong++;
            found++;
        }
    }
    file.close();

    if (found == 0) {
        cout << "  [Info] No logs found for Learner ID: " << studentID << endl;
    } else {
        cout << "---------------------------------------------------" << endl;
        cout << "  Summary -> Total: " << found
             << " | Correct: "        << correct
             << " | Wrong: "          << wrong << endl;
    }
    cout << "---------------------------------------------------" << endl;
}


// ============================================================
// 4. Export Logs to activity_logs.csv
//    - Writes all current circular queue entries to CSV
//    - Includes row number, student ID, question ID, result
// ============================================================
void exportLogsToFile(ArrayContainer& ac) {
    ifstream logFile("logger.csv");
    if (!logFile.is_open()) {
        cout << "  [Error] No logs to export. Complete some activities first!" << endl;
        return;
    }

    ofstream outFile("activity_logs.csv");
    if (!outFile.is_open()) {
        cout << "  [Error] Could not create activity_logs.csv." << endl;
        logFile.close();
        return;
    }

    outFile << "No,Student_ID,Student_Name,Question_ID,Result\n";

    string line;
    getline(logFile, line); // skip header
    int count = 0;

    while (getline(logFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, qid, result;
        getline(ss, sid, ',');
        getline(ss, qid, ',');
        getline(ss, result, ',');

        // Look up student name from learner.csv
        string studentName = "Unknown";
        ifstream learnerFile("learner.csv");
        if (learnerFile.is_open()) {
            string lline;
            getline(learnerFile, lline); // skip header
            while (getline(learnerFile, lline)) {
                if (lline.empty()) continue;
                stringstream ls(lline);
                string lid, lname;
                getline(ls, lid, ',');
                getline(ls, lname, ',');
                if (lid == sid) {
                    studentName = lname;
                    break;
                }
            }
            learnerFile.close();
        }

        count++;
        outFile << count << "," << sid << "," << studentName << "," << qid << "," << result << "\n";
    }

    logFile.close();
    outFile.close();

    if (count == 0) {
        cout << "  [Error] No logs to export. Complete some activities first!" << endl;
    } else {
        cout << "  [Success] Full log history exported to 'activity_logs.csv' | Total entries: " << count << endl;
    }
}


// ============================================================
// 5. Save Logs to logger.csv  (Persistence Layer)
//    - Rewrites logger.csv with the current circular queue state
//    - Called automatically after every addActivityLog() and deleteLastLog()
//    - Works together with loadLogsFromCSV() in Helper.cpp on startup
// ============================================================
void saveLogsToCSV(ArrayContainer& ac) {
    // Only save the latest log entry (append mode - don't overwrite history)
    if (ac.count == 0) return;

    // Check if file exists and has header
    ifstream checkFile("logger.csv");
    bool hasHeader = false;
    if (checkFile.is_open()) {
        string firstLine;
        if (getline(checkFile, firstLine) && firstLine.find("Student_ID") != string::npos) {
            hasHeader = true;
        }
        checkFile.close();
    }

    ofstream file("logger.csv", ios::app);
    if (!file.is_open()) return;

    if (!hasHeader) {
        file << "Student_ID,Question_ID,Is_Correct\n";
    }

    // Only append the latest log (rear index)
    LogRecord latest = ac.logs[ac.rear];
    file << latest.studentId  << ","
         << latest.questionId << ","
         << (latest.isCorrect ? "True" : "False") << "\n";

    file.close();
}


// ============================================================
// 6. Delete Last Log  (Undo Feature - called by Task 2)
//    - Removes the most recently added log from the circular queue
//    - Syncs logger.csv immediately after deletion
// ============================================================
void deleteLastLog(ArrayContainer& ac) {
    if (ac.count == 0) {
        cout << "  [Info] No logs to delete." << endl;
        return;
    }

    // Remove from circular queue
    removeLastLog(ac);

    // Remove last line from logger.csv
    ifstream inFile("logger.csv");
    if (inFile.is_open()) {
        string allLines = "";
        string line;
        string lastLine = "";

        getline(inFile, line);
        allLines += line + "\n"; // header

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            if (!lastLine.empty()) {
                allLines += lastLine + "\n"; // write previous line
            }
            lastLine = line; // hold current line, don't write yet
        }
        // lastLine is now the last entry - skip it (this is the undo)
        inFile.close();

        ofstream outFile("logger.csv");
        outFile << allLines;
        outFile.close();
    }

    cout << "[System] Task 3: The last activity log has been erased." << endl;
}


// ============================================================
// 7. task3()  -  Sub-Menu Entry Point
//    Called from main.cpp switch case 3
//    Pass systemLog from main so all functions share the same container
// ============================================================
void task3(ArrayContainer& systemLog) {
    int choice;
    bool running = true;

    while (running) {
        cout << "\n=========================================" << endl;
        cout << "   Task 3: Recent Activity Logs Menu     " << endl;
        cout << "    [Data Structure: Circular Queue]     " << endl;
        cout << "=========================================" << endl;
        cout << "  1. View All Current Logs"               << endl;
        cout << "  2. Filter Logs by Learner ID"           << endl;
        cout << "  3. Export Logs to CSV File"             << endl;
        cout << "  4. Return to Main Menu"                 << endl;
        cout << "=========================================" << endl;
        cout << "  Select action: ";
        choice = validateInput(1,4);

        switch (choice) {
            case 1:
                viewAllActivityLogs(systemLog);
                break;

            case 2: {
                string searchId;
                cout << "\n  Enter Learner ID to search: ";
                cin >> searchId;
                filterLogsByLearner(systemLog, searchId);
                break;
            }

            case 3:
                exportLogsToFile(systemLog);
                break;

            case 4:
                cout << "\n  [Back] Returning to Main Menu..." << endl;
                running = false;
                break;

            default:
                cout << "\n  [Error] Invalid choice! Please enter 1-4." << endl;
        }
    }
}