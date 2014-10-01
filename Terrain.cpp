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
    for (int x = -8; x < 8; x+=2)
    {
        for (int y = -8; y < 8; y+=2)
        {
            for (int z = 0; z < 4; z+=2)
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
