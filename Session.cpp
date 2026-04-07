#include "Session.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// 1. Display System Status
void displaySystemStatus(Node* activeSession, Node* waitingQueue) {
    cout << "\n=========================================" << endl;
    cout << "       CURRENT SESSION STATUS            " << endl;
    cout << "=========================================" << endl;

    // Use Helper.cpp to count active nodes
    int activeCount = countNodes(activeSession); 
    cout << "--- ACTIVE LEARNERS (" << activeCount << "/5) ---" << endl;
    if (activeCount == 0) {
        cout << "  [Empty]" << endl;
    } else {
        Node* temp = activeSession;
        int pos = 1;
        while (temp != nullptr) {
            cout << "  " << pos++ << ". [" << temp->data.id << "] " << temp->data.name << endl;
            temp = temp->next;
        }
    }

    // Use Helper.cpp to count waiting nodes
    int waitCount = countNodes(waitingQueue); 
    cout << "\n--- WAITING QUEUE (" << waitCount << ") ---" << endl;
    if (waitCount == 0) {
        cout << "  [Empty]" << endl;
    } else {
        Node* temp = waitingQueue;
        int pos = 1;
        while (temp != nullptr) {
            cout << "  " << pos++ << ". [" << temp->data.id << "] " << temp->data.name << endl;
            temp = temp->next;
        }
    }
    cout << "=========================================" << endl;
}


// 2. Automatically Move Learner from Wait to Active
string addActiveLearner(Node*& waitingQueue, Node*& activeSession) {
    if (waitingQueue == nullptr) {
        return ""; // No one is waiting, return an empty string
    }

    // Grab the data of the first person in line
    Student nextLearner = waitingQueue->data;

    // Remove them from the waiting queue (FIFO)
    removeFromHead(waitingQueue); 

    // Update their status and push to Active Session
    nextLearner.state = "Active";
    insertAtTail(activeSession, nextLearner); 

    // Update the CSV
    updateLearnerStateInCSV(nextLearner.id, "Active"); 

    // RETURN the message instead of cout
    return "[System Auto-Move] Learner " + nextLearner.id + " (" + nextLearner.name + ") has been moved to the Active Session!\n";
}


// 3. Register Learner to session
void registerLearner(Node*& waitingQueue, Node*& activeSession) {
    string newId;
    cout << "\n--- REGISTER LEARNER ---" << endl;
    cout << "Enter Learner ID (e.g., TP00): ";
    cin >> newId;
    cin.ignore(1000, '\n'); 
    
    // Check the current status of the ID in the csv
    string status = checkIDInCSV(newId);
    
    Student studentToAdd; 
    bool isNewLearner = false;
    
    string finalMessage = ""; 

    // --- CASE 1: ALREADY IN THE SYSTEM ---
    if (status == "Active" || status == "Waiting") {
        // displaySystemStatus(activeSession, waitingQueue);
        finalMessage += "\n[Error] Learner " + newId + " is already in the session (" + status + ")!";
        cout << finalMessage << endl;
        return;
    } 
    // --- CASE 2: REJOIN INACTIVE LEARNER ---
    else if (status == "Inactive") {
        finalMessage += "\n[Info] Existing learner found. Reactivating " + newId + "...";
        
        // fetch historical data so they don't lose their scores
        ifstream inFile("learner.csv");
        string line, id, name, state, scoreStr, qCompStr, failsStr;
        
        if (inFile.is_open()) {
            getline(inFile, line); // Skip the header row
            while (getline(inFile, line)) {
                stringstream ss(line);
                getline(ss, id, ',');
                if (id == newId) {
                    getline(ss, name, ',');
                    getline(ss, state, ',');
                    getline(ss, scoreStr, ',');
                    getline(ss, qCompStr, ',');
                    getline(ss, failsStr, ',');
                    
                    // Rebuild the student object with their old data
                    studentToAdd.id = id;
                    studentToAdd.name = name;
                    studentToAdd.score = stoi(scoreStr);
                    studentToAdd.questionsCompleted = stoi(qCompStr);
                    studentToAdd.failedAttempts = stoi(failsStr);
                    break;
                }
            }
            inFile.close();
        }
    } 
    // --- CASE 3: NEW LEARNER ---
    else { 
        isNewLearner = true;
        string newName;
        cout << "Enter New Learner Name: ";
        getline(cin, newName);
        
        // Build a fresh student object
        studentToAdd.id = newId;
        studentToAdd.name = newName;
        studentToAdd.score = 0;
        studentToAdd.questionsCompleted = 0;
        studentToAdd.failedAttempts = 0;
    }

    // ROUTING: Check capacity and assign position
    int currentActive = countNodes(activeSession); 

    if (currentActive < 5) {
        studentToAdd.state = "Active";
        insertAtTail(activeSession, studentToAdd); 
        finalMessage += "\n[Success] " + studentToAdd.name + " joined the ACTIVE session.\n";
    } else {
        studentToAdd.state = "Waiting";
        insertAtTail(waitingQueue, studentToAdd); 
        finalMessage += "\n[Success] Session is full. " + studentToAdd.name + " joined the WAITING list.\n";
    }

    // DATABASE UPDATE
    if (isNewLearner) {
        // Append the new learner to the bottom of learner.csv
        ofstream outFile("learner.csv", ios::app);
        if (outFile.is_open()) {
            outFile << studentToAdd.id << "," << studentToAdd.name << "," 
                    << studentToAdd.state << ",0,0,0\n";
            outFile.close();
        } else {
            finalMessage += "[Error] Could not open learner.csv to save data.\n";
        }
    } else {
        // Just update the existing learner's state from Inactive -> Active/Waiting
        updateLearnerStateInCSV(studentToAdd.id, studentToAdd.state);
    }

    // THE FIX: Output the updated dashboard FIRST, then the message
    displaySystemStatus(activeSession, waitingQueue);
    cout << finalMessage << endl;
}


