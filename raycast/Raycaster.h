#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <ncurses.h>
#include <chrono>
#include <math.h>

class Raycaster {
    public:
    Raycaster();

    private:
    void run();
    void redraw();
    bool input();

    int getMap(int x, int y);
    double degToRad(int i);

    WINDOW* window;
    int heights[80];

    const double FPS = 60;
    const double SPEED = 2 / FPS;
    const double ANG_SPEED = 0.0174;
    double posX = 4;
    double posY = 2;
    double rot = 0;
    int map[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                     {1, 0, 0, 0, 0, 0, 1, 1},
                     {1, 0, 1, 0, 0, 0, 0, 1},
                     {1, 0, 0, 1, 1, 0, 0, 1},
                     {1, 0, 0, 1, 1, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1, 0, 1},
                     {1, 1, 0, 0, 0, 0, 0, 1},
                     {1, 1, 1, 1, 1, 1, 1, 1}};

};

#endif