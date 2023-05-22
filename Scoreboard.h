#include <SDL.h>
#include <SDL_ttf.h>
#include "GameBase.h"
#include "GameObj.h"
using namespace std;

#ifndef GOLF_GAME_SCOREBOARD_H
#define GOLF_GAME_SCOREBOARD_H

class Scoreboard : public GameObj {
public:
    SDL_Surface* title2;
    int numPlayers;
    vector<int> score;
    vector<int> total;
    vector<SDL_Surface*> text;
    Scoreboard() {
        title2 = TTF_RenderText_Solid(font24, "Number of Swings", black);
    }
    Scoreboard(int players) {
        title2 = TTF_RenderText_Solid(font24, "Number of Swings", black);
        numPlayers =  players;
        for (int player = 0; player < players; player++) {
            score.push_back(0);
            total.push_back(0);
            msg = "Player " + to_string(player + 1) + ": " + to_string(score[player]);
            char final[msg.length()];
            for (int character = 0; character < msg.length(); character++) {
                final[character] = msg[character];
            }
            temp = TTF_RenderText_Solid(font18, final, black);
            text.push_back(temp);
        }
    }
    void update(int playerNum) {

        score[playerNum]++;
        msg = "Player " + to_string(playerNum + 1) + ": " + to_string(score[playerNum]);
        char final[msg.length()];
        for (int character = 0; character < msg.length(); character++) {
            final[character] = msg[character];
        }
        text[playerNum] = TTF_RenderText_Solid(font18, final, black);
    }
    void reset() {
        for (int player = 0; player < numPlayers; player++) {
            total[player] += score[player];
            score[player] = 0;
        }
    }
private:
    string msg;
    SDL_Surface* temp;
    SDL_Color black = {0, 0, 0, 255};
    TTF_Font* font18 = TTF_OpenFont("C:/Users/Aaron/fonts/SFCartoonistHand.ttf", 18);
    TTF_Font* font24 = TTF_OpenFont("C:/Users/Aaron/fonts/SFCartoonistHand.ttf", 24);


public:
    void draw(SDL_Renderer *renderer) {
        SDL_Texture* curText;
        SDL_Texture* titleTex;
        SDL_Rect pos;

        for (int curBall = 0; curBall < numPlayers; curBall++) {
            curText = SDL_CreateTextureFromSurface(renderer, text[curBall]);
            SDL_QueryTexture(curText, NULL, NULL, &pos.w, &pos.h);
            pos.y = 30 + 15 * curBall;
            pos.x = 900;
            SDL_RenderCopy(renderer, curText, NULL, &pos);
        }

        titleTex = SDL_CreateTextureFromSurface(renderer, title2);
        SDL_QueryTexture(titleTex, NULL, NULL, &pos.w, &pos.h);
        pos.y = 3;
        pos.x = 850;
        SDL_RenderCopy(renderer, titleTex, NULL, &pos);
    }

};

#endif //GOLF_GAME_SCOREBOARD_H
