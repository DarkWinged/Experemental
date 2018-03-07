/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "inputController.h"
#include "physicsMath.h"
#include "physicsObject.h"

//Screen dimension constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

bool init()
{
    //Initialization flag
    bool success = true;

    srand (time(NULL));

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "Experimental Test Zone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == nullptr)
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == nullptr )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Nothing to load
    return success;
}

void close()
{
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == nullptr )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void zoneWrap(physicsObject& wrapMe){
    double temp[2];
    wrapMe.getPosition(temp);
    double x=temp[0], y=temp[1];

    while(x < 0){
        wrapMe.setPosition(x+SCREEN_HEIGHT,y);
        wrapMe.getPosition(temp);
        x=temp[0];
        y=temp[1];
    }

    while(x > SCREEN_WIDTH){
        wrapMe.setPosition(x-SCREEN_HEIGHT,y);
        wrapMe.getPosition(temp);
        x=temp[0];
        y=temp[1];
    }

    while(y < 0){
        wrapMe.setPosition(x,y+SCREEN_HEIGHT);
        wrapMe.getPosition(temp);
        x=temp[0];
        y=temp[1];
    }

    while(y > SCREEN_HEIGHT){
        wrapMe.setPosition(x,y-SCREEN_HEIGHT);
        wrapMe.getPosition(temp);
        x=temp[0];
        y=temp[1];
    }
}

void zoneReflect(physicsObject& reflectMe){
    double temp1[2],temp2[2];
    reflectMe.getPosition(temp1);
    reflectMe.getVelocity(temp2);

    while(temp1[0] < 0){
        reflectMe.setPosition(-temp1[0],temp1[1]);
        reflectMe.setVelocity(-temp2[0],temp2[1]);
        reflectMe.getPosition(temp1);
        reflectMe.getVelocity(temp2);
    }

    while(temp1[0] > SCREEN_WIDTH){
        reflectMe.setPosition(SCREEN_HEIGHT-(temp1[0]-SCREEN_HEIGHT),temp1[1]);
        reflectMe.setVelocity(-temp2[0],temp2[1]);
        reflectMe.getPosition(temp1);
    }

    while(temp1[1] < 0){
        reflectMe.setPosition(temp1[0],-temp1[1]);
        reflectMe.setVelocity(temp2[0],-temp2[1]);
        reflectMe.getPosition(temp1);
    }

    while(temp1[1] > SCREEN_HEIGHT){
        reflectMe.setPosition(temp1[0],SCREEN_HEIGHT-(temp1[1]-SCREEN_HEIGHT));
        reflectMe.setVelocity(temp2[0],-temp2[1]);
        reflectMe.getPosition(temp1);
    }
}

void renderVelocity(physicsObject renderMe){
    double temp1[2],temp2[2];
    renderMe.getVelocity(temp1);
    renderMe.getPosition(temp2);

    physicsMath().scaleVector(temp1, 4);

    double temp3[]{temp1[0]+temp2[0],temp1[1]+temp2[1]}, temp4[]{4,0};

    physicsMath().rotateVect(temp4,physicsMath().angleTo(temp3,temp2)+30);

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderDrawLine( gRenderer, temp2[0], temp2[1], temp3[0], temp3[1]);
    SDL_RenderDrawLine( gRenderer, temp3[0], temp3[1], temp4[0]+temp3[0], temp4[1]+temp3[1]);
    physicsMath().rotateVect(temp4,-60);
    SDL_RenderDrawLine( gRenderer, temp3[0], temp3[1], temp4[0]+temp3[0], temp4[1]+temp3[1]);
}

