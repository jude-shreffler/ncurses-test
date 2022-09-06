#ifndef SORTPLANE_H
#define SORTPLANE_H

#include <ncurses.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

class SortPlane {
    public:
    SortPlane();

    private:
    void run();
    void redraw();
    void random();

    bool input();

    void swap(int i, int j);

    void bubble();
    void selection();
    void bogo();

    WINDOW* window;
    int data[24];

    WINDOW* ui;
    const std::vector<std::string> sortList = {"Bubble", "Selection", "Bogosort"};
    int sortOption = 0;
    bool enterOption = false;

    WINDOW* wdebug;
    bool debug = true;
};

#endif