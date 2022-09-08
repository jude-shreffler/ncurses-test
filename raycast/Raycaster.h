#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <ncurses.h>
#include <locale.h>
#include <chrono>
#include <math.h>
#include <string>

#define NUMCOL 160
#define NUMROW 48
#define SPD 3
#define ASPD 2

#define WALL_COLOR 4
#define DOOR_COLOR 3

class Raycaster {
    public:
    Raycaster();

    private:
    void run();
    void redraw();
    bool input();

    int getMap(int x, int y);
    void setMap(int x, int y, int a);
    
    static double degToRad(double i);
    static double distance(double x1, double x2, double y1, double y2);
    static double normalize(double angle);
    static int roundDownToEven(int toRound);

    const double FPS = 10;
    const double SPEED = SPD / FPS * (FPS / 60);
    const double ANG_SPEED = Raycaster::degToRad(ASPD); 
    const double FOV = 80;
    const int NUM_COLUMNS = NUMCOL;
    const int NUM_ROWS = NUMROW;
    const int MAP_WIDTH = 16;
    const int MAP_HEIGHT = 16;

    WINDOW* window;
    int heights[NUMCOL];

    double posX = 1.5;
    double posY = 1.5;
    double rot = M_PI_2;
    int map[16][16] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

};

#endif