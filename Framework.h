#include <iostream>
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <SDL2_gfxPrimitives.h>
#include <cmath>
#include <SDL_ttf.h>
#include "Block.h"
#include "Boostpad.h"
#include "Blackhole.h"
#include "Finalhole.h"
#include "Ball.h"
#include "Level.h"
#include "Scoreboard.h"
using namespace std;

#ifndef GOLF_GAME_FRAMEWORK_H
#define GOLF_GAME_FRAMEWORK_H

class Framework{
public:

    // Constructor which initializes the parameters.
    Framework(SDL_Renderer *renderer) {
        this->renderer = renderer;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);      // setting draw color
        SDL_RenderClear(renderer);      // Clear the newly created window
        SDL_RenderPresent(renderer);    // Reflects the changes done in the
        //  window.
    }

    // Destructor
    ~Framework() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    /**
     * contains the data for all the obstacles to be added in each level
     */

    void setUpLevels() {
        // level 1
        levels.push_back(Level(20, 20, 100, 900));
        levels[0].addBlock(100, 100, 100, 100, 20);
        levels[0].addPad(500, 500, 200, 200, 5, 225);

        // level 2
        levels.push_back(Level(100, 100, 950, 950));
        levels[1].addBlock(50, 700, 700, 100, 20);
        levels[1].addHole(500, 350, 20, 300, 500);
    }

    void addPlayers() {
        balls.push_back(Ball(5, 20, 20, 10, 255, 255, 255));
        balls.push_back(Ball(5, 20, 20, 10, 0, 255, 150));

        //initialize scoreboard
        board = Scoreboard(balls.size());
    }

    void updateBall(int curBall) {
        // update the selected ball
        bool bounced = false;
        bool collided;
        bool interacted;

        // check if ball activates any obstacles
        for (int obstacle = 0; obstacle < levels[curLevel].obstacles.size(); obstacle++) {
            interacted = levels[curLevel].obstacles[obstacle]->interact(&balls[curBall]);
            if (interacted and obstacle == 0) {
                if (levelComplete()) {
                    return;
                }
            }
        }
        // update velocity of the ball and reset acceleration
        if (action) {
            balls[curBall].stopped = !balls[curBall].updateVel();
            balls[curBall].accelX = 0;
            balls[curBall].accelY = 0;
            if (balls[curBall].stopped and balls[curBall].teed) {
                balls[curBall].started = true;
                balls[curBall].teed = false;
                if (curBall == selectedBall) {
                    selectedBall++;
                    selectedBall %= balls.size();
                    while(!balls[selectedBall].show) {
                        selectedBall++;
                        selectedBall %= balls.size();
                    }
                }

            }
        }

        for (auto &wall : levels[curLevel].walls) {
            bounced = wall.interact(&balls[curBall]);
        }

        for (int otherBall = 0; otherBall < balls.size(); otherBall++) {
            if (otherBall != curBall and balls[otherBall].started) {
                collided = balls[curBall].bounce(&balls[otherBall]);
                if (collided) {
                    balls[otherBall].stopped = false;
                    updateBall(otherBall);
                }
            }
        }

        // update position of the ball
        if (action and !bounced) {
            balls[curBall].updatePos();
        }
    }

    bool levelComplete() {
        // check if all players have finished
        for (int curBall = 0; curBall < balls.size(); curBall++) {
            if (balls[curBall].show) {
                return false;
            }
        }

        // advance to next level
        curLevel++;
        curLevel %= levels.size();
        printf("shud run");
        for (int curBall = 0; curBall < balls.size(); curBall++) {
            balls[curBall].show = 1;
            balls[curBall].teed = false;
            balls[curBall].started = false;
            balls[curBall].totalSwings += board.score[curBall];
            balls[curBall].x = levels[curLevel].startX;
            balls[curBall].y = levels[curLevel].startY;
        }
        printf("done");
        board.reset();
        selectedBall = 0;
        return true;
    }

    void drawBackground() {
        // draw dark green tiles for background
        SDL_SetRenderDrawColor(renderer, 0, 235, 0, 255);
        for (int tileX = 0; tileX < screenW; tileX += 2 * tileS) {
            for (int tileY = 0; tileY < screenH; tileY += 2 * tileS) {
                SDL_Rect tile;
                tile.x = tileX;
                tile.y = tileY;
                tile.w = tileS;
                tile.h = tileS;
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        for (int tileX = tileS; tileX < screenW; tileX += 2 * tileS) {
            for (int tileY = tileS; tileY < screenH; tileY += 2 * tileS) {
                SDL_Rect tile;
                tile.x = tileX;
                tile.y = tileY;
                tile.w = tileS;
                tile.h = tileS;
                SDL_RenderFillRect(renderer, &tile);
            }
        }

        // draw light green tiles for background
        SDL_SetRenderDrawColor(renderer, 64, 255, 25, 255);
        for (int tileX = tileS; tileX < screenW; tileX += 2 * tileS) {
            for (int tileY = 0; tileY < screenH; tileY += 2 * tileS) {
                SDL_Rect tile;
                tile.x = tileX;
                tile.y = tileY;
                tile.w = tileS;
                tile.h = tileS;
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        for (int tileX = 0; tileX < screenW; tileX += 2 * tileS) {
            for (int tileY = tileS; tileY < screenH; tileY += 2 * tileS) {
                SDL_Rect tile;
                tile.x = tileX;
                tile.y = tileY;
                tile.w = tileS;
                tile.h = tileS;
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }

    void drawSelectedArrow(double ballX, double ballY) {
        Sint16 arrowX[3] = {(Sint16) ballX, (Sint16) (ballX - 3), (Sint16) (ballX + 3)};
        Sint16 arrowY[3] = {(Sint16) (ballY - 16 - arrowBounce.first), (Sint16) (ballY - 19 - arrowBounce.first), (Sint16) (ballY - 19 - arrowBounce.first)};
        if (frameCount % 10 == 0) {
            if (arrowBounce.first == 5) {
                arrowBounce.second = -1;
            }
            else if (arrowBounce.first == 0) {
                arrowBounce.second = 1;
            }
            arrowBounce.first += arrowBounce.second;
        }
        filledPolygonRGBA(renderer, arrowX, arrowY, 3, 0, 0, 0, 255);
    }

    /**
* method to draw all elements of the game
*/

    void drawGame() {

        SDL_RenderClear(renderer);

        // draw tiles for background
        drawBackground();

        // draw obstacles
        for (auto &obstacle : levels[curLevel].obstacles) {
            obstacle->draw(renderer);
        }

        // draw walls
        for (auto &wall : levels[curLevel].walls) {
            wall.draw(renderer);
        }

        // draw selected arrow
        if (balls[selectedBall].stopped) {
            drawSelectedArrow(balls[selectedBall].x, balls[selectedBall].y);
        }

        // draw launch vector
        balls[selectedBall].drawLaunchVector(renderer);

        // draw balls
        for(int curBall = balls.size() - 1; curBall >= 0; curBall--) {
            balls[curBall].draw(renderer);
        }

        // draw scoreboard
        SDL_Texture* text;
        SDL_Texture* titleTex;
        SDL_Rect pos;

        for (int curBall = 0; curBall < balls.size(); curBall++) {
            text = SDL_CreateTextureFromSurface(renderer, board.text[curBall]);
            SDL_QueryTexture(text, NULL, NULL, &pos.w, &pos.h);
            pos.y = 30 + 15 * curBall;
            pos.x = 900;
            SDL_RenderCopy(renderer, text, NULL, &pos);
        }

        titleTex = SDL_CreateTextureFromSurface(renderer, board.title2);
        SDL_QueryTexture(titleTex, NULL, NULL, &pos.w, &pos.h);
        pos.y = 3;
        pos.x = 850;
        SDL_RenderCopy(renderer, titleTex, NULL, &pos);

        // set background color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
    }

    bool homeLoop() {

        SDL_Rect startButton;
        startButton.x = 300;
        startButton.y = 475;
        startButton.w = 400;
        startButton.h = 50;

        SDL_Color white = {255, 255, 255, 255};
        TTF_Font* curFont = TTF_OpenFont("C:/Users/Aaron/fonts/SFCartoonistHand.ttf", 36);
        SDL_Surface* words = TTF_RenderText_Solid(curFont, "Start!", white);

        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, words);
        SDL_FreeSurface(words);
        SDL_Rect pos;
        SDL_QueryTexture(text, NULL, NULL, &pos.w, &pos.h);
        pos.y = 479;
        pos.x = 460;
        int close = 0;

        while (close != 1){
            SDL_RenderClear(renderer);
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        close = 1;
                        break;

                        // check for mouse movement
                    case SDL_MOUSEMOTION:
                        mouseX = event.motion.x;
                        mouseY = event.motion.y;
                        break;

                        // check for mouse click
                    case SDL_MOUSEBUTTONDOWN:
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (mouseX > startButton.x and mouseX < startButton.x + startButton.w and mouseY > startButton.y and mouseY < startButton.y + startButton.h) {
                                return true;
                            }
                        }
                        break;
                }
            }

            drawBackground();

            // draw button
            SDL_SetRenderDrawColor(renderer, 181, 101, 29, 255);
            SDL_RenderFillRect(renderer, &startButton);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &startButton);

            //draw text on button
            SDL_RenderCopy(renderer, text, NULL, &pos);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderPresent(renderer);

            SDL_Delay(1000 / fps);
            frameCount++;
            if (frameCount == 1000) {
                frameCount = 0;
            }
        }
    }

    void gameLoop() {

        int close = 0;
        while (close != 1) {
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        close = 1;
                        break;

                        // check for keystrokes
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) {
                            case SDL_SCANCODE_O:
                                action = !action;
                                break;

                            case SDL_SCANCODE_L:
                                selectedBall++;
                                selectedBall %= balls.size();
                                break;

                            case SDL_SCANCODE_K:
                                selectedBall--;
                                selectedBall += balls.size();
                                selectedBall %= balls.size();
                                break;

                            case SDL_SCANCODE_W:
                            case SDL_SCANCODE_UP:
                                if (action) {
                                    balls[selectedBall].changeLaunchVect(true, -1);
                                }
                                else {
                                    balls[selectedBall].y -= 5;
                                }
                                break;

                            case SDL_SCANCODE_A:
                            case SDL_SCANCODE_LEFT:
                                if (action) {
                                    balls[selectedBall].changeLaunchVect(false, -1);
                                }
                                else {
                                    balls[selectedBall].x -= 5;
                                }
                                break;

                            case SDL_SCANCODE_S:
                            case SDL_SCANCODE_DOWN:
                                if (action) {
                                    balls[selectedBall].changeLaunchVect(true, 1);
                                }
                                else {
                                    balls[selectedBall].y += 5;
                                }
                                break;

                            case SDL_SCANCODE_D:
                            case SDL_SCANCODE_RIGHT:
                                if (action) {
                                    balls[selectedBall].changeLaunchVect(false, 1);
                                }
                                else {
                                    balls[selectedBall].x += 5;
                                }
                                break;

                            case SDL_SCANCODE_SPACE:

                                if (balls[selectedBall].launch()) {
                                    // if launched update score
                                    board.update(selectedBall);
                                    updateBall(selectedBall);
                                }
                                break;

                            default:
                                break;
                        }
                }
            }
            drawGame();

            for (int ballNum = 0; ballNum < balls.size(); ballNum++) {
                if (!balls[ballNum].stopped) {
                    updateBall(ballNum);
                }
            }

            SDL_Delay(1000 / fps);
            frameCount++;
            if (frameCount == 1000) {
                frameCount = 0;
            }
        }
    }

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    Scoreboard board;
    vector<Level> levels;
    int curLevel = 0;
    vector<Ball> balls;
    int selectedBall = 0;
    bool action = true;
    int mouseX, mouseY;
    SDL_Color white = {255, 255, 255, 255};
    const int tileS = 20;
    // arrowBounce.second indicates how much to increment the position fo the arrow
    // ArrowBounce.first indicates the position offset of the bouncing arrow
    pair<int, int> arrowBounce = {0, 1};
};

#endif //GOLF_GAME_FRAMEWORK_H
