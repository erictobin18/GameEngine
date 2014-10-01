//
//  Terrain.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/29/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Terrain.h"

Terrain::Terrain() : testChunk(Chunk(0,0,0)),testChunk2(Chunk(-1,0,0))
{
    for (int x = -4; x < 4; x++)
    {
        for (int y = -4; y < 4; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                Chunk temp(x,y,z);
                chunks.push_back(temp);
                chunks.at(chunks.size()-1).init(); //C++ IS PASS BY VALUE YOU IDIOT
            }
        }
    }
}

void Terrain::init()
{

}

void Terrain::draw()
{
    for (int i = 0; i < chunks.size(); i++)
    {
        chunks.at(i).draw();
    }
}
