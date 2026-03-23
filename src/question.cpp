//
// Created by karls on 23/03/2026.
//

#include "../include/question.h"

Question::Question(const string &term, const string &hint) {
    this->term = term;
    this->hint = hint;
}

string Question::get_hint() const {
    return this->hint;
}

string Question::get_term() const {
    return this->term;
}

void Question::set_hint(const string &hint) {
    this->hint = hint;
}

void Question::set_term(const string &term) {
    this->term = term;
}