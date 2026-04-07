#include "Analytic.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// 1. Risk score calculation
int calculateRiskScore(Student s) {
    if (s.questionsCompleted == 0) return -1; // no activity special case

    int totalQuestions = getTotalQuestions();

    // unattempted questions count as failed
    int unattempted = 0;
    if (totalQuestions > s.questionsCompleted) {
        unattempted = totalQuestions - s.questionsCompleted;
    }

    int effectiveFails = s.failedAttempts + unattempted;
    int denominator = (totalQuestions > 0) ? totalQuestions : s.questionsCompleted;

    int failRate = (effectiveFails * 100) / denominator;
    return failRate - (s.score / 10);
}

// 2. Priority queue sorted insert
void rankLearnersByPriority(Node*& priorityHead, Student s) {
    Node* newNode = new Node(s);
    int currentRisk = calculateRiskScore(s);

    if (priorityHead == nullptr || calculateRiskScore(priorityHead->data) < currentRisk) {
        newNode->next = priorityHead;
        priorityHead = newNode;
        return;
    }

    Node* temp = priorityHead;
    while (temp->next != nullptr && calculateRiskScore(temp->next->data) >= currentRisk) {
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
}

// 3. Recommendation engine
string getRecommendation(int riskScore) {
    if (riskScore == -1) {
        return "NO ACTIVITY: Learner has not attempted any questions yet.";
    } else if (riskScore >= 50) {
        return "URGENT: Repeat previous topic and attempt an easier activity.";
    } else if (riskScore > 0) {
        return "WARNING: Review recent mistakes carefully before proceeding.";
    } else {
        return "GOOD: Excellent progress. Keep up the good work!";
    }
}

// 4. Display high risk learners
void displayHighRiskLearners(Node* priorityHead) {
    cout << "\n=== Task 4: High-Priority At-Risk Learners ===" << endl;
    if (priorityHead == nullptr) {
        cout << "No learners to analyze." << endl;
        return;
    }

    Node* temp = priorityHead;
    int rank = 1;
    while (temp != nullptr) {
        Student s = temp->data;
        int risk = calculateRiskScore(s);
        cout << "#" << rank << " | ID: " << s.id << " | Name: " << s.name
             << " | Score: " << s.score
             << " | Failed: " << s.failedAttempts
             << " | Risk Score: " << risk << endl;
        cout << "   -> Recommendation: " << getRecommendation(risk) << endl;
        cout << "-------------------------------------------------" << endl;
        temp = temp->next;
        rank++;
    }
}

// 5. Export to CSV
void exportRiskRecommendations(Node* priorityHead) {
    if (priorityHead == nullptr) {
        cout << "[Error] No data to export." << endl;
        return;
    }

    ofstream outFile("at_risk_learners.csv");
    if (!outFile.is_open()) {
        cout << "[Error] Could not create export file." << endl;
        return;
    }

    outFile << "Rank,Student_ID,Name,Score,FailedAttempts,Risk_Score,Recommendation\n";

    Node* temp = priorityHead;
    int rank = 1;
    while (temp != nullptr) {
        Student s = temp->data;
        int risk = calculateRiskScore(s);
        outFile << rank << ","
                << s.id << ","
                << s.name << ","
                << s.score << ","
                << s.failedAttempts << ","
                << risk << ","
                << "\"" << getRecommendation(risk) << "\"\n";
        temp = temp->next;
        rank++;
    }

    outFile.close();
    cout << "\n[Success] At-risk list exported to 'at_risk_learners.csv'!" << endl;
}

// 6. Build priority queue from learner.csv (Active learners only)
void buildPriorityQueueFromCSV(Node*& priorityHead) {
    ifstream file("learner.csv");
    if (!file.is_open()) {
        cout << "[Error] Cannot open learner.csv!" << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    int count = 0;
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

        if (!qCompleted.empty() && stoi(qCompleted) > 0) {
            Student s(id, name, score.empty() ? 0 : stoi(score));
            s.failedAttempts = failedAttempts.empty() ? 0 : stoi(failedAttempts);
            s.questionsCompleted = qCompleted.empty() ? 0 : stoi(qCompleted);
            rankLearnersByPriority(priorityHead, s);
            count++;
        }
    }

    file.close();
    cout << "[System] " << count << " active learners loaded for risk analysis." << endl;
}

void task4(Node* priorityQueue){
    // write your function here...
    // void xxx(){...}
    cout << "\n--- Entering Task 4: At-Risk Analytics ---" << endl;

                // Reset priority queue
                while (priorityQueue != nullptr) {
                    Node* toDelete = priorityQueue;
                    priorityQueue = priorityQueue->next;
                    delete toDelete;
                }
                priorityQueue = nullptr;

                // Build from CSV
                buildPriorityQueueFromCSV(priorityQueue);

                if (priorityQueue == nullptr) {
                    cout << "[Error] No active learners found in learner.csv!" << endl;
                    return;
                }

                bool task4Running = true;
                while (task4Running) {
                    cout << "\n[Task 4 Menu - Priority Queue]" << endl;
                    cout << "1. Display At-Risk Learners & Recommendations" << endl;
                    cout << "2. Export Risk Report to CSV" << endl;
                    cout << "3. Return to Main Menu" << endl;
                    cout << "Select action: ";

                    int t4Choice = validateInput(1,3);

                    if (t4Choice == 1) {
                        displayHighRiskLearners(priorityQueue);
                    } else if (t4Choice == 2) {
                        exportRiskRecommendations(priorityQueue);
                    } else if (t4Choice == 3) {
                        cout << "Returning to Main Menu..." << endl;
                        task4Running = false;
                    } else {
                        cout << "Invalid choice!" << endl;
                    }
                }

                // Clean up priority queue
                while (priorityQueue != nullptr) {
                    Node* toDelete = priorityQueue;
                    priorityQueue = priorityQueue->next;
                    delete toDelete;
                }
                priorityQueue = nullptr;
}