//
// Created by Aaron on 12/18/2021.
//
#include "GameBase.h"
#include "Obstacle.h"

#ifndef GOLF_GAME_BLOCK_H
#define GOLF_GAME_BLOCK_H

class Block : public Obstacle{
public:
    SDL_Rect rectH;
    SDL_Rect rectW;
    int x, y, w, h, rad;
    int r = 181, g = 101, b = 29;
    Block(int x, int y, int w, int h, int rad) {
        type = "block";
        this->rectW.x = x;
        this->rectW.y = y + rad;
        this->rectW.w = w;
        this->rectW.h = h - 2 * rad;
        this->rectH.x = x + rad;
        this->rectH.y = y;
        this->rectH.w = w - 2 * rad;
        this->rectH.h = h;
        this->x = x + rad;
        this->y = y + rad;
        this->w = w - 2 * rad;
        this->h = h - 2 * rad;
        this->rad = rad;
    }

public:
    void draw(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &rectH);
        SDL_RenderFillRect(renderer, &rectW);
        filledPieRGBA(renderer, x + w, y + h, rad, 0, 90, r, g, b, 255);
        filledPieRGBA(renderer, x, y + h, rad, 90, 180, r, g, b, 255);
        filledPieRGBA(renderer, x, y, rad, 180, 270, r, g, b, 255);
        filledPieRGBA(renderer, x + w, y, rad, 270, 360, r, g, b, 255);
        // outline
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x - rad, y, x - rad, y + h);
        SDL_RenderDrawLine(renderer, x, y - rad, x + w, y - rad);
        SDL_RenderDrawLine(renderer, x + w + rad, y, x + w + rad, y + h);
        SDL_RenderDrawLine(renderer, x, y + h + rad, x + w, y + h + rad);
        arcRGBA(renderer, x + w, y + h, rad, 0, 90, 0, 0, 0, 255);
        arcRGBA(renderer, x, y + h,rad, 90, 180, 0, 0, 0, 255);
        arcRGBA(renderer, x, y, rad, 180, 270, 0, 0, 0, 255);
        arcRGBA(renderer, x + w, y, rad, 270, 360, 0, 0, 0, 255);
    }

    bool interact(Ball* ball) {
        pair<double, double> colCoord; // coordinates for ball during collision
        pair<double, double> collidVec; // vector of axis of collision
        double postTime; // remaining time after ball has collided with the surface
        double collidCompMag; // magnitude of colCoord vector
        double collidVecMag; // magnitude of collidVec vector
        double distAfterUpdate;
        // striking left wall
        if (ball->velX > 0 and abs(x - ball->x - rad - rad) < ball->velX * timeInt and ball->y +
        (x - ball->x - ball->rad - rad) * (ball->velY / ball->velX) >= y and ball->y + (x - ball->x - ball->rad - rad) * (ball->velY / ball->velX) <= y + h) {
            ball->y += ball->velY * timeInt;
            postTime = timeInt - ((x - ball->x - ball->rad - rad) / ball->velX);
            ball->x = x - ball->rad - rad - ball->velX * postTime;
            ball->velX *= -1;
            return true;
        }
            // striking right wall
        else if (ball->velX < 0 and abs(ball->x - w - x - ball->rad - rad) < -ball->velX * timeInt and ball->y +
        (ball->x - w - x - ball->rad - rad) * (ball->velY / -ball->velX) >= y and ball->y + (ball->x - w - x - ball->rad - rad) * (ball->velY / -ball->velX) <= y + h) {
            ball->y += ball->velY * timeInt;
            postTime = timeInt - ((ball->x - w - x - ball->rad - rad) / ball->velX);
            ball->x = x + w + ball->rad + rad - ball->velX * postTime;
            ball->velX *= -1;
            return true;
        }
            // striking top wall
        else if (ball->velY > 0 and abs(y - ball->y - ball->rad - rad) < ball->velY * timeInt and ball->x +
        (y - ball->y - ball->rad - rad) * (ball->velX / ball->velY) >= x and ball->x + (y - ball->y - ball->rad - rad) * (ball->velX / ball->velY) <= x + w) {
            ball->x += ball->velX * timeInt;
            postTime = timeInt - ((y - ball->y - ball->rad - rad) / ball->velY);
            ball->y = y - rad - rad - ball->velY * postTime;
            ball->velY *= -1;
            printf("bounce");
            return true;
        }
            // striking bottom wall
        else if (ball->velY < 0 and abs(ball->y - h - y - ball->rad - rad) < -ball->velY * timeInt and ball->x + (ball->y - h - y - ball->rad - rad) * (ball->velX / -ball->velY) >= x and ball->x + (ball->y - h - y - ball->rad - rad) * (ball->velX / -ball->velY) <= x + w) {
            ball->x += ball->velX * timeInt;
            postTime = timeInt - ((ball->y - h - y - ball->rad - rad) / ball->velY);
            ball->y = y + h + rad + rad - ball->velY * postTime;
            ball->velY *= -1;
            return true;
        }
        // striking top left corner
        distAfterUpdate = magnitude({ball->x + ball->velX * timeInt - x, ball->y + ball->velY * timeInt - y});
        if (distAfterUpdate < ball->rad + rad) {
            colCoord = ball->ballCollid(x, y, rad);
            if (colCoord.first != -1) {
                collidVec = {x - colCoord.first, y - colCoord.second};
                collidVecMag = magnitude(collidVec);
                collidCompMag = project({ball->velX, ball->velY}, collidVec);
                postTime = timeInt - abs((colCoord.first - ball->x) / ball->velX);
                ball->x = colCoord.first;
                ball->y = colCoord.second;
                ball->x += (ball->velX - (collidCompMag / collidVecMag) * collidVec.first) * postTime;
                ball->y += (ball->velY - (collidCompMag / collidVecMag) * collidVec.second) * postTime;
                ball->x -= (collidCompMag / collidVecMag) * collidVec.first * postTime;
                ball->y -= (collidCompMag / collidVecMag) * collidVec.second * postTime;
                ball->velX -= 2 * (collidCompMag / collidVecMag) * collidVec.first;
                ball->velY -= 2 * (collidCompMag / collidVecMag) * collidVec.second;
                return true;
            }
        }
        // striking top right corner
        distAfterUpdate = magnitude({ball->x + ball->velX * timeInt - x - w, ball->y + ball->velY * timeInt - y});
        if (distAfterUpdate < ball->rad + rad) {
            colCoord = ball->ballCollid(x + w, y, rad);
            if (colCoord.first != -1) {
                collidVec = {x + w - colCoord.first, y - colCoord.second};
                collidVecMag = magnitude(collidVec);
                collidCompMag = project({ball->velX, ball->velY}, collidVec);
                postTime = timeInt - abs((colCoord.first - ball->x) / ball->velX);
                ball->x = colCoord.first;
                ball->y = colCoord.second;
                ball->x += (ball->velX - (collidCompMag / collidVecMag) * collidVec.first) * postTime;
                ball->y += (ball->velY - (collidCompMag / collidVecMag) * collidVec.second) * postTime;
                ball->x -= (collidCompMag / collidVecMag) * collidVec.first * postTime;
                ball->y -= (collidCompMag / collidVecMag) * collidVec.second * postTime;
                ball->velX -= 2 * (collidCompMag / collidVecMag) * collidVec.first;
                ball->velY -= 2 * (collidCompMag / collidVecMag) * collidVec.second;
                return true;
            }
        }
        // striking bottom left corner
        distAfterUpdate = magnitude({ball->x + ball->velX * timeInt - x, ball->y + ball->velY * timeInt - y - h});
        if (distAfterUpdate < ball->rad + rad) {
            colCoord = ball->ballCollid(x, y + h, rad);
            if (colCoord.first != -1) {
                collidVec = {x - colCoord.first, y + h - colCoord.second};
                collidVecMag = magnitude(collidVec);
                collidCompMag = project({ball->velX, ball->velY}, collidVec);
                postTime = timeInt - abs((colCoord.first - ball->x) / ball->velX);
                ball->x = colCoord.first;
                ball->y = colCoord.second;
                ball->x += (ball->velX - (collidCompMag / collidVecMag) * collidVec.first) * postTime;
                ball->y += (ball->velY - (collidCompMag / collidVecMag) * collidVec.second) * postTime;
                ball->x -= (collidCompMag / collidVecMag) * collidVec.first * postTime;
                ball->y -= (collidCompMag / collidVecMag) * collidVec.second * postTime;
                ball->velX -= 2 * (collidCompMag / collidVecMag) * collidVec.first;
                ball->velY -= 2 * (collidCompMag / collidVecMag) * collidVec.second;
                return true;
            }
        }
        // striking bottom right corner
        distAfterUpdate = magnitude({ball->x + ball->velX * timeInt - x - w, ball->y + ball->velY * timeInt - y - h});
        if (distAfterUpdate < ball->rad + rad) {
            colCoord = ball->ballCollid(x + w, y + h, rad);
            if (colCoord.first != -1) {
                collidVec = {x + w - colCoord.first, y + h - colCoord.second};
                collidVecMag = magnitude(collidVec);
                collidCompMag = project({ball->velX, ball->velY}, collidVec);
                postTime = timeInt - abs((colCoord.first - ball->x) / ball->velX);
                ball->x = colCoord.first;
                ball->y = colCoord.second;
                ball->x += (ball->velX - (collidCompMag / collidVecMag) * collidVec.first) * postTime;
                ball->y += (ball->velY - (collidCompMag / collidVecMag) * collidVec.second) * postTime;
                ball->x -= (collidCompMag / collidVecMag) * collidVec.first * postTime;
                ball->y -= (collidCompMag / collidVecMag) * collidVec.second * postTime;
                ball->velX -= 2 * (collidCompMag / collidVecMag) * collidVec.first;
                ball->velY -= 2 * (collidCompMag / collidVecMag) * collidVec.second;
                return true;
            }
        }
        return false;
    }

};


#endif //GOLF_GAME_BLOCK_H
