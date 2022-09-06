#include <ncurses.h>
#include <locale.h>
#include <string>
#include <fstream>

void redraw(char canvas[80][24]);
bool validChar(int ch);
void debug(std::string message, int x, int y);

const int delay = 10;

int main() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)
    timeout(delay); // getch will timeout after x ms
    start_color(); // allows color
    intrflush(stdscr, FALSE);
    refresh(); // initial refresh to make other windows work?

    // border setup
    WINDOW* window = newwin(25, 81, 0, 0);
    wborder(window, ' ', '|', ' ', '-', ' ', '|', '-', '@');
    wrefresh(window);

    char canvas[80][24];
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            canvas[i][j] = ' ';
        }
    }
    int cursorX = 0;
    int cursorY = 0;

    std::ifstream ins;
    std::ofstream outs;

    while(true) {
        int input = getch();
        //if (input > -1) { move(0, 82); printw((std::to_string(input) + "    ").c_str()); move(cursorY, cursorX); wrefresh(window); } // debug

        if (input == -1) {
            continue;
        }

        if (input < 128 && input > 31) { //denotes that it's a basic char
            canvas[cursorX][cursorY] = input;
            redraw(canvas);
            move(cursorY, cursorX);
        } else if (input == KEY_UP && cursorY != 0) {
            cursorY--;
            move(cursorY, cursorX);
        } else if (input == KEY_DOWN && cursorY != 23) {
            cursorY++;
            move(cursorY, cursorX);
        } else if (input == KEY_LEFT && cursorX != 0) {
            cursorX--;
            move(cursorY, cursorX);
        } else if (input == KEY_RIGHT && cursorX != 79) {
            cursorX++;
            move(cursorY, cursorX);
        } else if (input == KEY_HOME) {
            break;
        } else if (input == 15) { // open a file
            //get the file name
            move(25, 0);
            printw("What file would you like to open? ");
            std::string fileName = "";
            timeout(-1);
            echo(); 
            while (true) {
                int ch = getch();
                if (ch == '\n') {
                    break;
                } else {
                    fileName += ch;
                }
            }
            timeout(delay);
            noecho();
            mvprintw(25,0, "                                                                           ");
            move(cursorY, cursorX);

            // open a file
            ins.close();
            ins.open(fileName);
            
            //check if its empty
            int check = ins.peek();
            if (check == EOF) { // then it's empty, initialize array to all spaces
                debug(fileName + " was empty", cursorX, cursorY);
                for (int i = 0; i < 80; i++) {
                    for (int j = 0; j < 24; j++) {
                        canvas[i][j] = ' ';
                    }
                }
            } else { // otherwise, read in the file
                debug("Reading file", cursorX, cursorY);
                for (int j = 0; j < 24; j++) {
                    for (int i = 0; i < 80; i++) {
                        int check = ins.peek();
                        if (check == '\n') { // if you've reached a new line before 80 chars, fill the rest of the row with spaces
                            for (i; i < 80; i++) {
                                canvas[i][j] = ' ';
                            }
                        } else if (check == EOF) { // if EOF is reached at all, just fill the rest with spaces
                            canvas[i][j] = ' ';
                        } else { // if it's a normal ass character put it where it goes
                            canvas[i][j] = ins.get();
                        }
                    }
                    check = ins.peek(); // get rid of them dangling newlines
                    if (check == '\n') {
                        ins.get();
                    }
                }
            }

            redraw(canvas);
            move(cursorY, cursorX);
        } else if (input == 11) {
            //get the file name
            move(25, 0);
            printw("What file would you like to save to? ");
            std::string fileName = "";
            timeout(-1);
            echo(); 
            while (true) {
                int ch = getch();
                if (ch == '\n') {
                    break;
                } else {
                    fileName += ch;
                }
            }
            
            // get the width of your sprite
            mvprintw(25, 0, "                                                                                             ");
            mvprintw(25, 0, "What is the width of your sprite? ");
            std::string width = "";
            while (true) {
                int ch = getch();
                if (ch == '\n') {
                    break;
                } else {
                    width += ch;
                }
            }

            // get the height of your sprite
            mvprintw(25, 0, "                                                                                             ");
            mvprintw(25, 0, "What is the height of your sprite? ");
            std::string height = "";
            while (true) {
                int ch = getch();
                if (ch == '\n') {
                    break;
                } else {
                    height += ch;
                }
            }

            timeout(delay);
            noecho();
            mvprintw(25,0, "                                                                           ");
            move(cursorY, cursorX);

            // open a file
            outs.close();
            outs.open(fileName);

            // print canvas to file
            for (int j = 0; j < stoi(height); j++) {
                for (int i = 0; i < stoi(width); i++) {
                    outs.put(canvas[i][j]);
                }
                outs.put('\n');
            }

            outs.close();
            debug("File written!", cursorX, cursorY);
        }
    }

    ins.close();
    endwin();
    return 0;
}

void redraw(char canvas[80][24]) {
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            mvaddch(j, i, canvas[i][j]);
        }
    }
    refresh();
}

bool validChar(int ch) {

    return false;
}


void debug(std::string message, int x, int y) {
    mvprintw(0, 82, "                                                                                                    ");
    mvprintw(0, 82, message.c_str());
    move(y, x);
}