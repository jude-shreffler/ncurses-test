#include "object.h"
#include <fstream>

Object::Object(int newX, int newY, int newLayer) {
    posX = newX;
    posY = newY;
    layer = newLayer;
}

void Object::redraw() {
    redrawwin(window);
    wrefresh(window);
}

void Object::move(int newX, int newY) {
    posX = newX;
    posY = newY;
}



Player::Player(int newX, int newY, int newLayer) {
    posX = newX;
    posY = newY;
    layer = newLayer;
    window = newwin(height, width, newY, newX);
}

void Player::redraw() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mvwaddch(window, j, i, sprite[i][j]);
        }
    }
    Object::redraw();
}

void Player::move(int newX, int newY) {
    Object::move(newX, newY);
    mvwprintw(window, 0, 0, "   ");
    mvwprintw(window, 1, 0, "   ");
    mvwprintw(window, 2, 0, "   ");
    window = newwin(height, width, newY, newX);
}



Background::Background() {
    Object(0, 0, 15);
    window = newwin(height, width, 0, 0);
}

void Background::redraw() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mvwaddch(window, j, i, sprite[i][j]);
        }
    }
    Object::redraw();
}

void Background::load(std::string fileName) {
    std::ifstream ins;
    ins.open(fileName);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            sprite[i][j] = ins.get();
        }
        ins.get();
    }

    ins.close();
}

void redrawScreen(std::vector<Object*> objectList) {
    for (int i = 15; i >= 0; i--) {
        for (size_t j = 0; j < objectList.size(); j++) {
            if (objectList[j]->getLayer() == i) {
                objectList[j]->redraw();
            }
        }
    }
}