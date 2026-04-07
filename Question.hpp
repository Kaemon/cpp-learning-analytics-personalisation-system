#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>
using namespace std;

struct Question {
    string id;          
    string topic;       
    string difficulty;  
    string description; 
    string answer; 
    bool wasCorrect;     

    Question(string i = "", string t = "", string diff = "", string desc = "", string ans = "");
};

void displayQuestion(Question q);

#endif