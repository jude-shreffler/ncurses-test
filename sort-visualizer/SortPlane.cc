#include "SortPlane.h"

SortPlane::SortPlane() {
    window = newwin(24, 24, 0, 0);
    wdebug = newwin(1, 72, 25, 0);

    ui = newwin(6, 20, 0, 24);
    wborder(ui, '|', '|', ' ', '_', '|', '|', '|', '|');
    for (int i = 1; i < 19; i++) {
        mvwaddch(ui, 2, i, '_');
    }
    for (int j = 3; j < 6; j++) {
        mvwaddch(ui, j, 12, '|');
    }
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    random();
    redraw();

    srand(time(NULL));

    run();
}

void SortPlane::run() {
    bool toRun = true;
    while (toRun) {
        toRun = input();
    }
}

void SortPlane::redraw() {
    // shake the sort window like an etch a sketch
    for (int i = 0; i < 24; i++) { 
        for (int j = 0; j < 24; j++) {
            mvwaddch(window, j, i, ' ');
        }
    }

    // put in the columns
    for (int i = 0; i < 24; i++) {
        for (int j = 23; j > 23 - data[i]; j--) {
            mvwaddch(window, j, i, '@');
        }
    }

    // do the debug window
    for (int i = 0; i < 72; i++) {
        mvwaddch(wdebug, 0, i, ' ');
    }

    if (debug == true) {
        for (int i = 0; i < 24; i++) {
            mvwprintw(wdebug, 0, 3*i, std::to_string(data[i]).c_str());
        }
        wrefresh(wdebug);
    }

    // do the words in the ui screen
    mvwprintw(ui, 1, 2, "                ");
    mvwprintw(ui, 1, 2, sortList[sortOption].c_str());
    wattron(ui, COLOR_PAIR(!enterOption));
    mvwprintw(ui, 4, 2, "Randomize");
    wattroff(ui, COLOR_PAIR(!enterOption));
    wattron(ui, COLOR_PAIR(enterOption));
    mvwprintw(ui, 4, 14, "Play");
    wattroff(ui, COLOR_PAIR(enterOption));

    wrefresh(window);
    wrefresh(ui);
}

void SortPlane::random() {
    int newArray[24];

    for (int i = 0; i < 24; i++) {
        newArray[i] = 0;
    }

    for (int i = 1; i < 25; i++) {
        while (true) {
            int check = rand() % 24;
            if (newArray[check] == 0) {
                newArray[check] = i;
                break;
            }
        }
    }

    for (int i = 0; i < 24; i++) {
        data[i] = newArray[i];
    }

    redraw();
}

bool SortPlane::input() {
    int a = getch();

    if (a == KEY_LEFT || a == KEY_RIGHT) {
        if (enterOption) {
            enterOption = false;
        } else {
            enterOption = true;
        }
        redraw();
    } else if (a == KEY_UP) {
        sortOption--;
        if (sortOption < 0) {
            sortOption = sortList.size() - 1;
        }
        redraw();
    } else if (a == KEY_DOWN) {
        sortOption++;
        if (sortOption >= sortList.size()) {
            sortOption = 0;
        }
        redraw();
    } else if (enterOption == false && a == 10) { // keyboard enter key
        random();
    } else if (enterOption == true && a == 10) { // keyboard enter key
        if (sortOption == 0) {
            bubble();
        } else if (sortOption == 1) {
            selection();
        } else if (sortOption == sortList.size() - 1) {
            bogo();
        }
    } else if (a == KEY_HOME) {
        return false;
    }

    return true;
}

void SortPlane::swap(int i, int j) {
    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;

}

void SortPlane::bubble() { // todo add time delay
    bool didSwap = true;
    while (didSwap == true) {
        didSwap = false;
        for (int i = 0; i < 23; i++) {
            if (data[i] > data[i+1]) {
                swap(i, i+1);
                redraw();
                didSwap = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            }
        }
    }
}

void SortPlane::selection() {
    for (int i = 0; i < 24; i++) {
        int min = data[i];
        int minIn = i;
        for (int j = i + 1; j < 24; j++) {
            if (data[j] < min) {
                min = data[j];
                minIn = j;
            }
        }
        swap(i, minIn);
        redraw();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SortPlane::bogo() {
    bool sorted = false;
    while (sorted == false) {
        random();
        sorted = true;
        for (int i = 0; i < 23; i++) {
            if (data[i] > data[i+1]) {
                sorted = false;
                break;
            }
        }
    }
}