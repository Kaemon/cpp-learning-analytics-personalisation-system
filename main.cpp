#include <iostream>
#include <string>

#include "Student.hpp"
#include "Question.hpp"
#include "LinkedList.hpp"
#include "Array.hpp"
#include "Helper.hpp"

#include "Session.hpp"
#include "Activity.hpp"
#include "Logger.hpp"
#include "Analytic.hpp"

using namespace std;

int main() {
    // ==========================================
    // Core containers
    // ==========================================
    Node* waitingQueue = nullptr;
    Node* activeSession = nullptr;

    Node* activityQueue = nullptr;
    Node* historyStack = nullptr;

    ArrayContainer systemLog;
    initArray(systemLog);

    Node* priorityQueue = nullptr;

    // Load persisted data on startup
    loadLearnersFromCSV(waitingQueue, activeSession);
    loadLogsFromCSV(systemLog);

    int choice;
    bool running = true;

    while (running) {
        cout << "\n=========================================" << endl;
        cout << "       PLAPS Learning System Menu        " << endl;
        cout << "=========================================" << endl;
        cout << "1. Learner Registration (Task 1 - Queue)" << endl;
        cout << "2. Start Learning Activity (Task 2 - Queue)" << endl;
        cout << "3. View Activity Logs (Task 3 - Circular Queue)" << endl;
        cout << "4. View At-Risk Analytics (Task 4 - Priority Queue)" << endl;
        cout << "5. Exit System" << endl;
        cout << "=========================================" << endl;
        cout << "Enter your choice (1-5): ";

        choice = validateInput(1, 5);

        switch (choice) {

            // ==========================================
            // TASK 1: Registration
            // ==========================================
            case 1: {
                task1(waitingQueue, activeSession);
                break;
            }

            // ==========================================
            // TASK 2: Activity
            // ==========================================
            case 2: {
                task2(activeSession, activityQueue, historyStack, systemLog);
                break;
            }

            // ==========================================
            // TASK 3: Logs
            // ==========================================
            case 3: {
                task3(systemLog);
                break;
            }

            // ==========================================
            // TASK 4: Analytics
            // ==========================================
            case 4: {
                task4(priorityQueue);
                break;
            }

            case 5:
                cout << "Exiting PLAPS System. Goodbye!" << endl;
                running = false;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}