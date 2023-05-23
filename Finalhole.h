//
// Created by Aaron on 12/18/2021.
//
#include "GameBase.h"
#include "Obstacle.h"
using namespace std;

#ifndef GOLF_GAME_FINALHOLE_H
#define GOLF_GAME_FINALHOLE_H

/**
 * Finalhole object is the final hole on each level
 * Finalhole is the hole that the player are trying to reach to complete the level
 */

class Finalhole : public Obstacle {
public:
    int x, y, r;
    Sint16 flagX[3];
    Sint16 flagY[3];
    Finalhole() {
        Finalhole(0, 0, 0);
        // dont delete, need to create instance without immediate definition
    }
    Finalhole(int x, int y, int r) {
        type = "finalhole";
        this->x = x;
        this->y = y;
        this->r = r;
        flagX[0] = static_cast<Sint16>(x);
        flagX[1] = static_cast<Sint16>(x) + 20;
        flagX[2] = static_cast<Sint16>(x);
        flagY[0] = static_cast<Sint16>(y - 50);
        flagY[1] = static_cast<Sint16>(y - 40);
        flagY[2] = static_cast<Sint16>(y - 30);
    }

public:
    /**
     * draws the finahole on the screen (flag and hole)
     * @param renderer
     */
    void draw(SDL_Renderer *renderer) {
        filledCircleRGBA(renderer, x, y, r, 0, 0, 0, 255);
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
        SDL_RenderDrawLine(renderer, x, y, x, y - 50);
        filledPolygonRGBA(renderer, flagX, flagY, 3, 255, 0, 0, 255);
        aapolygonRGBA(renderer, flagX, flagY, 3, 0, 0, 0, 255);
    }

    bool interact(Ball* ball) {
        if (r > sqrt((ball->x - x) * (ball->x - x) + (ball->y - y) * (ball->y - y))) {
            ball->show = 0;
            ball->velX = 0;
            ball->velY = 0;
            ball->accelX = 0;
            ball->accelY = 0;
            ball->stopped = true;
            return true;
        }
        return false;
    }

};


#endif //GOLF_GAME_FINALHOLE_H
