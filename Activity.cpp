#include "Activity.hpp"
#include "Logger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void clearList(Node*& head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}

void loadQuestionsFromCSV(Node*& activityQueue) {
    ifstream file("question.csv");
    if (!file.is_open()) {
        cout << "[Error] Unable to open question.csv" << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, topic, diff, desc, ans;

        getline(ss, id, ',');
        getline(ss, topic, ',');
        getline(ss, diff, ',');
        getline(ss, desc, ',');
        getline(ss, ans, ',');

        Question q(id, topic, diff, desc, ans);
        insertAtTail(activityQueue, q);
        count++;
    }

    file.close();
    cout << "[System] Task 2 initialized. " << count << " questions loaded into the Activity Queue." << endl;
}

void displayCurrentQuestion(Node* activityQueue) {
    if (activityQueue == nullptr) {
        cout << "[Info] No more questions! You have completed the activity." << endl;
        return;
    }
    displayQuestion(activityQueue->qData);
}

bool recordAndSave(Student& s, Question q, string ans) {
    bool isCorrect = (ans == q.answer);

    cout << "\n===  Activity Outcome Record ===" << endl;

    if (isCorrect) {
        s.score += 10;
        cout << "[Result] Correct! Brilliant work." << endl;
    } else {
        s.failedAttempts += 1;
        cout << "[Result] Incorrect. The correct answer was: " << q.answer << endl;
    }

    s.questionsCompleted += 1;

    cout << "Learner ID      : " << s.id << endl;
    cout << "Current Score   : " << s.score << endl;
    cout << "Failed Attempts : " << s.failedAttempts << endl;
    cout << "==================================" << endl;

    // Save into learner.csv (leader requirement)
    updateLearnerScoreInCSV(s.id, s.score);
    updateFailedAttemptsInCSV(s.id, s.failedAttempts);
    updateQuestionsCompletedInCSV(s.id, s.questionsCompleted);
    updateLearnerStateInCSV(s.id, "Active");

    cout << "[System] Learner progress saved to learner.csv" << endl;

    return isCorrect;
}

void moveToNextQuestion(Node*& activityQueue, Node*& historyStack) {
    if (activityQueue == nullptr) {
        cout << "[Info] You have finished all questions!" << endl;
        return;
    }

    Question completedQ = activityQueue->qData;

    insertAtHead(historyStack, completedQ);
    removeFromHead(activityQueue);

    cout << "[System] Moving to next question..." << endl;
}

void undoPreviousQuestion(Node*& activityQueue, Node*& historyStack, Student& s, ArrayContainer& systemLog) {
    if (historyStack == nullptr) {
        cout << "[Info] No previous question to undo." << endl;
        return;
    }

    Question prev = historyStack->qData;

    if (prev.wasCorrect) {
        s.score -= 10;
        if (s.score < 0) s.score = 0;
        cout << "[System] Undo: Reverted +10 score. Current Score: " << s.score << endl;
    } else {
        if (s.failedAttempts > 0) s.failedAttempts -= 1;
        cout << "[System] Undo: Reverted +1 failed attempt. Failed Attempts: " << s.failedAttempts << endl;
    }

    if (s.questionsCompleted > 0) s.questionsCompleted -= 1;

    removeFromHead(historyStack);
    insertAtHead(activityQueue, prev);

    // persist undo
    updateLearnerScoreInCSV(s.id, s.score);
    updateFailedAttemptsInCSV(s.id, s.failedAttempts);
    updateQuestionsCompletedInCSV(s.id, s.questionsCompleted);

    cout << "[System] Returned to the previous question." << endl;

    deleteLastLog(systemLog);
}

// ==========================================================
// TASK 2
// ==========================================================
void task2(Node*& activeSession, Node*& activityQueue, Node*& historyStack, ArrayContainer& systemLog) {

    cout << "\n--- Entering Task 2: Activity Flow ---" << endl;

    if (activeSession == nullptr) {
        cout << "[Error] No active learners! Please register learners first." << endl;
        return;
    }

    // Ask for learner ID
    string inputId;
    cout << "Enter your Learner ID: ";
    cin >> inputId;

    // Search for the learner in activeSession
    Node* found = nullptr;
    Node* temp = activeSession;
    while (temp != nullptr) {
        if (temp->data.id == inputId) {
            found = temp;
            break;
        }
        temp = temp->next;
    }

    if (found == nullptr) {
        cout << "[Error] Learner ID " << inputId << " is not in the Active Session." << endl;
        cout << "[Info] Only Active learners can start a learning activity." << endl;
        return;
    }

    Student& s = found->data;

    // load saved stats
    s.score = getScoreFromCSV(s.id);
    s.failedAttempts = getFailedAttemptsFromCSV(s.id);
    s.questionsCompleted = getQuestionsCompletedFromCSV(s.id);

    cout << "\n[Welcome] Learner " << s.name << " (ID: " << s.id << ") is taking the quiz!" << endl;
    cout << "[System] Preparing a fresh set of questions..." << endl;

    clearList(activityQueue);
    clearList(historyStack);

    loadQuestionsFromCSV(activityQueue);

    int skip = s.questionsCompleted;
    for (int i = 0; i < skip && activityQueue != nullptr; i++) {
        removeFromHead(activityQueue);
    }

    bool running = true;
    while (running) {

        cout << "\n[Task 2 Menu - Active: " << s.name << "]" << endl;
        cout << "1. View Current Question (Peek Queue)" << endl;
        cout << "2. Answer & Go Next (Queue -> Stack)" << endl;
        cout << "3. Undo Previous Question (Stack -> Queue)" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Select action: ";

        int choice = validateInput(1,4);

        if (choice == 1) {
            displayCurrentQuestion(activityQueue);

        } else if (choice == 2) {

            if (activityQueue == nullptr) {
                cout << "[Info] You have finished all questions!" << endl;
                continue;
            }

            displayCurrentQuestion(activityQueue);

            string ans;
            cout << "Your Answer: ";
            cin >> ans;

            bool ok = recordAndSave(s, activityQueue->qData, ans);
            activityQueue->qData.wasCorrect = ok;

            addActivityLog(systemLog, s.id, activityQueue->qData.id, ok);
            cout << "[System] Log sent to Circular Queue successfully!" << endl;

            moveToNextQuestion(activityQueue, historyStack);

        } else if (choice == 3) {

            undoPreviousQuestion(activityQueue, historyStack, s, systemLog);

        } else if (choice == 4) {

            cout << "Returning to Main Menu..." << endl;
            running = false;

        } else {
            cout << "Invalid choice!" << endl;
        }
    }
}