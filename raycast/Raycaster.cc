#include "Raycaster.h"

Raycaster::Raycaster() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)
    timeout(500); // getch will timeout after x ms
    start_color(); // allows color
    intrflush(stdscr, FALSE);
    curs_set(0); // makes cursor invisible

    window = newwin(NUM_ROWS, NUM_COLUMNS + 20, 0, 0);
    wrefresh(window);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(DOOR_COLOR, COLOR_YELLOW, COLOR_RED);
    init_pair(WALL_COLOR, COLOR_CYAN, COLOR_WHITE);

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
            lastRun = std::chrono::steady_clock::now();
        }
    }

    endwin();
}

void Raycaster::redraw() {
    // draw background
    for (int i = 0; i < NUM_COLUMNS; i++) {
        for (int j = 0; j < NUM_ROWS; j++) {
            if (j == 0) {
                wattron(window, COLOR_PAIR(1));
            } else if (j == NUM_ROWS / 2) {
                wattroff(window, COLOR_PAIR(1));
                wattron(window, COLOR_PAIR(2));
            }
            mvwaddch(window, j, i, ' ');
            if (j == NUM_ROWS - 1) {
                wattroff(window, COLOR_PAIR(2));
            }
        }
    }

    double currentOffset = degToRad(FOV/2);
    double currentAngle = normalize(rot + currentOffset);
    double rayX, rayY, offsetX, offsetY;
    int depthOfField, mapX, mapY;
    int mapTileH, mapTileV, mapTile;

    // get the heights of our bars
    for (int i = 0; i < NUM_COLUMNS; i++) {
        double aTan = -1/tan(currentAngle);
        double nTan = -tan(currentAngle);

        depthOfField = 0;
        /// horizontal lines
        if (currentAngle < M_PI) { /// looking up
            rayY = int(posY + 1);
            rayX = (rayY - posY) * -aTan + posX;
            offsetY = 1;
            offsetX = -offsetY * aTan;
        } else if (currentAngle > M_PI) { /// looking down
            rayY = int(posY);
            rayX = (posY - rayY) * aTan + posX;
            offsetY = -1;
            offsetX = -offsetY * aTan;
        } else if (fabs(currentAngle - 0) < 0.001 || fabs(currentAngle - M_PI) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = MAP_HEIGHT;
        }

        while (depthOfField < MAP_HEIGHT) {
            mapX = int(rayX);
            mapY = int(rayY + (currentAngle < M_PI ? 0.1 : -0.1));
            if (getMap(mapX, mapY) != 0) {
                depthOfField = MAP_HEIGHT;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        mapTileH = getMap(mapX, mapY);

        double distanceToH = distance(posX, rayX, posY, rayY);

        /// vertical lines
        depthOfField = 0;
        if (currentAngle < M_PI / 2 || currentAngle > 3 * M_PI / 2) { /// looking right
            rayX = int(posX + 1);
            rayY = (rayX - posX) * -nTan + posY;
            offsetX = 1;
            offsetY = -offsetX * nTan;
        } else if (currentAngle > M_PI / 2 && currentAngle < 3 * M_PI / 2) { /// looking left
            rayX = int(posX);
            rayY = (posX - rayX) * nTan + posY;
            offsetX = -1;
            offsetY = -offsetX * nTan;
        } else if (fabs(currentAngle - M_PI / 2) < 0.001 || fabs(currentAngle - 3 * M_PI / 2) < 0.001) {
            rayX = posX;
            rayY = posY;
            depthOfField = MAP_HEIGHT;
        }

        while (depthOfField < MAP_HEIGHT) {
            mapX = int(rayX + (currentAngle < M_PI / 2 || currentAngle > 3 * M_PI / 2 ? 0.1 : -0.1));
            mapY = int(rayY);
            if (getMap(mapX, mapY) != 0) {
                depthOfField = MAP_HEIGHT;
            } else {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField++;
            }
        }

        int mapTileV = getMap(mapX, mapY);

        double distanceToV = distance(posX, rayX, posY, rayY);
        
        // pick the shortest distance and normalize it
        double distance;
        int mapTile;
        char toPrint;
        if (distanceToV < distanceToH) {
            distance = distanceToV;
            mapTile = mapTileV;
            toPrint = '@';
        } else {
            distance = distanceToH;
            mapTile = mapTileH;
            toPrint = ';';
        }
        distance = distance * cos(currentOffset);
        heights[i] = int(NUM_ROWS / distance + 0.5);
        heights[i] = roundDownToEven(heights[i]); // rounds numbers down to the next even number

        // draw the column onto the screen
        if (mapTile == 2) {
            wattron(window, COLOR_PAIR(DOOR_COLOR));
        } else if (mapTile == 1) {
            wattron(window, COLOR_PAIR(WALL_COLOR));
        }

        int startY = (NUM_ROWS / 2) - (heights[i] / 2);
        for (int j = startY; j < heights[i] + startY; j++) {
            mvwaddch(window, j, i, toPrint);
        }

        if (mapTile == 2) {
            wattroff(window, COLOR_PAIR(DOOR_COLOR));
        } else if (mapTile == 1) {
            wattroff(window, COLOR_PAIR(WALL_COLOR));
        }

        // update your offset/angle
        currentOffset -= degToRad(FOV/NUM_COLUMNS);
        currentAngle = normalize(rot + currentOffset);
    }
    
    // debug stuff
    mvwprintw(window, 0, NUM_COLUMNS, std::to_string(posX).c_str());
    mvwprintw(window, 1, NUM_COLUMNS, std::to_string(posY).c_str());
    mvwprintw(window, 2, NUM_COLUMNS, std::to_string(rot).c_str());

    // the map
    int cornerY = 3;
    int cornerX = NUM_COLUMNS;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            mvwaddch(window, cornerY + MAP_HEIGHT - 1 - j, i + cornerX, getMap(i, j) + '0');
        }
    }

    wrefresh(window);
}

