//
// Created by karls on 23/03/2026.
//

#include <fstream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>

#include "../include/game.h"

// Constructor
Game::Game() {
    if (load_questions()) {
        score = 0;
        shuffle_questions();
    } else {
        cerr << "Error opening the file!" << endl;
    }
}

// Deconstructor
Game::~Game() {
    for (int i = 0; i < questions.size(); i++) {
        delete questions[i];
    }
}

bool Game::load_questions() {
    ifstream file(QUESTIONS_CSV_FILE);
    if (!file.is_open()) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row_data;

        while (getline(ss, cell, ',')) {
            row_data.push_back(cell);
        }

        if (row_data.size() >= 2) {
            questions.push_back(new Question(row_data[0], row_data[1]));
        }
    }

    file.close();
    return true;
}

void Game::shuffle_questions() {
    random_device rd;
    mt19937 g(rd());
    ranges::shuffle(questions, g);
}

void Game::play_round(Question *q) {
    // TODO
    cout << q->get_hint() << " | " << q->get_term() << endl;
}

void Game::start() {
    // TODO

    // TEST CODE
    int i = 0;
    while(i < 10) {
        shuffle_questions();
        for (int i = 0; i < questions.size(); i++) {
            play_round(questions[i]);
        }
        cout << endl;
        i++;
    }
}

int Game::get_score() const {
    return score;
}

