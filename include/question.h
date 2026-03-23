//
// Created by karls on 23/03/2026.
//

#ifndef QUESTION_H
#define QUESTION_H

#include <string>

using namespace std;

class Question {
private:
    string term;
    string hint;

public:
    Question(const string &term, const string &hint);
    string get_term() const;
    string get_hint() const;

    void set_term(const string &term);
    void set_hint(const string &hint);
};


#endif //QUESTION_H
