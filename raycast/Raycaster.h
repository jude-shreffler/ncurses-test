#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <ncurses.h>
#include <locale.h>
#include <chrono>
#include <math.h>
#include <string>

class Raycaster {
    public:
    Raycaster();

    private:
    void run();
    void redraw();
    bool input();

    int getMap(int x, int y);
    double degToRad(double i);
    double distance(double x1, double x2, double y1, double y2);
    double normalize(double angle);
    int roundDownToEven(int toRound);

    WINDOW* window;
    int heights[80];

    const double FPS = 60;
    const double SPEED = 2 / FPS;
    const double ANG_SPEED = 0.0174;
    const double FOV = 80;
    const double NUM_COLUMNS = 80;
    double posX = 2;
    double posY = 2;
    double rot = 0;
    int map[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                     {1, 0, 0, 0, 0, 0, 0, 1},
                     {1, 0, 1, 1, 1, 1, 0, 1},
                     {1, 0, 0, 0, 0, 1, 0, 1},
                     {1, 0, 1, 0, 0, 1, 0, 1},
                     {1, 0, 1, 1, 1, 1, 0, 1},
                     {1, 0, 0, 0, 0, 0, 0, 1},
                     {1, 1, 1, 1, 1, 1, 1, 1}};

};

#endif