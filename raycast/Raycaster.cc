#include "Raycaster.h"

Raycaster::Raycaster() {
    setlocale(LC_ALL, ""); //sets the locale so the characters print right

    initscr(); // initiailizes the screen
    cbreak(); // makes the input one character at a time
    noecho(); // no echo
    keypad(stdscr, TRUE); // enables the keypad (arrow keys, f keys, etc)
    timeout(5); // getch will timeout after x ms
    start_color(); // allows color
    intrflush(stdscr, FALSE);
    curs_set(0); // makes cursor invisible

    window = newwin(NUM_ROWS, NUM_COLUMNS + 20, 0, 0);
    wrefresh(window);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);

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
            depthOfField = 8;
        }

        while (depthOfField < 8) {
            mapX = int(rayX);
            mapY = int(rayY + (currentAngle < M_PI ? 0.1 : -0.1));
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
            depthOfField = 8;
        }

        while (depthOfField < 8) {
            mapX = int(rayX + (currentAngle < M_PI / 2 || currentAngle > 3 * M_PI / 2 ? 0.1 : -0.1));
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
        
        // pick the shortest distance and normalize it
        double distance;
        char toPrint;
        if (distanceToV < distanceToH) {
            distance = distanceToV;
            toPrint = '@';
        } else {
            distance = distanceToH;
            toPrint = '.';
        }
        distance = distance * cos(currentOffset);
        heights[i] = int(NUM_ROWS / distance + 0.5);
        heights[i] = roundDownToEven(heights[i]); // rounds numbers down to the next even number

        // draw the column onto the screen
        int startY = (NUM_ROWS / 2) - (heights[i] / 2);
        for (int j = startY; j < heights[i] + startY; j++) {
            mvwaddch(window, j, i, toPrint);
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
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mvwaddch(window, cornerY + 7 - j, i + cornerX, getMap(i, j) + '0');
        }
    }

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
    } else if (a == KEY_LEFT) {
        rot += ANG_SPEED;
        if (rot >= 2 * M_PI) {
            rot -= 2 * M_PI;
        }
    } else if (a == KEY_RIGHT) {
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
    if (x >= 0 && y >= 0 && x < 8 && y < 8) {
        return map[7 - y][x];
    }
    
    return -1;
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