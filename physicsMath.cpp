//
// Created by levi on 11/30/17.
//

#include "physicsMath.h"

void physicsMath::rotateVect(double *vect , double degree)
{
    double temp[2], limit = 0.000000000000001;
    long double x1= cos((M_PIl*degree)/180), y1 = sin((M_PIl*degree)/180), x2 = -sin((M_PIl*degree)/180), y2 = cos((M_PIl*degree)/180);

    temp[0] = static_cast<double>(vect[0]*x1 + vect[1]*x2);
    temp[1] = static_cast<double>(vect[0]*y1 + vect[1]*y2);

    if(temp[0] < limit && temp[0] > -limit)
        temp[0] = 0;

    if(temp[1] < limit && temp[1] > -limit)
        temp[1] = 0;

    vect[0] = temp[0];
    vect[1] = temp[1];
}

double physicsMath::findDistance(double *origin , double *target){
    double adj = target[0] - origin[0],  opp = target[1] - origin[1], result = 0;
    result = sqrt(adj*adj+opp*opp);
    return result;
}

double physicsMath::angleTo(double *origin , double *target){
    double adj = target[0] - origin[0],  opp = target[1] - origin[1], result = 0, limit = 0.000000000000001;
    result = atan2(opp,adj)*(180/M_PIl);
    if(result < limit && result > -limit)
        result = 0;
    return result;
}

double *physicsMath::scaleVectorTo(double *vect, double targetLength) {

    double vectLength = sqrt(vect[0]*vect[0]+vect[1]*vect[1]);
    vectLength = targetLength/vectLength;
    vect[0] *= vectLength;
    vect[1] *= vectLength;
    return vect;
}

double *physicsMath::scaleVector(double *vect, double scaleFactor) {
    vect[0] *= scaleFactor;
    vect[1] *= scaleFactor;
    return vect;
}
