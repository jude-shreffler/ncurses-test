#include "Raycaster.h"

Raycaster::Raycaster() {
    window = newwin(24, 100, 0, 0);
    wrefresh(window);

    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    //cd redraw();
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
    double rayX, rayY, offsetX, offsetY;
    int depthOfField, mapX, mapY;

    // get the heights of our bars
    for (int i = 0; i < 80; i++) {
        double aTan = -1/tan(rot + currentOffset);
        double nTan = -tan(rot + currentOffset);

        depthOfField = 0;
        /// horizontal lines
        if (rot < M_PI) { /// looking up
            rayY = int(posY + 1);
            rayX = (rayY - posY) * -aTan + posX;
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
            mapX = int(rayX);
            mapY = int(rayY);
            if (getMap(mapX, mapY) == 1) {
                depthOfField = 8;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        double distanceToH = distance(posX, rayX, posY, rayY);

        /// vertical lines
        depthOfField = 0;
        if (rot < M_PI / 2 || rot > 3 * M_PI / 2) { /// looking right
            rayX = int(posX + 1);
            rayY = (rayX - posX) * -nTan + posY;
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
            mapX = int(rayX);
            mapY = int(rayY);
            if (getMap(mapX, mapY) == 1) {
                depthOfField = 8;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        double distanceToV = distance(posX, rayX, posY, rayY);

        double distance;
        if (distanceToV < distanceToH) {
            distance = distanceToV;
        } else {
            distance = distanceToH;
        }
        distance = distance * cos(currentOffset);
        heights[i] = int(24 / distance + 0.5);

        currentOffset += degToRad(1);
    }

    // update the screen
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            mvwaddch(window, j, i, ' ');
        }
    }

    for (int i = 0; i < 80; i++) {
        int startY = 13 - (heights[i] / 2);
        for (int j = startY; j < heights[i]; j++) {
            mvwaddch(window, j, i, '@');
        }
    }

    mvwprintw(window, 0, 80, std::to_string(posX).c_str());
    mvwprintw(window, 1, 80, std::to_string(posY).c_str());
    mvwprintw(window, 2, 80, std::to_string(rot).c_str());

    wrefresh(window);
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
    } else if (a == KEY_RIGHT) {
        rot += ANG_SPEED;
        if (rot >= 2 * M_PI) {
            rot -= 2 * M_PI;
        }
    } else if (a == KEY_LEFT) {
        rot -= ANG_SPEED;
        if (rot < 0) {
            rot += 2 * M_PI;
        }
    } else if (a == KEY_DC) {
        return false;
    }

    return true;
}

int Raycaster::getMap(int x, int y) {
    if (x > 8) {
        x = 8;
    }
    if (x < 0) {
        x = 0;
    }
    if (y > 8) {
        y = 8;
    }
    if (y < 0) {
        y = 0;
    }
    return map[x][7 - y];
}

double Raycaster::degToRad(int i) {
    return i * (2 * M_PI / 180);
}

double Raycaster::distance(double x1, double x2, double y1, double y2) {
    if (fabs(y2 - y1) < 0.001 && fabs(x2 - x1) < 0.001) {
        return 100000000000;
    }
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}