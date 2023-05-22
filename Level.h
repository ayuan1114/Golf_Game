#include "Block.h"
#include "Boostpad.h"
#include "Blackhole.h"
#include "Finalhole.h"
#include "Obstacle.h"
using namespace std;

#ifndef GOLF_GAME_LEVEL_H
#define GOLF_GAME_LEVEL_H

/**
 * level object contains a list of all the obstacles that are to be added
 */

class Level {
public:
    vector<Obstacle*> obstacles;
    vector<Block> walls;
    int startX, startY;
    Level(int startX, int startY, int finalX, int finalY) {
        this->startX = startX;
        this->startY = startY;
        obstacles.push_back(new Finalhole(finalX, finalY, 15));
    }
    void addBlock(int x, int y, int w, int h, int rad) {
        walls.push_back(Block(x, y, w, h, rad));
    }
    void addPad(int x, int y, int w, int h, double accel, double angle) {
        obstacles.push_back(new Boostpad(x, y, w, h, accel, angle));
    }
    void addHole(int x, int y, int rad, int maxR, double G) {
        obstacles.push_back(new Blackhole(x, y, rad, maxR, G));
    }
};


#endif //GOLF_GAME_LEVEL_H
