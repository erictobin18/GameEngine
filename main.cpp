//
//  main.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/9/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

#ifndef SERVER_GL_H
#define SERVER_GL_H
#include "ServerGL.h"
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

using namespace std;

int main(int argc, const char *argv[])
{
    //Engine::gameEngine->createObject("rotatingCube", (state){(vect){0,0,0}, (vect){0,0,0}, (quaternion){1.0,0.0,0.0,0.0},(vect){1.0,.5,.25}});
    
    Engine gameEngine;
    
    for (int i = 0; i < 27; i++)
    {
        gameEngine.createObject("rotatingCube", (state){(vect){0,0,0} , (vect){0,0,0} , (quaternion){1.0, 0.0, 0.0, 0.0} , (vect){static_cast<float>(i%3 - 1.0),static_cast<float>((i/3)%3 - 1.0),static_cast<float>((i/9)%3 - 1)}});
    }
    
     
    
    gameEngine.mainloop();
    return EXIT_SUCCESS;
} //main