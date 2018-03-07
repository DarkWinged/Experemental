//
// Created by levi on 11/27/17.
//

#include "inputController.h"


inputController::inputController(){
    for(int i=0; i < 3; i++)
        isPressed[i]=false;
}

void inputController::evaluateState(SDL_Event e){
    switch(e.key.keysym.sym)
    {
        case SDLK_w:
            if(e.key.state == SDL_PRESSED)
                isPressed[0] = true;
            else
                isPressed[0] = false;
            break;

        case SDLK_a:
            if(e.key.state == SDL_PRESSED)
                isPressed[1] = true;
            else
                isPressed[1] = false;
            break;

        case SDLK_s:
            if(e.key.state == SDL_PRESSED)
                isPressed[2] = true;
            else
                isPressed[2] = false;
            break;

        case SDLK_d:
            if(e.key.state == SDL_PRESSED)
                isPressed[3] = true;
            else
                isPressed[3] = false;
            break;
    }
}

bool inputController::checkState(unsigned char key)
{
    switch(key) {
        case 'w':
            return isPressed[0];
        case 'a':
            return isPressed[1];
        case 's':
            return isPressed[2];
        case 'd':
            return isPressed[3];
    }
}