#ifndef SESSION_HPP
#define SESSION_HPP
#include "LinkedList.hpp"
#include "Helper.hpp"
#include <string>
using namespace std;


// Displays the current Active and Waiting lists
void displaySystemStatus(Node* activeSession, Node* waitingQueue);

// Handles adding a new learner (can't decide Active or Waiting, system auto handle, if session full, go to wait)
void registerLearner(Node*& waitingQueue, Node*& activeSession);

// Removes a learner by ID and updates their CSV status to "Inactive"
void removeLearner(Node*& waitingQueue, Node*& activeSession);

// Automatically moves the 1st person in Waiting to the Active session
string addActiveLearner(Node*& waitingQueue, Node*& activeSession);

// The main menu loop for Task 1
void task1(Node*& waitingQueue, Node*& activeSession);


#endif