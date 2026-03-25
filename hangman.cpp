#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <regex>
#include <cstdlib>

#define FILENAME "words.csv"
#define LIVES 6
#define INDENT "    "

class Word
{
    // Private member variables
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

    // Show which letters have been guessed correctly
    void printBlanks() const
    {
        std::cout << INDENT;
        for (auto& c : m_blanks)
        {
            std::cout << c << " ";
        }
        std::cout << "\n";
    }

    // Attempt a guess then update state
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

    // If all letters have been uncovered, the game is won
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

// Extract Word object from a line in the CSV
Word getWord(const std::string& line)
{
    std::vector<std::string> strings = split(line, ';');
    return Word(strings[0], strings[1], strings[2]);
}

// Get a list of Word objects from the CSV
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

// Get a random Word object from the list
Word randWord(std::vector<Word> words)
{
    // Get random index
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister
    std::uniform_int_distribution<> dis(0, words.size() - 1);
    int rand_index = dis(gen);

    return words[rand_index];
}

// Check if the input is a valid letter
int isLetter(const std::string& input)
{
    std::regex r("[a-zA-Z]");
    return std::regex_match(input, r);
}

void clearScreen()
{
    #ifdef _WIN32 // Windows
        std::system("cls");
    #else // Linux
        std::system("clear");
    #endif
}

void printTitle()
{
    std::cout << R"(
    ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖ ▗▄▄▖▗▖  ▗▖ ▗▄▖ ▗▖  ▗▖
    ▐▌ ▐▌▐▌ ▐▌▐▛▚▖▐▌▐▌   ▐▛▚▞▜▌▐▌ ▐▌▐▛▚▖▐▌
    ▐▛▀▜▌▐▛▀▜▌▐▌ ▝▜▌▐▌▝▜▌▐▌  ▐▌▐▛▀▜▌▐▌ ▝▜▌
    ▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌▝▚▄▞▘▐▌  ▐▌▐▌ ▐▌▐▌  ▐▌

    Theme: History of Computing and Programming Languages
)" << "\n\n";
}

// Print the game over screen
void printGameOver(const std::string& frame, const std::string& message, const Round& round)
{
    clearScreen();
    printTitle();
    std::cout << frame << "\n";
    round.printBlanks();
    std::cout << "\n" << INDENT << message << "\n";
    std::cout << INDENT << "The word was \"" << round.getWord().getTerm() << "\"\n\n";
    std::cout << INDENT << "Fun Fact:\n";
    std::cout << INDENT << round.getWord().getDesc() << "\n";
}

bool promptRetry()
{
    std::string input;
    char letter;

    while (1)
    {
        std::cout << INDENT << "Retry? (Y/N) ";
        std::cin >> input;
        if (input.length() > 0)
        {
            letter = (char) toupper(input[0]);
            if (letter == 'Y') return true;
            if (letter == 'N') return false;
        }
    }
}

bool isFinished(const std::vector<Word>& finished, const Word& word)
{
    for (auto& w : finished)
    {
        if (w.getTerm() == word.getTerm()) return true;
    }
    return false;
}

int main(int argc, char * argv[])
{
    std::string frames[LIVES+1];
    frames[0] = R"(
    _____
    |/   |
    |
    |
    |
    |
    |
    |_____
)";
    frames[1] = R"(
    _____
    |/   |
    |    O
    |
    |
    |
    |
    |_____
)";
    frames[2] = R"(
    _____
    |/   |
    |    O
    |    |
    |    |
    |
    |
    |_____
)";
    frames[3] = R"(
    _____
    |/   |
    |    O
    |   /|
    |    |
    |
    |
    |_____
)";
    frames[4] = R"(
    _____
    |/   |
    |    O
    |   /|\
    |    |
    |
    |
    |_____
)";
    frames[5] = R"(
    _____
    |/   |
    |    O
    |   /|\
    |    |
    |   / 
    |
    |_____
)";
    frames[6] = R"(
    _____
    |/   |
    |    O
    |   /|\
    |    |
    |   / \
    |
    |_____
)";

    std::string letter;
    Word word;
    std::vector<Word> words;
    std::vector<Word> finished;

    words = getWords();
    while (1)
    {    
        // If all words are done, empty the list of finished words
        if (words.size() == finished.size())
        {
            finished.clear();
        }

        // Prevent repetition of finished words
        do 
        {
            word = randWord(words);
        } 
        while (isFinished(finished, word));
        finished.push_back(word);

        Round round(word);
        while (1)
        {
            // Display hangman and blanks
            clearScreen();
            printTitle();
            std::cout << frames[round.getMistakes()] << "\n";
            std::cout << INDENT << "Hint: " << round.getWord().getHint();
            std::cout << " (" << round.getWord().length() << ")\n";
            round.printBlanks();

            // Ask for a letter
            std::cout << "\n" << INDENT << "Enter a letter: ";
            std::cin >> letter;
            round.guess(letter[0]);
            
            // Check if the player won or lost
            if (round.gameWon())
            {
                printGameOver(frames[round.getMistakes()], "Congratulations, you win :)", round);
                break;
            }
            else if (round.getMistakes() == LIVES)
            {
                printGameOver(frames[round.getMistakes()], "You failed to guess the word :(", round);
                break;
            }
        }

        // Ask player if they wish to retry
        if (!promptRetry()) break;
    }

    return 0;
}