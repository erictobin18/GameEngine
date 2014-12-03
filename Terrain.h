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
#include "GraphicsMath.h"
#include "ServerGL.h"

class Terrain
{
public:
    Terrain();
    
    void init();
    
    void draw();
    
    GLuint getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z,GLubyte blockID);
    std::vector<std::vector<std::vector<Chunk> > > chunks;
    int originX,originY,originZ;
};

#endif