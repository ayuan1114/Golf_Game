#include <iostream>
#include "Framework.h"

using namespace std;

/**
 * main function that runs the gameloop and homeloop
 * @param argc
 * @param args
 */
int main(int argc, char* args[]) {
    SDL_Window *window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Init(SDL_INIT_EVERYTHING); // Initializing SDL as Video
    Framework* fw = new Framework(renderer);
    TTF_Init();
    if (fw->homeLoop()) {
        fw->addPlayers();
        fw->setUpLevels();
        fw->gameLoop();
    }
    return 0;
}
