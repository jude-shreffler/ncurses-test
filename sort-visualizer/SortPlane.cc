#include "SortPlane.h"
#include <cstdlib>
#include <time.h>

SortPlane::SortPlane() {
    window = newwin(24, 24, 0, 0);
    random();
    redraw();
}

void SortPlane::redraw() {
    // shake the window like an etch a sketch
    for (int i = 0; i < 24; i++) { 
        for (int j = 0; j < 24; j++) {
            mvwaddch(window, j, i, ' ');
        }
    }

    for (int i = 0; i < 24; i++) {
        for (int j = 23; j > 23 - data[i]; j--) {
            mvwaddch(window, j, i, '@');
        }
    }

    wrefresh(window);
}

void SortPlane::random() {
    srand(time(NULL));
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

    redraw();
}