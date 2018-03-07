//
// Created by levi on 11/30/17.
//

#ifndef EXPERIMENTAL_PHYSICSOBJECT_H
#define EXPERIMENTAL_PHYSICSOBJECT_H

#include <SDL.h>
#include "physicsMath.h"
#include <iostream>


class physicsObject {
    double position[2], velocity[2], mass;
    Uint8 color[4];
public:
    physicsObject(double initX, double initY, double initM);
    physicsObject(double initX, double initY, Uint8 initR, Uint8 initB, Uint8 initG, Uint8 initA, double initM);

    void evaluatePosition(double speed, double friction);
    bool getPosition(double* fillerArray);
    void setPosition(const double newX, const double newY);
    void setPosition(const double* newPosition);
    void evaluateVelocity(double angle, double speed, double speed_limit);
    void setVelocity(const double newX, const double newY);
    void setVelocity(const double* newVelocity);
    bool getVelocity(double* fillerArray);
    double getMass();
    void setMass(const double newMass);
    Uint8* getColor(Uint8* fillerArray);

    void evaluateVelocity(double *tempVect, double speed_limit);
};


#endif //EXPERIMENTAL_PHYSICSOBJECT_H
