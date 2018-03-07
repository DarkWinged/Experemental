//

#include "physicsObject.h"

// Created by levi on 11/30/17.

physicsObject::physicsObject(double initX, double initY, double initM) {
    position[0]=initX;
    position[1]=initY;
    velocity[0]=0;
    velocity[1]=0;
    color[0]= 0xFF;
    color[1]= 0xFF;
    color[2]= 0xFF;
    color[3]= 0xFF;
    mass= initM;

}

physicsObject::physicsObject(double initX, double initY, Uint8 initR, Uint8 initB, Uint8 initG, Uint8 initA, double initM) {
    position[0]= initX;
    position[1]= initY;
    velocity[0]= 0;
    velocity[1]= 0;
    color[0]= initR;
    color[1]= initB;
    color[2]= initG;
    color[3]= initA;
    mass= initM;
}


void physicsObject::evaluatePosition(double speed, double friction) {
    position[0] += velocity[0];
    position[1] += velocity[1];

    velocity[0] -= velocity[0]*friction;
    if(velocity[0] < friction*speed && velocity[0] > -(friction*speed))
        velocity[0] = 0;

    velocity[1] -= velocity[1]*friction;
    if(velocity[1] < friction*speed && velocity[1] > -(friction*speed))
        velocity[1] = 0;
}

bool physicsObject::getPosition(double* fillerArray) {
    fillerArray[0] = position[0];
    fillerArray[1] = position[1];
    if(fillerArray[0] == position[0] && fillerArray[1] == position[1] )
        return true;
    else
        return false;
}

void physicsObject::setPosition(double newX, double newY) {
    position[0] = newX;
    position[1] = newY;
}

void physicsObject::setPosition(const double *newPosition) {
    position[0] = newPosition[0];
    position[1] = newPosition[1];
}

void physicsObject::evaluateVelocity(double angle, double speed, double speed_limit) {
    double tempVect[2] {speed,0};

    physicsMath().rotateVect(tempVect, angle);

    velocity[0] += tempVect[0];
    velocity[1] += tempVect[1];

    double totalVelocity = sqrt(velocity[0]*velocity[0]+velocity[1]*velocity[1]);

    if(totalVelocity > speed_limit) {
        physicsMath().scaleVectorTo(velocity, speed_limit);
    }

    if(velocity[0] < 0.00001 && velocity[0] > -0.00001) {
        velocity[0] = 0;
    }

    if(velocity[1] < 0.00001 && velocity[1] > -0.00001) {
        velocity[1] = 0;
    }
}

void physicsObject::evaluateVelocity(double* tempVect, double speed_limit) {

    velocity[0] += tempVect[0];
    velocity[1] += tempVect[1];

    double totalVelocity = sqrt(velocity[0]*velocity[0]+velocity[1]*velocity[1]);

    if(totalVelocity > speed_limit) {
        physicsMath().scaleVectorTo(velocity, speed_limit);
    }

    if(velocity[0] < 0.00001 && velocity[0] > -0.00001) {
        velocity[0] = 0;
    }

    if(velocity[1] < 0.00001 && velocity[1] > -0.00001) {
        velocity[1] = 0;
    }
}

void physicsObject::setVelocity(double newX, double newY) {
    velocity[0] = newX;
    velocity[1] = newY;
}

void physicsObject::setVelocity(const double *newVelocity) {
    velocity[0] = newVelocity[0];
    velocity[1] = newVelocity[1];
}

bool physicsObject::getVelocity(double* fillerArray) {
    fillerArray[0] = velocity[0];
    fillerArray[1] = velocity[1];
    if(fillerArray[0] == velocity[0] && fillerArray[1] == velocity[1] )
        return true;
    else
        return false;
}

Uint8 *physicsObject::getColor(Uint8* fillerArray) {
    fillerArray[0] = color[0];
    fillerArray[1] = color[1];
    fillerArray[2] = color[2];
    fillerArray[3] = color[3];
    return fillerArray;
}

double physicsObject::getMass() {
    return mass;
}

void physicsObject::setMass(const double newMass) {
    mass = newMass;
}
