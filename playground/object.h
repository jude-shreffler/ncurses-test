#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <ncurses.h>
#include <string>

class Object{
    public:
        Object(int newX = 0, int newY = 0, int newLayer = 0);
        virtual void redraw();
        virtual void move(int newX, int newY);

        int getLayer(){return layer;}
        int getPosX(){return posX;}
        int getPosY(){return posY;}

    protected:
        int posX;
        int posY;
        WINDOW* window;
        int layer;
};

class Player : public Object {
    public:
        Player(int newX = 0, int newY = 0, int newLayer = 0);
        void redraw();
        void move(int newX, int newY);

    private:
        const int width = 3;
        const int height = 3;
        char sprite[3][3] = {{' ', '-', '/'}, {'o', '|', ' '}, {' ', '-', '\\'}};
};

class Background : public Object {
    public:
        Background();
        void redraw();
        void load(std::string fileName);

    private:
        const int width = 80;
        const int height = 24;
        char sprite[80][24];
};

void redrawScreen(std::vector<Object*> objectList);
void debug(WINDOW* window, std::string message, int x, int y);

#endif