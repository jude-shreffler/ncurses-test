/**
 * Brief: hopefully will be a screen w/ background that you can place objects onto
 * 
 * 
 * 
 */

#include <ncurses.h>
#include <locale.h>
#include <string>
#include <chrono>
#include <vector>
#include "object.h"

void redrawScreen(std::vector<Object*> objectList);

int delay = 250;

int main() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)
    timeout(delay * 9 / 10); // getch will timeout after x ms
    start_color(); // allows color
    intrflush(stdscr, FALSE);
    curs_set(0); // makes cursor invisible

    /*
    printw("hello");
    */    
    refresh();

    std::vector<Object*> objectList;

    Background* newBck = new Background();
    newBck->load("background.sp");
    objectList.push_back(newBck);
    
    Player* newPly = new Player(3, 3, 0);
    objectList.push_back(newPly);

    std::chrono::time_point<std::chrono::steady_clock> lastRun = std::chrono::steady_clock::now(); // sets up our last run counter

    redrawScreen(objectList);

    while (true) {
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now(); // checks current time to compare to lastRun
    
        if (now - lastRun >= std::chrono::milliseconds(delay)) {
            int decision = getch();
            
            if (decision == KEY_UP) {
                newPly->move(newPly->getPosX(), newPly->getPosY() - 1);
                redrawScreen(objectList);
            } else if (decision == KEY_DOWN) {
                newPly->move(newPly->getPosX(), newPly->getPosY() + 1);
                redrawScreen(objectList);
            } else if (decision == KEY_LEFT) {
                newPly->move(newPly->getPosX() - 1, newPly->getPosY());
                redrawScreen(objectList);
            } else if (decision == KEY_RIGHT) {
                newPly->move(newPly->getPosX() + 1, newPly->getPosY());
                redrawScreen(objectList);
            } else if (decision == KEY_HOME) {
                break;
            }
        }
    }

    endwin();
    return 0;
}
