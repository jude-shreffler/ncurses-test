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
    double aTan = -1/tan(rot);
    double nTan = -tan(rot);
    double rayX, rayY, offsetX, offsetY;
    int depthOfField, mapX, mapY;

    for (int i = 0; i < 80; i++) {
        depthOfField = 0;
        /// horizontal lines
        if (rot < M_PI) { /// looking up
            rayY = int(posY + 1);
            rayX = (rayY - posY) * aTan + posX;
            offsetY = 1;
            offsetX = -offsetY * aTan;
        } else if (rot > M_PI) { /// looking down
            rayY = int(posY);
            rayX = (posY - rayY) * aTan + posX;
            offsetY = -1;
            offsetX = -offsetY * aTan;
        } else if (fabs(rot - 0) < 0.001 || fabs(rot - M_PI) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = 8;
        }

        while (depthOfField < 8) {
            mapX = int(posX);
            mapY = int(posY);
            if (getMap(mapX, mapY) == 1) {
                depthOfField = 8;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        /// vertical lines
        if (rot < M_PI / 2 || rot > 3 * M_PI / 2) { /// looking right
            rayX = int(posX + 1);
            rayY = (rayX - posX) * nTan + posY;
            offsetX = 1;
            offsetY = -offsetX * nTan;
        } else if (rot > M_PI / 2 && rot < 3 * M_PI / 2) { /// looking left
            rayX = int(posX);
            rayY = (posX - rayX) * nTan + posY;
            offsetX = -1;
            offsetY = -offsetX * nTan;
        } else if (fabs(rot - M_PI / 2) < 0.001 || fabs(rot - 3 * M_PI / 2) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = 8;
        }

        while (depthOfField < 8) {
            mapX = int(posX);
            mapY = int(posY);
            if (getMap(mapX, mapY) == 1) {
                depthOfField = 8;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        

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