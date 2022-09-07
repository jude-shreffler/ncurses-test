#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <ncurses.h>
#include <locale.h>
#include <chrono>
#include <math.h>
#include <string>

#define NUMCOL 160
#define NUMROW 48
#define SPD 4
#define ASPD 2

class Raycaster {
    public:
    Raycaster();

    private:
    void run();
    void redraw();
    bool input();

    int getMap(int x, int y);
    
    static double degToRad(double i);
    static double distance(double x1, double x2, double y1, double y2);
    static double normalize(double angle);
    static int roundDownToEven(int toRound);

    const double FPS = 60;
    const double SPEED = SPD / FPS;
    const double ANG_SPEED = Raycaster::degToRad(ASPD); 
    const double FOV = 80;
    const int NUM_COLUMNS = NUMCOL;
    const int NUM_ROWS = NUMROW;

    WINDOW* window;
    int heights[NUMCOL];

    double posX = 1.5;
    double posY = 1.5;
    double rot = M_PI_2;
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