// 4. Remove Learner (and trigger auto-move)
void removeLearner(Node*& waitingQueue, Node*& activeSession) {
    string targetId;
    cout << "\n--- REMOVE LEARNER ---" << endl;
    cout << "Enter Learner ID to remove: ";
    cin >> targetId;
    cin.ignore(1000, '\n');

    bool found = false;
    bool removedFromActive = false;

    // 1. Search and delete from Active Session Linked List
    Node* temp = activeSession;
    Node* prev = nullptr;
    while (temp != nullptr) {
        if (temp->data.id == targetId) { 
            if (prev == nullptr) { activeSession = temp->next; } 
            else { prev->next = temp->next; } 
            delete temp;
            found = true;
            removedFromActive = true;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // 2. If not found in Active, search the Waiting Queue
    if (!found) {
        temp = waitingQueue;
        prev = nullptr;
        while (temp != nullptr) {
            if (temp->data.id == targetId) {
                if (prev == nullptr) { waitingQueue = temp->next; } 
                else { prev->next = temp->next; }
                delete temp;
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    // 3. Gather Messages and Trigger Auto-Move
    string finalMessage = "";
    string autoMoveMessage = "";

    if (found) {
        // Update their state in the CSV so they don't load next time
        updateLearnerStateInCSV(targetId, "Inactive"); 
        finalMessage = "\n[Success] Learner " + targetId + " has been removed and set to Inactive.\n";
        
        // If we opened up a spot in the Active Session, fill it BEFORE showing the dashboard!
        if (removedFromActive) {
            autoMoveMessage = addActiveLearner(waitingQueue, activeSession);
        }
    } else {
        finalMessage = "\n[Error] Learner ID " + targetId + " not found in system.\n";
    }

    // 4. Print Dashboard First, then Messages!
    displaySystemStatus(activeSession, waitingQueue);
    cout << finalMessage;
    if (autoMoveMessage != "") {
        cout << autoMoveMessage << endl;
    }
}


// 5. Task 1 Main Menu
void task1(Node*& waitingQueue, Node*& activeSession) {
    bool inTask1 = true;
    int choice;
    displaySystemStatus(activeSession, waitingQueue);

    while (inTask1) {
        cout << "\n1. Register Learner" << endl;
        cout << "2. Inactive Learner" << endl;
        cout << "3. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        choice = validateInput(1,3);

        if (choice == 1) {
            registerLearner(waitingQueue, activeSession);
        } else if (choice == 2) {
            removeLearner(waitingQueue, activeSession);
        } else {
            inTask1 = false;
        }
    }
}




