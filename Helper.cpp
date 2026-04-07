#include "Helper.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// for input validation
int validateInput(int min, int max) {
    int choice;
    while (true) {
        cin >> choice;
        // Check if input is a valid integer
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(100, '\n'); // Discard invalid input
            cout << " > Invalid input! \n    Please enter number (" << min << "-" << max << "): ";
        } 
        // Check if input is within range
        else if (choice < min || choice > max) {
            cout << " > Out of range! \n    Please enter a number between " << min << " and " << max << ": ";
        } 
        else {
            return choice;
        }
    }
}

// Count nodes in a linked list
int countNodes(Node* head) {
    int count = 0;
    while (head != nullptr) {
        count++;
        head = head->next;
    }
    return count;
}

// Helper: rewrite entire learner.csv with updated field
// fieldIndex: 0=id, 1=name, 2=state, 3=score, 4=qCompleted, 5=failedAttempts
void rewriteLearnerCSV(string targetId, int fieldIndex, string newValue) {
    ifstream inFile("learner.csv");
    if (!inFile.is_open()) return;

    string allLines = "";
    string line;
    getline(inFile, line);
    allLines += line + "\n"; // header

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, name, state, score, qCompleted, failedAttempts;
        getline(ss, sid, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');

        if (sid == targetId) {
            if (fieldIndex == 0) sid = newValue;
            else if (fieldIndex == 1) name = newValue;
            else if (fieldIndex == 2) state = newValue;
            else if (fieldIndex == 3) score = newValue;
            else if (fieldIndex == 4) qCompleted = newValue;
            else if (fieldIndex == 5) failedAttempts = newValue;
        }
        allLines += sid + "," + name + "," + state + "," + score + "," + qCompleted + "," + failedAttempts + "\n";
    }
    inFile.close();

    ofstream outFile("learner.csv");
    outFile << allLines;
    outFile.close();
}

// Check if ID exists in learner.csv, returns state or "NOT_FOUND"
string checkIDInCSV(string id) {
    ifstream file("learner.csv");
    if (!file.is_open()) return "NOT_FOUND";

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, name, state, score, qCompleted, failedAttempts;
        getline(ss, sid, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');
        if (sid == id) {
            file.close();
            return state;
        }
    }
    file.close();
    return "NOT_FOUND";
}

// Update learner state
void updateLearnerStateInCSV(string id, string newState) {
    rewriteLearnerCSV(id, 2, newState);
}

// Update learner score
void updateLearnerScoreInCSV(string id, int newScore) {
    rewriteLearnerCSV(id, 3, to_string(newScore));
}

// Get learner score
int getScoreFromCSV(string id) {
    ifstream file("learner.csv");
    if (!file.is_open()) return 0;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, name, state, score, qCompleted, failedAttempts;
        getline(ss, sid, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');
        if (sid == id) {
            file.close();
            return score.empty() ? 0 : stoi(score);
        }
    }
    file.close();
    return 0;
}

// Update failed attempts
void updateFailedAttemptsInCSV(string id, int newCount) {
    rewriteLearnerCSV(id, 5, to_string(newCount));
}

// Get failed attempts
int getFailedAttemptsFromCSV(string id) {
    ifstream file("learner.csv");
    if (!file.is_open()) return 0;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, name, state, score, qCompleted, failedAttempts;
        getline(ss, sid, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');
        if (sid == id) {
            file.close();
            return failedAttempts.empty() ? 0 : stoi(failedAttempts);
        }
    }
    file.close();
    return 0;
}

// Update questions completed
void updateQuestionsCompletedInCSV(string id, int newCount) {
    rewriteLearnerCSV(id, 4, to_string(newCount));
}

// Get questions completed
int getQuestionsCompletedFromCSV(string id) {
    ifstream file("learner.csv");
    if (!file.is_open()) return 0;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, name, state, score, qCompleted, failedAttempts;
        getline(ss, sid, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');
        if (sid == id) {
            file.close();
            return qCompleted.empty() ? 0 : stoi(qCompleted);
        }
    }
    file.close();
    return 0;
}

// Load learners from learner.csv into memory on startup
void loadLearnersFromCSV(Node*& waitingQueue, Node*& activeSession) {
    ifstream file("learner.csv");
    if (!file.is_open()) return;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, state, score, qCompleted, failedAttempts;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, state, ',');
        getline(ss, score, ',');
        getline(ss, qCompleted, ',');
        getline(ss, failedAttempts, ',');

        if (id.empty()) continue;

        Student s(id, name, score.empty() ? 0 : stoi(score));
        s.state = state;
        s.questionsCompleted = qCompleted.empty() ? 0 : stoi(qCompleted);
        s.failedAttempts = failedAttempts.empty() ? 0 : stoi(failedAttempts);

        if (state == "Active") {
            insertAtTail(activeSession, s);
        } else if (state == "Waiting") {
            insertAtTail(waitingQueue, s);
        }
        // Inactive - don't load back
    }
    file.close();
    cout << "[System] Learner data loaded from learner.csv." << endl;
}

// Load logs from logger.csv into ArrayContainer on startup
void loadLogsFromCSV(ArrayContainer& systemLog) {
    ifstream file("logger.csv");
    if (!file.is_open()) return;

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, qid, correct;
        getline(ss, sid, ',');
        getline(ss, qid, ',');
        getline(ss, correct, ',');

        bool isCorrect = (correct == "True");
        addLogToArray(systemLog, sid, qid, isCorrect);
    }
    file.close();
    cout << "[System] Activity logs loaded from logger.csv." << endl;
}

int getTotalQuestions() {
    ifstream file("question.csv");
    if (!file.is_open()) return 0;
    string line;
    getline(file, line); // skip header
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) count++;
    }
    file.close();
    return count;
}