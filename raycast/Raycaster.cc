#include "Raycaster.h"

Raycaster::Raycaster() {
    window = newwin(24, 80, 0, 0);

    redraw();
    run();
}

void Raycaster::run() {
    bool toRun = true;
    std::chrono::time_point<std::chrono::steady_clock> lastRun = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> now;

    while (toRun) {
        now = std::chrono::steady_clock::now();
        if (now - lastRun >= std::chrono::milliseconds(int(1 / FPS) * 1000)) {
            toRun = input();
            redraw();
        }
    }
}

void Raycaster::redraw() {

}

bool Raycaster::input() {
    int a = getch();

    if (a == KEY_UP) {
        double tempX = posX + SPEED * cos(rot);
        double tempY = posY + SPEED * sin(rot);
        
    } else if (a == KEY_DOWN) {
        posX -= SPEED * cos(rot);
        posY -= SPEED * sin(rot);
    } else if (a == KEY_LEFT) {

    } else if (a == KEY_RIGHT) {

    } else if (a == KEY_DC) {
        return false;
    }

    return true;
}