void renderCharacter(physicsObject renderMe) {

    renderVelocity(renderMe);

    double temp1[2];
    renderMe.getPosition(temp1);
    double x=temp1[0],y=temp1[1], scale = 5;
    Uint8 temp2[4];
    renderMe.getColor(temp2);
    Uint8 R = temp2[0],B = temp2[1],G = temp2[2],A = temp2[3];

    SDL_Rect character = {x-(scale/2), y-(scale/2), scale, scale };
    SDL_SetRenderDrawColor( gRenderer, R, G, B, A );
    SDL_RenderFillRect( gRenderer, &character );
    /*
    if(x-10 < 0){
        character = {x-10+SCREEN_WIDTH, y-10, 20, 20 };
        SDL_SetRenderDrawColor( gRenderer, R, G, B, A );
        SDL_RenderFillRect( gRenderer, &character );
    }

    if(x+10 > SCREEN_HEIGHT){
        character = {x-10-SCREEN_WIDTH, y-10, 20, 20 };
        SDL_SetRenderDrawColor( gRenderer, R, G, B, A );
        SDL_RenderFillRect( gRenderer, &character );
    }

    if(y-10 < 0){
        character = {x-10, y-10+SCREEN_HEIGHT, 20, 20 };
        SDL_SetRenderDrawColor( gRenderer, R, G, B, A );
        SDL_RenderFillRect( gRenderer, &character );
    }

    if(y+10 > SCREEN_HEIGHT){
        character = {x-10, y-10-SCREEN_HEIGHT, 20, 20 };
        SDL_SetRenderDrawColor( gRenderer, R, G, B, A );
        SDL_RenderFillRect( gRenderer, &character );
    }*/


}

void repulseObject(physicsObject &object1, physicsObject &object2, const double speed, const double limit, double radius, double force) {

    double temp1[2], temp2[2], angle = 0;
    object1.getPosition(temp1);
    object2.getPosition(temp2);

    if(physicsMath().findDistance(temp1,temp2) < radius)
    {
        angle = physicsMath().angleTo(temp1,temp2);
        object1.evaluateVelocity(-angle,speed*force/object2.getMass(),limit);
        object2.evaluateVelocity(angle,speed*force/object2.getMass(),limit);
    }
}

void attractObject(physicsObject &object1, physicsObject &object2, const double speed, const double limit, double radius, double force) {

    double temp1[2], temp2[2], angle = 0;
    object1.getPosition(temp1);
    object2.getPosition(temp2);

    if(physicsMath().findDistance(temp1,temp2) < radius)
    {
        angle = physicsMath().angleTo(temp1,temp2);
        object1.evaluateVelocity(angle,speed*force/object2.getMass(),limit);
        object2.evaluateVelocity(-angle,speed*force/object2.getMass(),limit);
    }
}


void renderGrid(int gScale) {

    for(int i = 0; i <=SCREEN_HEIGHT; i += gScale * 16){
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
        for(int j=-2; j <= 2; j+=1)
            SDL_RenderDrawLine( gRenderer, 0, i + j, SCREEN_WIDTH, i + j);
    }

    for(int i = 0; i <=SCREEN_WIDTH; i += gScale * 16){
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
        for(int j=-2; j<= 2; j+=1)
            SDL_RenderDrawLine(gRenderer, i + j, 0, i + j, SCREEN_HEIGHT);
    }

    for(int i = 0; i <=SCREEN_HEIGHT; i += gScale*4){
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        for(int j=-1; j <= 1; j+=1)
            SDL_RenderDrawLine( gRenderer, 0, i + j, SCREEN_WIDTH, i + j);
    }

    for(int i = 0; i <=SCREEN_WIDTH; i += gScale*4){
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
        for(int j=-1; j<= 1; j+=1)
            SDL_RenderDrawLine(gRenderer, i + j, 0, i + j, SCREEN_HEIGHT);
    }

    for(int i = 0; i <=SCREEN_HEIGHT; i += gScale){
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
        SDL_RenderDrawLine( gRenderer, 0, i, SCREEN_WIDTH, i);
    }

    for(int i = 0; i <=SCREEN_WIDTH; i += gScale){
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
        SDL_RenderDrawLine( gRenderer, i, 0, i, SCREEN_HEIGHT);
    }

}

