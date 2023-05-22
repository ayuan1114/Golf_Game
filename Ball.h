#include "GameBase.h"
#include "GameObj.h"
using namespace std;

#ifndef GOLF_GAME_BALL_H
#define GOLF_GAME_BALL_H

/**
 * calculates the magnitude of a vector
 * @param v a pair of doubles with first as x and second as y
 * @return double value representing its magnitude
 */
double magnitude(pair<double, double> v) { // does pythag to find magnitude
    return sqrt(v.first * v.first + v.second * v.second);
}

/**
 * finds the magnitude of the projection of v1 onto v2
 * @param v1 vector to project
 * @param v2 vector to project onto
 * @return the magnitude of v1 projected onto v2
 */
double project(pair<double, double> v1, pair<double, double> v2) {
    return (v1.first * v2.first + v1.second * v2.second) / magnitude(v2);
}

/**
 * Ball object is the gold ball that each player controls
 */

class Ball : public GameObj {
private:
    Sint16 dirArrowX[3];
    Sint16 dirArrowY[3];
public:
    double velCap = 75;
    double x, y;
    int rad;
    double accelX, accelY;
    double velX, velY;
    int r = 255, g = 255, b = 255;
    double mass;
    int show = 1;
    bool teed = false;
    bool started = false;
    bool stopped = true;
    int totalSwings = 0;
    double launchX = 0, launchY = 0, launchMag;


