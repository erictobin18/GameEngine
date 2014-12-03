//
//  main.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/9/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include <iostream>
#include "Engine.h"
#include "ServerGL.h"
#include "GraphicsMath.h"

using namespace std;

int main(int argc, const char *argv[])
{
    Engine gameEngine; //perhaps a static init method in Engine, which creates a static Engine in Engine?
                       //the createObject and mainloop methods would need to be static in that case
    
    gameEngine.init();
    
    //gameEngine.createObject("rotatingCube", (gMath::state){(gMath::vect){0,0,0}, (gMath::vect){0,0,0}, (gMath::quaternion){1.0,0.3,0.8,-0.2},(gMath::vect){0,0,0}});
    
    //gameEngine.createObject("rotatingCube", (gMath::state){(gMath::vect){-1,0,0}, (gMath::vect){1,0,0}, (gMath::quaternion){1.0,0.3,0.8,-0.2},(gMath::vect){0,0,0}});
    
    //gameEngine.createObject("rotatingCube", (state){(vect){ .75,-.75,0}, (vect){0,0,0}, (quaternion){1.0,0.0,0.0,0.0},(vect){.3,-.3,0}});
    
    //gameEngine.createObject("rotatingCube", (state){(vect){ .75, .75,0}, (vect){0,0,0}, (quaternion){1.0,0.0,0.0,0.0},(vect){.3,.3,0}});
    
    /*
    for (int i = 0; i < 27; i++)
    {
        gameEngine.createObject("rotatingCube", (state){(vect){0,0,0} , (vect){(i%3 - 1.0f)/15.0f,((i/3)%3 - 1.0f)/15.0f,((i/9)%3 - 1.0f)/15.0f} , (quaternion){1.0, 0.0, 0.0, 0.0} , (vect){(i%3 - 1.0f),((i/3)%3 - 1.0f),((i/9)%3 - 1.0f)}});
    }
    */
    
    gameEngine.mainloop(); //run!
    return EXIT_SUCCESS;
} //main