void applyState(physicsObject& player, inputController& controller, const double speed, const double limit) {
    if(controller.checkState('w'))
    {
        if (!controller.checkState('a') && !controller.checkState('d') && !controller.checkState('s'))
            player.evaluateVelocity(-90, speed, limit);
        else
        if(controller.checkState('a') && !controller.checkState('d'))
            player.evaluateVelocity(-135, speed, limit);
        else if(controller.checkState('d') && !controller.checkState('a'))
            player.evaluateVelocity(-45, speed, limit);
        else if(!controller.checkState('s'))
            player.evaluateVelocity(-90, speed, limit);


    }

    if(controller.checkState('s'))
    {
        if (!controller.checkState('a') && !controller.checkState('d') && !controller.checkState('w'))
            player.evaluateVelocity(90, speed, limit);
        else
        if(controller.checkState('a') && !controller.checkState('d'))
            player.evaluateVelocity(135, speed, limit);
        else if(controller.checkState('d') && !controller.checkState('a'))
            player.evaluateVelocity(45, speed, limit);
        else if(!controller.checkState('w'))
            player.evaluateVelocity(90, speed, limit);

    }

    if(controller.checkState('a'))
    {
        if (!controller.checkState('w') && !controller.checkState('s') && !controller.checkState('d'))
            player.evaluateVelocity(180, speed, limit);
        else if (controller.checkState('w') && controller.checkState('s') && !controller.checkState('d'))
            player.evaluateVelocity(180, speed, limit);

    }

    if(controller.checkState('d'))
    {
        if (!controller.checkState('w') && !controller.checkState('s') && !controller.checkState('a'))
            player.evaluateVelocity(0, speed, limit);
        else if (controller.checkState('w') && controller.checkState('s') && !controller.checkState('a'))
            player.evaluateVelocity(0, speed, limit);
    }


}

