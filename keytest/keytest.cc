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
        int a = getch();

        if (a == KEY_HOME) {
            break;
        }

        if (a != -1) {
            mvprintw(0, 1, to_string(a).c_str());
        }

    }

    endwin();
    return 0;
}
