#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <regex>

#define FILENAME "words.csv"
#define LIVES 6

class Word
{
    std::string m_term = "";
    std::string m_hint = "";
    std::string m_desc = "";

public:
    Word() {}

    Word(const std::string& term, const std::string& hint, const std::string& desc)
        :m_term(term)
        ,m_hint(hint)
        ,m_desc(desc)
    {
    }

    std::string getTerm() const
    {
        return m_term;
    }

    std::string getHint() const
    {
        return m_hint;
    }

    std::string getDesc() const
    {
        return m_desc;
    }

    int length() const
    {
        return m_term.length();
    }

    void print() const
    {
        std::cout << m_term << " " << m_hint << " " << m_desc << "\n";
    }
};

class Round
{
    Word m_word;
    std::vector<char> m_blanks;
    int m_mistakes = 0;

public:
    Round(const Word& word)
    {    
        m_word = word; 
        for (int i=0; i<word.length(); i++)
        {
            m_blanks.push_back('_');
        }
    }

    Word getWord() const
    {
        return m_word;
    }

    int getMistakes() const
    {
        return m_mistakes;
    }

    void printBlanks() const
    {
        for (auto& c : m_blanks)
        {
            std::cout << c << " ";
        }
        std::cout << "\n";
    }

    bool guess(char letter)
    {
        bool guessed = false;
        letter = (char) toupper(letter);
        for (int i=0; i<m_blanks.size(); i++)
        {
            char c = (char) toupper(m_word.getTerm()[i]);
            if (m_blanks[i] == '_')
            {
                if (letter == c)
                {
                    m_blanks[i] = letter;
                    guessed = true;
                }
            }
        }
        if (!guessed) m_mistakes++;
        return guessed;
    }

    bool gameWon() const
    {
        for (auto& c : m_blanks)
        {
            if (c == '_') return false;
        }
        return true;
    }
};

std::vector<std::string> split(const std::string& str, char del)
{
    std::vector<std::string> strings;
    std::string part = "";

    for (auto& c : str)
    {
        if (c == del)
        {
            strings.push_back(part);
            part = "";
        }
        else if (c != '\n')
        {
            part += c;
        }
    }
    strings.push_back(part);

    return strings;
}

Word getWord(const std::string& line)
{
    std::vector<std::string> strings = split(line, ',');
    return Word(strings[0], strings[1], strings[2]);
}

std::vector<Word> getWords()
{
    std::ifstream File(FILENAME);
    std::string line;
    std::vector<Word> words; 

    while (std::getline(File, line))
    {
        words.push_back(getWord(line));
    }

    File.close();

    return words;
}

Word randWord()
{
    std::vector<Word> words = getWords();

    // Get random index
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister
    std::uniform_int_distribution<> dis(0, words.size() - 1);
    int rand_index = dis(gen);

    return words[rand_index];
}

int isLetter(const std::string& input)
{
    std::regex r("[a-zA-Z]");
    return std::regex_match(input, r);
}

void printGameOver(const std::string& message, const Round& round)
{
    round.printBlanks();
    std::cout << "\n" << message << "\n";
    std::cout << "The word was \"" << round.getWord().getTerm() << "\"\n\n";
    std::cout << "Fun Fact:\n";
    std::cout << round.getWord().getDesc() << "\n";
}

bool promptRetry()
{
    std::string input;
    char letter;

    while (1)
    {
        std::cout << "Retry? (Y/N) ";
        std::cin >> input;
        if (input.length() > 0)
        {
            letter = (char) toupper(input[0]);
            if (letter == 'Y') return true;
            if (letter == 'N') return false;
        }
    }
}

int main(int argc, char * argv[])
{
    std::cout << R"(
    ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖ ▗▄▄▖▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖
    ▐▌ ▐▌▐▌ ▐▌▐▛▚▖▐▌▐▌   ▐▛▚▞▜▌▐▌ ▐▌▐▛▚▖▐▌
    ▐▛▀▜▌▐▛▀▜▌▐▌ ▝▜▌▐▌▝▜▌▐▌  ▐▌▐▛▀▜▌▐▌ ▝▜▌
    ▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌▝▚▄▞▘▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌
)" << "\n\n";

    std::string frames[LIVES+1];
    frames[0] = R"(
     ____
    |/   |
    |
    |
    |
    |
    |
    |_____
)";
    frames[1] = R"(
     ____
    |/   |
    |    O
    |
    |
    |
    |
    |_____
)";
    frames[2] = R"(
     ____
    |/   |
    |    O
    |    |
    |    |
    |
    |
    |_____
)";
    frames[3] = R"(
    ____
    |/   |
    |    O
    |   /|
    |    |
    |
    |
    |_____
)";
    frames[4] = R"(
    ____
    |/   |
    |    O
    |   /|\
    |    |
    |
    |
    |_____
)";
    frames[5] = R"(
    ____
    |/   |
    |    O
    |   /|\
    |    |
    |   / 
    |
    |_____
)";
    frames[6] = R"(
    ____
    |/   |
    |    O
    |   /|\
    |    |
    |   / \
    |
    |_____
)";

    std::string letter;
    
    while (1)
    {
        Round round(randWord());
        while (1)
        {
            // Display hangman and blanks
            std::cout << frames[round.getMistakes()] << "\n";
            std::cout << "Hint: " << round.getWord().getHint() << "\n";
            round.printBlanks();

            // Ask for a letter
            std::cout << "\nEnter a letter: ";
            std::cin >> letter;
            round.guess(letter[0]);
            
            // Check if the player won or lost
            if (round.gameWon())
            {
                std::cout << frames[round.getMistakes()] << "\n";
                printGameOver("Congratulations, You Win! :)", round);
                break;
            }
            else if (round.getMistakes() == LIVES)
            {
                std::cout << frames[round.getMistakes()] << "\n";
                printGameOver("You Lost! :(", round);
                break;
            }
        }

        // Ask player if they wish to retry
        if (!promptRetry()) break;
    }

    return 0;
}