int main(int argc, char* args[] )
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            inputController controller;
            bool quit = false;
            const double speed = 1, scale =5, limit = 5, friction = 0.0001, gravity=.5, magnetizm = .5, nuclear =10;
            physicsObject player(SCREEN_WIDTH / 2*5, SCREEN_HEIGHT / 2*5,0xff,0xff,0xff,0xff,0);

            /*
            std::vector<physicsObject> objectS;
            for(int i=0; i < 8; i++){
                if(i%4 == 0)
                    objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0xff,0x00,0x00,0xff/2));
                if(i%4 == 1)
                    objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0x00,0xff,0x00,0xff/2));
                if(i%4 == 2)
                    objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0x00,0x00,0xff,0xff/2));
                if(i%4 == 3)
                    objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0xff,0x00,0xff,0xff/2));
            }
            */

            int electron=50, proton=50, neutron=60;

            std::vector<physicsObject> objectS;
            for(int i=0; i < electron; i++){
                    objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0xff,0x00,0xff,0xff/2,.0075));
            }
            for(int i=0; i < proton; i++){
                objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0xff,0xff,0x00,0xff/2,25));
            }
            for(int i=0; i < neutron; i++){
                objectS.push_back(physicsObject(rand() % SCREEN_WIDTH + 1,rand() % SCREEN_HEIGHT + 1,0x00,0xff,0xff,0xff/2,40));
            }

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit ) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT)
                        quit = true;

                    if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
                        controller.evaluateState(e);
                }

                applyState(player, controller, speed, limit);

                double temp1[2], temp2[2];
                double angle = 0;
                player.getPosition(temp2);

                //player physics manipulation
                for (int i = 0; i < objectS.size(); i++) {
                    objectS[i].getPosition(temp1);
                    if (physicsMath().findDistance(temp1, temp2) < scale) {
                        angle = physicsMath().angleTo(temp1, temp2);
                        if (i % 4 == 0 || i % 4 == 2)
                            objectS[i].evaluateVelocity(-angle, speed, limit);
                        else
                            objectS[i].evaluateVelocity(angle, speed, limit);
                    }
                }

                //gravity effects
                /*
                temp2[0] = 0;
                temp2[1] = 0;
                double totalMass;
                for (int i = 0; i < objectS.size(); i++) {
                    objectS[i].getPosition(temp1);
                    temp2[0] += temp1[0];
                    temp2[1] += temp1[1];
                    totalMass += objectS[i].getPosition(temp1);
                }
                temp2[0] /= objectS.size();
                temp2[1] /= objectS.size();
                for (int i = 0; i < objectS.size(); i++) {
                    objectS[i].getPosition(temp1);
                    double gravityFactor=gravity/(objectS[i].getMass()/totalMass);
                    angle = physicsMath().angleTo(temp1, temp2);
                    objectS[i].evaluateVelocity(angle, speed*gravityFactor/(physicsMath().findDistance(temp1, temp2)*.1), limit);
                }
                */
                for (int i = 0; i < objectS.size(); i++) {
                    double temp3[2];
                    objectS[i].getPosition(temp1);
                    double tempmass1 = objectS[i].getMass();

                    for (int o = i + 1; o < objectS.size(); o++){
                        objectS[o].getPosition(temp2);
                        double distance = physicsMath().findDistance(temp1,temp2);
                        double tempmass2 = objectS[i].getMass();

                        if((distance*scale) < 100 && distance > scale+5){

                            temp3[0] = (temp2[0]+temp1[0])/2;
                            temp3[1] = (temp2[1]+temp1[1])/2;

                            temp3[0] = (temp3[0]-temp1[0]);
                            temp3[1] = (temp3[1]-temp1[1]);
                            physicsMath().scaleVectorTo(temp3,(distance/2)*(tempmass1/tempmass2));
                            temp3[0] = (temp3[0]+temp1[0]);
                            temp3[1] = (temp3[1]+temp1[1]);

                            double temp4[]{temp3[0]-temp1[0],temp3[1]-temp1[1]},temp5[]{temp3[0]-temp2[0],temp3[1]-temp2[1]};


                            physicsMath().scaleVector(temp4,(gravity*((tempmass1+tempmass2)/2))/(distance*(tempmass1*speed)));
                            physicsMath().scaleVector(temp5,(gravity*((tempmass1+tempmass2)/2))/(distance*(tempmass2*speed)));

                            //physicsMath().scaleVectorTo(temp4,(gravity*(tempmass1+tempmass2))/distance);
                            //physicsMath().scaleVectorTo(temp5,(gravity*(tempmass1+tempmass2))/distance);

                            objectS[i].evaluateVelocity(temp4,limit);
                            objectS[o].evaluateVelocity(temp5,limit);
                        }

                    }
                }
                //repulsion calculations
                /*
                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        repulseObject(objectS[i], objectS[o], speed, limit, 20, repulsionForce);

                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if (i % 4 != (o - 2) % 4)
                            repulseObject(objectS[i], objectS[o], speed, limit, 40, repulsionForce);

                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if (i % 4 != (o - 2) % 4 || i % 4 != (o - 1) % 4)
                            repulseObject(objectS[i], objectS[o], speed, limit, 60, repulsionForce);

                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if (i % 4 != (o - 2) % 4 || i % 4 != (o - 1) % 4 || i % 4 != (o + 1) % 4)
                            repulseObject(objectS[i], objectS[o], speed, limit, 80, repulsionForce);
                */
                /*
                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++){
                        if ((i % 5 == 0 || i % 5 == 4) && (o % 5 == 0 || i % o == 4))
                            repulseObject(objectS[i], objectS[o], speed, limit, 40, repulsionForce);
                        else if((i % 5 == 2 || i % 5 == 1) && (o % 5 == 1 || i % o == 2))
                            repulseObject(objectS[i], objectS[o], speed, limit, 40, repulsionForce);

                        repulseObject(objectS[i], objectS[o], speed, limit, 20, repulsionForce);
                    }
                */


                for (int i = 0; i < electron; i++) {
                    double temp3[2];
                    objectS[i].getPosition(temp1);
                    for (int o = i + 1; o < electron; o++) {
                        objectS[o].getPosition(temp2);
                        double distance = physicsMath().findDistance(temp1,temp2);
                        if(distance*scale < 10){
                            temp3[0] = 1;
                            temp3[1] = 0;

                            angle = physicsMath().angleTo(temp1,temp2);
                            physicsMath().rotateVect(temp3,-angle);

                            physicsMath().scaleVectorTo(temp3,(magnetizm/objectS[i].getMass())/(distance/scale));

                            objectS[i].evaluateVelocity(temp3,limit);

                            physicsMath().rotateVect(temp3,angle*180);
                            objectS[o].evaluateVelocity(temp3,limit);
                        }
                    }
                }

                for (int i = electron; i < proton+electron; i++) {
                    double temp3[2];
                    objectS[i].getPosition(temp1);
                    for (int o = i + 1; o < proton+electron; o++) {
                        objectS[o].getPosition(temp2);
                        double distance = physicsMath().findDistance(temp1,temp2);
                        if(distance < 5*scale){
                            temp3[0] = 1;
                            temp3[1] = 0;

                            angle = physicsMath().angleTo(temp1,temp2);
                            physicsMath().rotateVect(temp3,-angle);

                            physicsMath().scaleVectorTo(temp3,(magnetizm/objectS[i].getMass())/(distance/scale));

                            objectS[i].evaluateVelocity(temp3,limit);

                            physicsMath().rotateVect(temp3,angle*180);
                            objectS[o].evaluateVelocity(temp3,limit);
                        }
                    }
                }

                for (int i = 0; i < electron; i++) {
                    double temp3[2];
                    objectS[i].getPosition(temp1);
                    for (int o = electron; o < proton+electron; o++) {
                        objectS[o].getPosition(temp2);
                        double distance = physicsMath().findDistance(temp1,temp2);
                        if(distance < 20*scale){
                            temp3[0] = 1;
                            temp3[1] = 0;

                            angle = physicsMath().angleTo(temp1,temp2);
                            physicsMath().rotateVect(temp3,angle);

                            physicsMath().scaleVectorTo(temp3,magnetizm/(distance/scale));

                            objectS[i].evaluateVelocity(temp3,limit);

                            physicsMath().rotateVect(temp3,angle*180);
                            objectS[o].evaluateVelocity(temp3,limit);
                        }
                    }
                }

                for (int i = electron; i < proton+electron; i++) {
                    double temp3[2];
                    objectS[i].getPosition(temp1);
                    for (int o = proton+electron; o < objectS.size(); o++) {
                        objectS[o].getPosition(temp2);
                        double distance = physicsMath().findDistance(temp1,temp2);
                        if(distance < 10*scale){
                            temp3[0] = 1;
                            temp3[1] = 0;

                            angle = physicsMath().angleTo(temp1,temp2);
                            physicsMath().rotateVect(temp3,angle);

                            physicsMath().scaleVectorTo(temp3,nuclear/(distance/scale));

                            objectS[i].evaluateVelocity(temp3,limit);

                            physicsMath().rotateVect(temp3,angle*180);
                            objectS[o].evaluateVelocity(temp3,limit);
                        }
                    }
                }
                //attaction calculations
                /*
                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if (i % 4 == (o - 2) % 4)
                            attractObject(objectS[i], objectS[o], speed, limit, 60, attractionForce);

                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if (i % 4 == (o - 2) % 4 || i % 4 == (o - 1) % 4)
                            attractObject(objectS[i], objectS[o], speed, limit, 80, attractionForce);

                for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        attractObject(objectS[i], objectS[o], speed, limit, 100, attractionForce);
                */
                /*for (int i = 0; i < objectS.size(); i++)
                    for (int o = i + 1; o < objectS.size(); o++)
                        if ((i % 5 == 0 || i % 5 == 4) && (o % 5 == 1 || o % 5 == 2))
                            attractObject(objectS[i], objectS[o], speed, limit, 960, attractionForce);
                */


                //player.evaluatePosition(speed,friction);

                for(int i=0; i < objectS.size(); i++){
                    objectS[i].evaluatePosition(speed,friction);
                }

                /*player.getPosition(temp1);
                std::cout<<"player.position= ";
                std::cout<<temp1[0];
                std::cout<<",";
                std::cout<<temp1[1];
                std::cout<<"\n";*/

                //zoneReflect(player);

                //border effecs
                for(int i=0; i < objectS.size(); i++){
                        zoneReflect(objectS[i]);
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderClear( gRenderer );

                //render background grid
                renderGrid(20);

                //render physics objects
                for(int i=0; i < objectS.size(); i++){
                    renderCharacter(objectS[i]);
                }

                //renderCharacter(player);

                //Update screen
                SDL_RenderPresent( gRenderer );

                SDL_Delay(1000/120);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}