//
//  Terrain.h
//  CppProgram
//
//  Created by Eric Tobin on 9/29/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _Terrain_
#define _Terrain_

#include <stdio.h>
#include "GlobalConstants.h"
#include "ServerGL.h"

class Terrain
{
public:
    Terrain();
    
    void init();
    
    void draw();
    std::vector<Chunk> chunks;
};

#endif