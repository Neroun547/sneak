#include <cctype>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include "doubleLinkedList.h"
#include <clocale>

const std::string DIRECTION_LEFT = "LEFT";
const std::string DIRECTION_RIGHT = "RIGHT";
const std::string DIRECTION_UP = "UP";
const std::string DIRECTION_DOWN = "DOWN";

bool isPaused = false;

char pressedKeyInLowerCase = 'a';
std::string currentDirection = DIRECTION_LEFT;

const int SLEEP_MICROSECONDS_ANIMATION_TIME = 100000;
const double SNEAK_STEP = 1;

const int boardRows = 30;
const int boardCols = 30;

std::string board[boardRows][boardCols] = {
    { "|", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "|" },
    { "|", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "|" }
};

struct position target;

void enableRawMode();
void drawBoard();
void gameLoop();
void sneakGoLeft();
void sneakGoRight();
void sneakGoUp();
void sneakGoDown();
void readKeywordFromConsole();
void sneakReachTarget();
void drawScoreCount();
void loseGame();
void changeSneakDirectionSideEffects();
void winGame();

const std::string sneakUnicode = "\u2B1C";
const std::string targetUnicode = "\u2B24";

DoubleLinkedList sneak;

int score = 0;

int main() {
    system("clear");

    struct position sneakStartPositionFirstItem;

    target.col = 15;
    target.row = 15;

    sneakStartPositionFirstItem.col = 4;
    sneakStartPositionFirstItem.row = 4;

    struct position sneakStartPositionSecondItem;

    sneakStartPositionSecondItem.col = 4;
    sneakStartPositionSecondItem.row = 3;

    sneak = DoubleLinkedList();

    sneak.push(sneakStartPositionFirstItem);
    sneak.push(sneakStartPositionSecondItem);

    enableRawMode();
    drawBoard();

    std::thread(readKeywordFromConsole).detach();

    gameLoop();

    return 0;
}

void drawScoreCount() {
    std::cout << std::endl;
    std::cout << "Score:" << score << std::endl;
}

void drawBoard() {
    for(int i = 0; i < boardRows; i++) {
        for(int j = 0; j < boardCols; j++) {
            struct position value;

            value.row = i;
            value.col = j;

            if(sneak.existsByValue(value)) {
                std::cout << sneakUnicode;
            } else if(target.row == i && target.col == j) {
                std::cout << targetUnicode;
            } else {
                std::cout << board[i][j];
            }
        }
        std::cout << "\n";
    }
}

