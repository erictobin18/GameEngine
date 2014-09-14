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

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

using namespace std;

int main(int argc, const char * argv[])
{
    Engine::gameEngine->createObject("rotatingCube");
    Engine::gameEngine->createObject("rotatingCube");
    Engine::gameEngine->mainloop();
    return EXIT_SUCCESS;
} //main