    Ball(double mass, int xPos, int yPos, int rad, double v_X, double v_Y, double a_X, double a_Y, int r, int g, int b){
        this->mass = mass;
        this->x = xPos;
        this->y = yPos;
        this->rad = rad;
        this->velX = v_X;
        this->velY = v_Y;
        this->accelX = a_X;
        this->accelY = a_Y;
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Ball(double mass, int xPos, int yPos, int rad, int r, int g, int b){
        this->mass = mass;
        this->x = xPos;
        this->y = yPos;
        this->rad = rad;
        this->r = r;
        this->g = g;
        this->b = b;
    }

    /**
     * updates the velocity of the ball on the x and y axis based on acceleration
     * @return true if the ball is still moving, false if the ball has stopped
     */

    bool updateVel() {
        double vMag = magnitude({velX, velY});
        double aMag = magnitude({accelX, accelY});
        if (aMag > 1) {
            velX += accelX * timeInt;
            velY += accelY * timeInt;
            return true;
        }
        else if (vMag > 0.5) {
            accelX -= velX / vMag; // applying friction
            accelY -= velY / vMag;
            velX += accelX * timeInt;
            velY += accelY * timeInt;
            if (vMag > velCap) {
                velX = velCap * (velX / vMag); // setting terminal
                velY = velCap * (velY / vMag);
            }
            return true;
        }
        else {
            velX = 0;
            velY = 0;
            return false;
        }
    }

    /**
     * updates the position of the ball along the x and y axis based on the velocity of the ball
     */

    void updatePos() {

        // right boundary
        if (x + rad + velX * timeInt > screenW) {
            x = screenW - rad - velX * (timeInt - ((screenW - x - rad) / velX));
            velX *= -1;
        }
        // left boundary
        if (x + velX * timeInt < rad) {
            x = rad + velX * (timeInt - ((x - rad) / velX));
            velX *= -1;
        }
        // bottom boundary
        if (y + rad + velY * timeInt > screenH) {
            y = screenH - rad - velY * (timeInt - ((screenH - y - rad) / velY));
            velY *= -1;
        }
        // upper boundary
        if (y + velY * timeInt < rad) {
            y = rad + velY * (timeInt - ((y - rad) / velY));
            velY *= -1;
        }
        x += velX * timeInt;
        y += velY * timeInt;
    }

    /**
     * get the position of the moving ball when it hits another ball
     * @param otherBallX x coordinate of the other ball's center
     * @param otherBallY y coordinate of the other ball's center
     * @param otherBallR radius of other ball
     * @return the position of this ball when the two balls collide
     */

    pair<double, double> ballCollid(double otherBallX, double otherBallY, double otherBallR) {
        double vMag = magnitude({velX, velY});
        double xCol, yCol;
        double distance = abs((velY * x - velX * y) - velY * otherBallX + velX * otherBallY) / vMag; // shortest distance between center of the other ball and the vector of motion
        printf("%f, %f\n", velX, velY);
        if (distance < rad + otherBallR) {
            if (velX == 0) {
                xCol = x;
                if (velY > 0) {
                    yCol = otherBallY - sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance);
                }
                else {
                    yCol = otherBallY + sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance);
                }
            }
            else if ((velY / velX) * (otherBallX - x) + y < otherBallY) { // if passing above center
                if (velY / velX < 0) { // if slope of ball path is negative
                    xCol = otherBallX - distance * abs(velY / vMag);
                    yCol = otherBallY - distance * abs(velX / vMag);
                }
                else { // if slope of ball path is positive
                    xCol = otherBallX + distance * abs(velY / vMag);
                    yCol = otherBallY - distance * abs(velX / vMag);
                }
                if (velX > 0) { // if ball moving right
                    xCol -= sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velX / vMag);
                }
                else { // if ball moving left
                    xCol += sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velX / vMag);
                }
                if (velY > 0) { // if ball moving down
                    yCol -= sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velY / vMag);
                }
                else { // if ball moving up
                    yCol += sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velY / vMag);
                }
            }
            else { // if passing below center
                if (velY / velX < 0) { // if slope of ball path is negative
                    xCol = otherBallX + distance * abs(velY / vMag);
                    yCol = otherBallY + distance * abs(velX / vMag);
                }
                else { // if slope of ball path is positive
                    xCol = otherBallX - distance * abs(velY / vMag);
                    yCol = otherBallY + distance * abs(velX / vMag);
                }
                if (velX > 0) { // if ball moving right
                    xCol -= sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velX / vMag);
                }
                else { // if ball moving left
                    xCol += sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velX / vMag);
                }
                if (velY > 0) { // if ball moving down
                    yCol -= sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velY / vMag);
                }
                else { // if ball moving up
                    yCol += sqrt((rad + otherBallR) * (rad + otherBallR) - distance * distance) * abs(velY / vMag);
                }
            }
            return {xCol, yCol};
        }
        return {-1, -1};
    }

    /**
     * checks for collision between this ball and another ball and will also calculate end velocities for both balls
     * @param otherBall the pointer for the Ball object that collision will be checked with
     * @return true if the balls collided, false if they won't
     */
    bool bounce(Ball* otherBall) { // V Check if sprites are touching V
        double distAfterUpdate = magnitude({x + velX * timeInt - otherBall->x, y + velY * timeInt - otherBall->y});
        if (distAfterUpdate <= rad + otherBall->rad and otherBall->show == 1 and show == 1) {
            pair<double, double> colCoord = ballCollid(otherBall->x, otherBall->y, otherBall->rad);
            pair<double, double> collidVec = {otherBall->x - colCoord.first, otherBall->y - colCoord.second}; // vector from center of moving ball to center of stationary ball
            double collidCompMag = project({velX, velY}, collidVec);
            double collidVecMag = magnitude(collidVec);
            double postTime;
            if (velX > 0) {
                postTime = timeInt - abs(x - colCoord.first) / velX;
            }
            else {
                postTime = timeInt - abs(y - colCoord.second) / velY;
            }
            x = colCoord.first;
            y = colCoord.second;
            x += (velX - (collidCompMag / collidVecMag) * collidVec.first) * postTime;
            y += (velY - (collidCompMag / collidVecMag) * collidVec.second) * postTime;
            x -= (collidCompMag / collidVecMag) * collidVec.first * postTime;
            y -= (collidCompMag / collidVecMag) * collidVec.second * postTime;
            velX -= (collidCompMag / collidVecMag) * collidVec.first;
            velY -= (collidCompMag / collidVecMag) * collidVec.second;
            otherBall->velX = (collidCompMag / collidVecMag) * collidVec.first;
            otherBall->velY = (collidCompMag / collidVecMag) * collidVec.second;
            otherBall->x += otherBall->velX * postTime;
            otherBall->y += otherBall->velY * postTime;
            return true;
        }
        return false;
    }

    /**
     * launches the ball
     * @return true if ball is launched, false if ball cannot be launched
     */
    bool launch() {
        if (stopped) {
            // launch ball
            velX = launchX;
            velY = launchY;
            launchX = 0;
            launchY = 0;

            stopped = false;
            teed = true;
            return true;
        }
        return false;
    }

    /**
     * changes the launch vector of the current ball
     * @param xy boolean to represent axis of change (false = x, true = y;
     * @param change the amount to change by along the chosen axis
     */

    void changeLaunchVect(bool xy, int change) {
        if (!stopped) {
            return;
        }
        if (xy) {
            if (launchMag < velCap) {
                launchY += change;
            }
            if (change > 0) {
                if (launchMag >= velCap and launchY < 0) {
                    launchY += 1;
                }
            }
            else {
                if (launchMag >= velCap and launchY > 0) {
                    launchY -= 1;
                }
            }
        }
        else {
            if (launchMag < velCap) {
                launchX += change;
            }
            if (change > 0) {
                if (launchMag >= velCap and launchX < 0) {
                    launchX += 1;
                }
            }
            else {
                if (launchMag >= velCap and launchX > 0) {
                    launchX -= 1;
                }
            }
        }
        launchMag = magnitude({launchX, launchY});
    }

    bool generateArrow() {
        if (launchMag < 5) {
            return false;
        }
        dirArrowX[0] = x + (launchX * 2);
        dirArrowY[0] = y + (launchY * 2);
        dirArrowX[1] = (-7 * cos(0.5)) * (launchX / launchMag) - (7 * sin(0.5)) * (launchY / launchMag) + dirArrowX[0];
        dirArrowY[1] = (-7 * cos(0.5)) * (launchY / launchMag) + (7 * sin(0.5)) * (launchX / launchMag) + dirArrowY[0];
        dirArrowX[2] = (-7 * cos(0.5)) * (launchX / launchMag) + (7 * sin(0.5)) * (launchY / launchMag) + dirArrowX[0];
        dirArrowY[2] = (-7 * cos(0.5)) * (launchY / launchMag) - (7 * sin(0.5)) * (launchX / launchMag) + dirArrowY[0];
        return true;
    }

    void drawLaunchVector(SDL_Renderer *renderer) {
        // draw launch vector
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, y, x + (launchX * 2), y + (launchY * 2));
        if (generateArrow()) {
            filledPolygonRGBA(renderer, dirArrowX, dirArrowY, 3, 0, 0, 0, 255);
        }
    }

public:
    void draw(SDL_Renderer *renderer) {
        filledCircleRGBA(renderer, x, y, rad, r, g, b, 255 * show);
        aacircleRGBA(renderer, x, y, rad, 0, 0, 0, 255 * show); // outline
    }

};


#endif //GOLF_GAME_BALL_H
