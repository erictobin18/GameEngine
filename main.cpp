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
#include "GlobalConstants.h"

using namespace std;

int main(int argc, const char *argv[])
{
    Engine gameEngine; //perhaps a static init method in Engine, which creates a static Engine in Engine?
                       //the createObject and mainloop methods would need to be static in that case
    
    gameEngine.createObject("rotatingCube", (state){(vect){0,0,0}, (vect){0,0,0}, (quaternion){1.0,0.0,0.0,0.0},(vect){1.0,.5,.25}});
    
    /*
    for (int i = 0; i < 27; i++)
    {
        gameEngine.createObject("rotatingCube", (state){(vect){0,0,0} , (vect){0,0,0} , (quaternion){1.0, 0.0, 0.0, 0.0} , (vect){static_cast<float>(i%3 - 1.0),static_cast<float>((i/3)%3 - 1.0),static_cast<float>((i/9)%3 - 1)}});
    }
     */
    
    gameEngine.mainloop(); //run!
    return EXIT_SUCCESS;
} //main