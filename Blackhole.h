#include "GameBase.h"
#include "Obstacle.h"
using namespace std;

#ifndef GOLF_GAME_BLACKHOLE_H
#define GOLF_GAME_BLACKHOLE_H

/**
 * Blackhole object is the blackhole obstacle that appears in some levels
 * The blackhole sucks the ball towards its center
 */

class Blackhole : public Obstacle {
public:
    int x, y, rad;
    double G; // constant that defines how strong the force of the blackhole is
    double maxR;
    int effectR = maxR;
    int rInner = 160, gInner = 32, bInner = 240;
    int rOuter = 160, gOuter = 32, bOuter = 240;
    int change;
    int frameCount = 0;
    Blackhole(int x, int y, int rad, int maxR, double G) {
        type = "blackhole";
        this->x = x;
        this->y = y;
        this->rad = rad;
        this->G = G;
        this->maxR = maxR;
    }

    /**
     * updates look of blackhole to create it's animation
     */
    void updateEffects() {
        if (effectR <= rad) {
            effectR = maxR;
        } else {
            effectR--;
        }
        if (frameCount % 5 == 0) {
            if (rInner <= 0) {
                rInner = 0;
                gInner = 0;
                bInner = 0;
                change = 1;
            } else if (rInner >= 80) {
                rInner = 80;
                gInner = 16;
                bOuter = 120;
                change = -1;
            }
            rInner += change * 10;
            gInner += change * 2;
            bInner += change * 15;
        }
    }

public:
    /**
     * draws the black hole on screen (center hole and suction perimeter)
     * @param renderer renderer to draw blackhole on
     */
    void draw(SDL_Renderer *renderer) {
        aaFilledEllipseRGBA(renderer, x, y, rad, rad, rInner, gInner, bInner, 255);
        aacircleRGBA(renderer, x, y, maxR, rOuter, gOuter, bOuter, 255);
        updateEffects();
        aacircleRGBA(renderer, x, y, effectR, rOuter, gOuter, bOuter, 255);
    }

    /**
     * checks if blackhole effects the given ball and updates the ball's acceleration if it does
     * @param ball the ball to be checked
     * @return true if the ball is effected by the blackhole false otherwise
     */
    bool interact(Ball* ball) {
        double centerMag = magnitude({ball->x - x, ball->y - y});
        if (centerMag < maxR) {
            ball->accelX -= G * ((ball->x - x) / (centerMag * centerMag));
            ball->accelY -= G * ((ball->y - y) / (centerMag * centerMag));
            return true;
        }
        return false;
    }

};

#endif //GOLF_GAME_BLACKHOLE_H
