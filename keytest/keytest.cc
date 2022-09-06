#include <ncurses.h>
#include <locale.h>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)

    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    while (true) {
        bool toBreak = false;
        unsigned int a = getch();

        switch (a) {
        case KEY_DOWN:
            printw("DOWN ");
            break;
        case KEY_UP:
            printw("UP ");
            break;
        case KEY_LEFT:
            printw("LEFT ");
            break;
        case KEY_RIGHT:
            printw("RIGHT ");
            break;
        case KEY_HOME:
            toBreak = true;
            break;
        default:
            break;
        }

        if (toBreak) {
            break;
        }

    }

    endwin();
    return 0;
}
