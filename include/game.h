//
// Created by karls on 23/03/2026.
//

#ifndef GAME_H
#define GAME_H

#include <vector>
#include "question.h"

#define QUESTIONS_CSV_FILE "../database/questions.csv"

using namespace std;

class Game {
private:
    vector<Question*> questions;
    int score;

    bool load_questions();
    void shuffle_questions();
    void play_round(Question* q);

public:
    Game();
    ~Game();

    void start();

    int get_score() const;
};

#endif //GAME_H
