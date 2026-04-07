#ifndef HELPER_HPP
#define HELPER_HPP

#include "LinkedList.hpp"
#include "Array.hpp"
#include <string>
using namespace std;

// for input validation
int validateInput(int min, int max); 

// Count nodes in a linked list
int countNodes(Node* head);

// Check if a student ID exists in learner.csv, returns state or "NOT_FOUND"
string checkIDInCSV(string id);

// Update a learner's state in learner.csv
void updateLearnerStateInCSV(string id, string newState);

// Update a learner's score in learner.csv
void updateLearnerScoreInCSV(string id, int newScore);

// Get a learner's score from learner.csv
int getScoreFromCSV(string id);

// Update a learner's failed attempts in learner.csv
void updateFailedAttemptsInCSV(string id, int newCount);

// Get a learner's failed attempts from learner.csv
int getFailedAttemptsFromCSV(string id);

// Update a learner's questions completed in learner.csv
void updateQuestionsCompletedInCSV(string id, int newCount);

// Get a learner's questions completed from learner.csv
int getQuestionsCompletedFromCSV(string id);

// Load learners from learner.csv into memory on startup
void loadLearnersFromCSV(Node*& waitingQueue, Node*& activeSession);

// Load logs from logger.csv into ArrayContainer on startup
void loadLogsFromCSV(ArrayContainer& systemLog);

int getTotalQuestions();
#endif