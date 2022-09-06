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
    double currentOffset = degToRad(-40);
    double rayX, rayY;

    for (int i = 0; i < 80; i++) {
        /// horizontal lines
        if (rot < M_PI) { /// looking up
            rayY = int(posY + 1);
            rayX = (rayY - posY) * atan(rot) + posX;
        } else if (rot > M_PI) { /// looking down
            rayY = int(posY);
        }

        /// vertical lines

        currentOffset += degToRad(1);
    }
}

bool Raycaster::input() {
    int a = getch();

    if (a == KEY_UP) {
        double tempX = posX + SPEED * cos(rot);
        double tempY = posY + SPEED * sin(rot);
        if (getMap(int(tempX), int(tempY)) == 0) {
            posX = tempX;
            posY = tempY;
        }
    } else if (a == KEY_DOWN) {
        double tempX = posX - SPEED * cos(rot);
        double tempY = posY - SPEED * sin(rot);
        if (getMap(int(tempX), int(tempY)) == 0) {
            posX = tempX;
            posY = tempY;
        }
    } else if (a == KEY_LEFT) {
        rot += ANG_SPEED;
        if (rot >= 2 * M_PI) {
            rot = 0;
        }
    } else if (a == KEY_RIGHT) {
        rot -= ANG_SPEED;
        if (rot < 0) {
            rot = 2 * M_PI;
        }
    } else if (a == KEY_DC) {
        return false;
    }

    return true;
}

int Raycaster::getMap(int x, int y) {
    return map[x][7 - y];
}

double Raycaster::degToRad(int i) {
    return i * (2 * M_PI / 180);
}