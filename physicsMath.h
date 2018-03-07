//
// Created by levi on 11/30/17.
//

#ifndef EXPERIMENTAL_PHYSICSMATH_H
#define EXPERIMENTAL_PHYSICSMATH_H

#include <cmath>
#include <iostream>

class physicsMath {
public:
    static void rotateVect(double *vect , double degree);
    static double angleTo(double *origin , double *target);
    static double findDistance(double *origin , double *target);
    static double* scaleVectorTo(double *vect, double targetLength);
    static double* scaleVector(double *vect, double scaleFactor);
};


#endif //EXPERIMENTAL_PHYSICSMATH_H
