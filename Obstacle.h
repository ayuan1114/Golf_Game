//
// Created by Aaron on 5/15/2023.
//
#include "Ball.h"
#include "GameObj.h"

#ifndef GOLF_GAME_OBSTACLE_H
#define GOLF_GAME_OBSTACLE_H

class Obstacle: public GameObj {
public:
    string type;
    virtual bool interact(Ball* ball) = 0;
};

#endif //GOLF_GAME_OBSTACLE_H