void enableRawMode() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void gameLoop() {
    if(isPaused) {
        usleep(SLEEP_MICROSECONDS_ANIMATION_TIME);

        gameLoop();

        return;
    }
    if(pressedKeyInLowerCase == 'a' && currentDirection != DIRECTION_RIGHT) {
        // left
        sneakGoLeft();
        currentDirection = DIRECTION_LEFT;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(pressedKeyInLowerCase == 'w' && currentDirection != DIRECTION_DOWN) {
        // up
        sneakGoUp();
        currentDirection = DIRECTION_UP;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(pressedKeyInLowerCase == 'd' && currentDirection != DIRECTION_LEFT) {
        // right
        sneakGoRight();
        currentDirection = DIRECTION_RIGHT;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(pressedKeyInLowerCase == 's' && currentDirection != DIRECTION_UP) {
        // down
        sneakGoDown();
        currentDirection = DIRECTION_DOWN;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(currentDirection == DIRECTION_DOWN) {
        // down
        sneakGoDown();
        currentDirection = DIRECTION_DOWN;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(currentDirection == DIRECTION_LEFT) {
        // left
        sneakGoLeft();
        currentDirection = DIRECTION_LEFT;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(currentDirection == DIRECTION_RIGHT) {
        // right
        sneakGoRight();
        currentDirection = DIRECTION_RIGHT;
        changeSneakDirectionSideEffects();
        gameLoop();
    } else if(currentDirection == DIRECTION_UP) {
        // up
        sneakGoUp();
        currentDirection = DIRECTION_UP;
        changeSneakDirectionSideEffects();
        gameLoop();
    }
}

void sneakGoLeft() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();
    DoubleLinkedListItem* sneakItem = sneakHead -> prev;

    int prevRow = sneakHead -> value.row;
    int prevCol = sneakHead -> value.col;

    if(sneakHead -> value.col - 1 < 0) {
        sneakHead -> value.col = boardCols;
    } else {
        sneakHead -> value.col = sneakHead -> value.col - SNEAK_STEP;
    }
    int tmpPrevRow;
    int tmpPrevCol;

    while(sneakItem != nullptr) {
        tmpPrevRow = sneakItem -> value.row;
        tmpPrevCol = sneakItem -> value.col;

        sneakItem -> value.row = prevRow;
        sneakItem -> value.col = prevCol;

        sneakItem = sneakItem -> prev;

        prevRow = tmpPrevRow;
        prevCol = tmpPrevCol;
    }
}

void sneakGoDown() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();
    DoubleLinkedListItem* sneakItem = sneakHead -> prev;

    int prevRow = sneakHead -> value.row;
    int prevCol = sneakHead -> value.col;

    if(sneakHead -> value.row + 1 > boardRows) {
        sneakHead -> value.row = 0;
    } else {
        sneakHead -> value.row = sneakHead -> value.row + SNEAK_STEP;
    }
    int tmpPrevRow;
    int tmpPrevCol;

    while(sneakItem != nullptr) {
        tmpPrevRow = sneakItem -> value.row;
        tmpPrevCol = sneakItem -> value.col;

        sneakItem -> value.row = prevRow;
        sneakItem -> value.col = prevCol;

        sneakItem = sneakItem -> prev;

        prevRow = tmpPrevRow;
        prevCol = tmpPrevCol;
    }
}

void sneakGoUp() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();
    DoubleLinkedListItem* sneakItem = sneakHead -> prev;

    int prevRow = sneakHead -> value.row;
    int prevCol = sneakHead -> value.col;

    if(sneakHead -> value.row - 1 < 0) {
        sneakHead -> value.row = boardRows;
    } else {
        sneakHead -> value.row = sneakHead -> value.row - SNEAK_STEP;
    }
    int tmpPrevRow;
    int tmpPrevCol;

    while(sneakItem != nullptr) {
        tmpPrevRow = sneakItem -> value.row;
        tmpPrevCol = sneakItem -> value.col;

        sneakItem -> value.row = prevRow;
        sneakItem -> value.col = prevCol;

        sneakItem = sneakItem -> prev;

        prevRow = tmpPrevRow;
        prevCol = tmpPrevCol;
    }
}

void sneakGoRight() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();
    DoubleLinkedListItem* sneakItem = sneakHead -> prev;

    int prevRow = sneakHead -> value.row;
    int prevCol = sneakHead -> value.col;

    if(sneakHead -> value.col + 1 > boardCols) {
        sneakHead -> value.col = 0;
    } else {
        sneakHead -> value.col = sneakHead -> value.col + SNEAK_STEP;
    }
    int tmpPrevRow;
    int tmpPrevCol;

    while(sneakItem != nullptr) {
        tmpPrevRow = sneakItem -> value.row;
        tmpPrevCol = sneakItem -> value.col;

        sneakItem -> value.row = prevRow;
        sneakItem -> value.col = prevCol;

        sneakItem = sneakItem -> prev;

        prevRow = tmpPrevRow;
        prevCol = tmpPrevCol;
    }
}

void readKeywordFromConsole() {
    char keyword;

    while(read(STDIN_FILENO, &keyword, 1) == 1) {
        std::setlocale(LC_ALL, "en_US.iso88591");
        char tmp = std::tolower((keyword));

        if(tmp == 'a' || tmp == 'd' || tmp == 's' || tmp == 'w') {
            pressedKeyInLowerCase = tmp;
        } else if(tmp == 'p') {
            isPaused = !isPaused;

            if(isPaused) {
                std::cout << std::endl;
                std::cout << "Paused" << std::endl;
            } else {
                system("clear");
            }
        }
    }
}

void sneakReachTarget() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();

    if(sneakHead->value.col == target.col && sneakHead->value.row == target.row) {
        score += 1;

        struct position newItemPos;
        DoubleLinkedListItem* sneakLastItem = sneak.firstElement;

        if(pressedKeyInLowerCase == 'a') {
            newItemPos.col = sneakLastItem -> value.col + 1;
            newItemPos.row = sneakLastItem -> value.row;
        }
        if(pressedKeyInLowerCase == 'd') {
            newItemPos.col = sneakLastItem -> value.col - 1;
            newItemPos.row = sneakLastItem -> value.row;
        }
        if(pressedKeyInLowerCase == 's') {
            newItemPos.col = sneakLastItem -> value.col;
            newItemPos.row = sneakLastItem -> value.row - 1;
        }
        if(pressedKeyInLowerCase == 'w') {
            newItemPos.col = sneakLastItem -> value.col;
            newItemPos.row = sneakLastItem -> value.row + 1;
        }
        struct position newTargetPos;

        newTargetPos.col = rand() % 30;
        newTargetPos.row = rand() % 30;

        while(sneak.existsByValue(newTargetPos)) {
            newTargetPos.col = rand() % 30;
            newTargetPos.row = rand() % 30;
        }
        target.col = newTargetPos.col;
        target.row = newTargetPos.row;

        sneak.unshift(newItemPos);
    }
}

void loseGame() {
    DoubleLinkedListItem* sneakHead = sneak.getLastItem();

    for(DoubleLinkedListItem* item = sneakHead -> prev; item != nullptr; item = item -> prev) {
        if(item -> value.col == sneakHead -> value.col && item -> value.row == sneakHead -> value.row) {
            std::cout << std::endl;
            std::cout << "You lose !" << std::endl;

            exit(0);
        }
    }
}

void changeSneakDirectionSideEffects() {
    sneakReachTarget();
    system("clear");
    drawBoard();
    drawScoreCount();
    usleep(SLEEP_MICROSECONDS_ANIMATION_TIME);
    winGame();
    loseGame();
}

void winGame() {
    int sneakLength = sneak.count();

    if(sneakLength >= boardCols * boardRows) {
        std::cout << "You win !" << std::endl;

        exit(0);
    }
}
