#include "GameBase.h"
#include "Obstacle.h"
#include <cmath>
using namespace std;

#ifndef GOLF_GAME_BOOSTPAD_H
#define GOLF_GAME_BOOSTPAD_H


class Boostpad : public Obstacle{
public:
    SDL_Rect rect;
    double accel;
    double angle;
    Sint16 triX[3];
    Sint16 triY[3];
    bool activated = false;
    int rInner = 255, gInner = 255, bInner = 255;
    int rOuter = 30, gOuter = 100, bOuter = 140;
    int change;
    Boostpad(int x, int y, int w, int h, double accel, double angle) { // positive x-axis = 0, clockwise
        type = "boostpad";
        this->rect.x = x;
        this->rect.y = y;
        this->rect.w = w;
        this->rect.h = h;
        this->accel = accel;
        this->angle = (angle / 180) * M_PI;
        createPosV();
    }

    void createPosV() {
        int size = min(rect.w, rect.h) / 15;
        triX[0] = rect.x + (rect.w / 2) - (double) (2 * size * cos(angle) + 5 * size * sin(angle));
        triY[0] = rect.y + (rect.h / 2) - (double) (2 * size * sin(angle) - 5 * size * cos(angle));
        triX[1] = rect.x + (rect.w / 2) + (double) (3 * size * cos(angle));
        triY[1] = rect.y + (rect.h / 2) + (double) (3 * size * sin(angle));
        triX[2] = rect.x + (rect.w / 2) - (double) (2 * size * cos(angle) - 5 * size * sin(angle));
        triY[2] = rect.y + (rect.h / 2) - (double) (2 * size * sin(angle) + 5 * size * cos(angle));
    }

    void updateEffects() {
        if (activated) {
            if (rInner <= 0) {
                rInner = 0;
                change = 5;
            }
            else if (rInner >= 200) {
                rInner = 200;
                change = -5;
            }
            rInner += change;
        }
        else {
            rInner = 255;
        }
    }

public:
    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, rOuter, gOuter, bOuter, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        updateEffects();
        filledPolygonRGBA(renderer, triX, triY, 3, rInner, gInner, bInner, 255);
        aapolygonRGBA(renderer, triX, triY, 3, 0, 0, 0, 255);
    }

    bool interact(Ball* ball) {
        if (ball->x <= rect.x + rect.w and ball->x >= rect.x and ball->y <= rect.y + rect.h and ball->y >= rect.y) {
            ball->accelX += accel * cos(angle);
            ball->accelY += accel * sin(angle);
            activated = true;
            return true;
        }
        activated = false;
        return false;
    }

};


#endif //GOLF_GAME_BOOSTPAD_H
