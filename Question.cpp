#include "Question.hpp"
#include <iostream>

Question::Question(string i, string t, string diff, string desc, string ans) {
    id = i;
    topic = t;
    difficulty = diff;
    description = desc;
    answer = ans;
    wasCorrect = false;
}

void displayQuestion(Question q) {
    cout << "=====================================" << endl;
    cout << "Question ID : " << q.id << " (" << q.difficulty << ")" << endl;
    cout << "Topic       : " << q.topic << endl;
    cout << "Description : " << q.description << endl;
    cout << "=====================================" << endl;
}