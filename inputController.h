//
// Created by levi on 11/27/17.
//

#ifndef EXPERIMENTAL_INPUTCONTROLLER_H
#define EXPERIMENTAL_INPUTCONTROLLER_H
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


class inputController {
public:
    inputController();
    void evaluateState(SDL_Event e);
    bool checkState(unsigned char key);
private:
    bool isPressed[4];
};


#endif //EXPERIMENTAL_INPUTCONTROLLER_H
