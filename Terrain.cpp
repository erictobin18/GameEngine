//
//  Terrain.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/29/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Terrain.h"

Terrain::Terrain()
{
    //Chunk testChunk;
    for (unsigned char x = 0; x < 16; x++)
    {
        for (unsigned char y = 0; y < 16; y++)
        {
            for (unsigned char z = 0; z < 16; z++)
            {
                //testChunk.setBlock(x, y, z, 5);
                testChunk.setBlock(x, y, z, std::rand()%256 );
                //std::cout << testChunk.getBlock(x, y, z) << '\n';
            }
        }
    }
    
}

void Terrain::draw()
{
    for (unsigned char x = 0; x < 16; x++)
    {
        for (unsigned char y = 0; y < 16; y++)
        {
            for (unsigned char z = 0; z < 16; z++)
            {
                //testChunk.setBlock(x, y, z, 5);
                testChunk.setBlock(x, y, z, std::rand()%256 );
                //std::cout << testChunk.getBlock(x, y, z) << '\n';
            }
        }
    }
    testChunk.needsUpdate();
    testChunk.draw();
}
