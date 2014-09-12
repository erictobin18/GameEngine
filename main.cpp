//
//  main.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/9/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include <iostream>
#include "Engine.h"

using namespace std;

int main(int argc, const char * argv[])
{
    Engine gameEngine = Engine();
    gameEngine.mainloop();
    return EXIT_SUCCESS;
} //main