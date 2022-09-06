#include <ncurses.h>
#include <locale.h>
#include <string>
#include <chrono>

using namespace std;

WINDOW* createSprite(int startY, int startX);
void deleteSprite(WINDOW* localWin);

int colors[] = {COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE};
int delay = 1;

int main() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)
    timeout(delay == 1 ? 1 : delay * 9 / 10); // getch will timeout after x ms
    start_color(); // allows color
    intrflush(stdscr, FALSE);
    curs_set(0); // makes cursor invisible

    int a = 0;  // this initializes all of our color pairs, 0-63
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            init_pair(a, colors[i], colors[j]);
            a++;
        } 
    }

    srand(time(NULL)); // rand setup

    chrono::time_point<chrono::steady_clock> lastRun = chrono::steady_clock::now(); // sets up our last run counter

    int spriteY = 0; // sets up our sprite and it's position
    int spriteX = 0;
    WINDOW* sprite = createSprite(spriteY, spriteX);
    bool toBreak = false;

    // setup for the array that stores the screen data
    char background[80][24]; //x, y
    int colors[80][24]; //x, y
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            background[i][j] = ' ';
            colors[80][24] = 0;
        }
    }

    while (true) {
        chrono::time_point<chrono::steady_clock> now = chrono::steady_clock::now(); // checks current time to compare to lastRun

        if (now - lastRun >= std::chrono::milliseconds(delay)) { // if the interval desired has been reached, change a cell and redraw the player
            int x = rand() % 80;
            int y = rand() % 24;
            int p = rand() % 64;

            background[x][y] = '@';
            colors[x][y] = p;

            for (int i = 0; i < 80; i++) {
                for (int j = 0; j < 24; j++) {
                    attron(COLOR_PAIR(colors[i][j]));
                    mvaddch(j, i, background[i][j]);
                    attroff(COLOR_PAIR(colors[i][j]));
                }
            }

            refresh();

            lastRun = chrono::steady_clock::now();
        }
        redrawwin(sprite);
        wrefresh(sprite);

        unsigned int dec = getch();
        switch (dec) {
        case KEY_UP:
            deleteSprite(sprite);
            spriteY--;
            sprite = createSprite(spriteY, spriteX);
            break;
        case KEY_DOWN:
            deleteSprite(sprite);
            spriteY++;
            sprite = createSprite(spriteY, spriteX);
            break;
        case KEY_LEFT:
            deleteSprite(sprite);
            spriteX--;
            sprite = createSprite(spriteY, spriteX);
            break;
        case KEY_RIGHT:
            deleteSprite(sprite);
            spriteX++;
            sprite = createSprite(spriteY, spriteX);
            break;
        case KEY_HOME:
            toBreak = true;
            break;
        default:
            break;
        }

        if (toBreak == true) {
            break;
        }
    }

    endwin();
    return 0;
}

WINDOW* createSprite(int startY, int startX) {
    WINDOW* localWin;

    localWin = newwin(3, 3, startY, startX);
    wprintw(localWin, " o ");
    wprintw(localWin, "-|-");
    wprintw(localWin, "/ \\");
    wrefresh(localWin);

    return localWin;
}

void deleteSprite(WINDOW* localWin) {
    mvwprintw(localWin, 0, 0, "   ");
    mvwprintw(localWin, 1, 0, "   ");
    mvwprintw(localWin, 2, 0, "   ");
    wrefresh(localWin);
}