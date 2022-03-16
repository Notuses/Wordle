#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define FOREGROUND(color, text) "\x1B[" << static_cast<int>(color) << "m" << text << "\033[0m"
#define BACKGROUND(color, text) "\033[3;42;" << static_cast<int>(color) << "m" << text << "\033[0m"

enum class ForegroundColor : int {
    Red = 31,
    Green = 32,
    Yellow = 33,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93
};

enum class BackgroundColor : int {
    Gray = 7,
    Red = 41,
    Green = 42,
    Yellow = 43,
    BrightRed = 101,
    BrightGreen = 102,
    BrightYellow = 103
};

using namespace std;

string guess = "?????";
char chars[4]{};
string theWord;
int lives = 6;

void TheGame(string);
int main();

// Get a random word from the text file
string GetTheWord() {
    string word;
    ifstream wordsFile;
    wordsFile.open("words.txt");

    if (wordsFile.is_open()) {
        vector<string> words;
        string aWord;
        while (getline(wordsFile, aWord)) {
            words.push_back(aWord);
        }
        srand(time(NULL));
        word = words[rand() % words.size() - 1];
    }
    else
    {
        cout << FOREGROUND(ForegroundColor::Red, "ERROR: COULD NOT FIND WORDS FILE!\n") << endl;
    }
    return word;
}

// Letters in the gussed word but on the wrong place is marked with a yellow background. Letters in the gussed word and in the correct place is marked with a green background.
void CheckChars() {
    cout << endl << theWord << endl;
    for (int i = 0; i <= 4; i++)
    {
        chars[i] = guess[i];

        if (chars[i] == theWord[i])
            cout << BACKGROUND(BackgroundColor::Green, chars[i]);
        else if (theWord.find(chars[i]) != string::npos)
            cout << BACKGROUND(BackgroundColor::Yellow, chars[i]);
        else
            cout << BACKGROUND(BackgroundColor::Gray, chars[i]);

        
        
    }
}

void Restart() {
    char yesorno;
    do
    {
        cout << endl << "Do you want to restart the game?" << endl << "Input[Y/N]: ";
        cin >> yesorno;
        yesorno = tolower(yesorno);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (!cin.fail() && yesorno != 'y' && yesorno != 'n');

    if (yesorno == 'y')
    {
        lives = 6;
        guess = "?????";
        main();
    }
}

void Win() {
    cout << "\nYou won! You guessed the correct word: " << theWord << endl;
    Restart();
}

void Loose() {
    cout << "\nYou lost! Correct word was: " << theWord << "." << endl;
    Restart();
}

// check the guessed word and make uppercase
void Guess() {
    string resultTxt;
    
    for (auto& c : guess) c = toupper(c);
    if (guess == theWord) {
        Win();
        return;
    }

    lives--;
    resultTxt = "Your guess: " + guess + " is incorrect!";
    
    if (lives <= 0) {
        Loose();
        return;
    }

    TheGame(resultTxt);
}

// check if guessed word is correct length
bool ValidateGuess(string theGuess) {
    if (theGuess.length() != 5)
    {
        TheGame("Invalid input! Please try again\n");
        return false;
    }
    guess = theGuess;
    return true;
}

void TheGame(string result = "") {
    cout << FOREGROUND(ForegroundColor::Yellow, "You are playing Wordle! Guess the FIVE letter word!\n");
    cout << FOREGROUND(ForegroundColor::Red, "Tries left: " << lives << "\n\n");

    cout << FOREGROUND(ForegroundColor::BrightRed, "The secret word: ");
    cin.clear();
    CheckChars();
    cout << "\n\n" << result << "\n" << "Input guess: ";
    string theGuess;
    cin >> theGuess;
    
    if (ValidateGuess(theGuess))
        Guess();
}

int main()
{
    theWord = GetTheWord();
    if (theWord.empty())
        return 0;
    TheGame("");
    
    return 0;
}