bool Raycaster::input() {
    flushinp();
    int a = getch();

    if (a == 'w') {
        double tempX = posX + SPEED * cos(rot);
        double tempY = posY + SPEED * sin(rot);

        // check up and down
        if (getMap(int(posX), int(posY + 1)) != 0) { // up
            if (tempY > int(posY) + 0.8) {
                posY = int(posY) + 0.8;
            } else {
                posY = tempY;
            }
        } else if (getMap(int(posX), int(posY - 1)) != 0) { // down
            if (tempY < int(posY) + 0.2) {
                posY = int(posY) + 0.2;
            } else {
                posY = tempY;
            }
        }

        // check left and right
        if (getMap(int(posX - 1), int(posY)) != 0) { // left
            if (tempX < int(posX) + 0.2) {
                posX = int(posX) + 0.2;
            } else {
                posX = tempX;
            }
        } else if (getMap(int(posX + 1), int(posY)) != 0) { // right
            if (tempX > int(posX) + 0.8) {
                posX = int(posX) + 0.8;
            } else {
                posX = tempX;
            }
        }

        // check corners
        if (getMap(int(posX - 1), int(posY - 1)) != 0) { // lower left
            if (tempX < int(posX) + 0.2 && tempY < int(posY) + 0.2) {
                posX = int(posX) + 0.2;
                posY = int(posY) + 0.2;
            } else {
                posX = tempX;
                posY = tempY;
            }
        } else if (getMap(int(posX - 1), int(posY + 1)) != 0) { // upper left
            if (tempX < int(posX) + 0.2 && tempY > int(posY) + 0.8) {
                posX = int(posX) + 0.2;
                posY = int(posY) + 0.8;
            } else {
                posX = tempX;
                posY = tempY;
            }
        } else if (getMap(int(posX + 1), int(posY - 1)) != 0) { // lower right
            if (tempX > int(posX) + 0.8 && tempY < int(posY) + 0.2) {
                posX = int(posX) + 0.8;
                posY = int(posY) + 0.2;
            } else {
                posX = tempX;
                posY = tempY;
            }
        } else if (getMap(int(posX + 1), int(posY + 1)) != 0) { // upper right
            if (tempX > int(posX) + 0.8 && tempY > int(posY) + 0.8) {
                posX = int(posX) + 0.8;
                posY = int(posY) + 0.8;
            } else {
                posX = tempX;
                posY = tempY;
            }
        }

    } else if (a == 's') {
        double tempX = posX - SPEED * cos(rot);
        double tempY = posY - SPEED * sin(rot);
        if (getMap(int(tempX), int(tempY)) == 0) {
            posX = tempX;
            posY = tempY;
        }
    } else if (a == 'a') {
        rot += ANG_SPEED;
        if (rot >= 2 * M_PI) {
            rot -= 2 * M_PI;
        }
    } else if (a == 'd') {
        rot -= ANG_SPEED;
        if (rot < 0) {
            rot += 2 * M_PI;
        }
    } else if  (a == 'e') {
        for (int i = 0; i < MAP_WIDTH; i++) {
            for (int j = 0; j < MAP_HEIGHT; j++) {
                if (getMap(i, j) == 2) {
                    if (distance(posX, i + 0.5, posY, j + 0.5) <= 1.5) {
                        setMap(i, j, 0);
                    }
                }
            }
        }
    } else if (a == KEY_DC) {
        return false;
    }

    return true;
}

int Raycaster::getMap(int x, int y) {
    if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
        return map[MAP_HEIGHT - 1 - y][x];
    }
    
    return -1;
}

void Raycaster::setMap(int x, int y, int a) {
    if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
        map[MAP_HEIGHT - 1 - y][x] = a;
    }
}

double Raycaster::degToRad(double i) {
    return i * (M_PI / 180);
}

double Raycaster::distance(double x1, double x2, double y1, double y2) {
    if (fabs(y2 - y1) < 0.001 && fabs(x2 - x1) < 0.001) {
        return 100000000000;
    }
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

double Raycaster::normalize(double angle) {
    if (angle > 2 * M_PI) {
        return normalize(angle - 2 * M_PI);
    } else if (angle < 0) {
        return normalize(angle + 2 * M_PI);
    } else {
        return angle;
    }
}

int Raycaster::roundDownToEven(int toRound) {
    return (toRound / 2) * 2;
}