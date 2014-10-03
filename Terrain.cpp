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
  
    for (int x = 0; x < 1; x+=1)
    {
        for (int y = 0; y < 1; y+=1)
        {
            for (int z = 0; z < 1; z+=1)
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
