#ifndef SORTPLANE_H
#define SORTPLANE_H

#include <ncurses.h>

class SortPlane {
    public:
        SortPlane();
        void redraw();
        void random();

    private:
    WINDOW* window;
    int data[24];
};

#endif