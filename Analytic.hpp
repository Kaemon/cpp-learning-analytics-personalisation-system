#ifndef ANALYTIC_HPP
#define ANALYTIC_HPP

#include "LinkedList.hpp"
#include "Helper.hpp"

// add your function here...
// void xxx(...);

void task4(Node* priorityQueue);  

// 1. Calculate risk score based on performance
int calculateRiskScore(Student s);

// 2. Rank learners by risk level using Priority Queue
void rankLearnersByPriority(Node*& priorityHead, Student s);

// 3. Display highest-priority learners needing attention
void displayHighRiskLearners(Node* priorityHead);

// 4. Export at-risk list and recommendations to CSV
void exportRiskRecommendations(Node* priorityHead);

// 5. Load active learners from learner.csv and build priority queue
void buildPriorityQueueFromCSV(Node*& priorityHead);
#endif