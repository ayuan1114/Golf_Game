//
// Created by Aaron on 5/15/2023.
//
#include "Ball.h"
#include "GameObj.h"

#ifndef GOLF_GAME_OBSTACLE_H
#define GOLF_GAME_OBSTACLE_H

/**
 * interface for all obstacles in a level that the ball can interact with
 */
class Obstacle: public GameObj {
public:
    string type;
    virtual bool interact(Ball* ball) = 0;
};

#endif //GOLF_GAME_OBSTACLE_H
