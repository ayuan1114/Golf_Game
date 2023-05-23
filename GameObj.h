//
// Created by Aaron on 5/15/2023.
//

#ifndef GOLF_GAME_3_GAMEOBJ_H
#define GOLF_GAME_3_GAMEOBJ_H

/**
 * interface for all objects that appear on screen
 */
class GameObj {
public:
    virtual void draw(SDL_Renderer *renderer) = 0;
};

#endif //GOLF_GAME_3_GAMEOBJ_H
