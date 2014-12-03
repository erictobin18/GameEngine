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
    originX = -4;
    originY = -4;
    originZ = 0;
    chunks.resize(1);
    for (int z = 0; z < 1; z+=1)
    {
        chunks.at(z).resize(8);
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                Chunk temp(x + originX, y + originY, z);
                chunks.at(z).at(x).push_back(temp);
                chunks.at(z).at(x).at(chunks.at(z).at(x).size()-1).init();
            }
        }
    }
}

void Terrain::init()
{

}

GLuint Terrain::getBlock(int x, int y, int z)
{
    int cX = (x - originX)/CHUNK_SIZE;
    int cY = (y - originY)/CHUNK_SIZE;
    int cZ = (z - originZ)/CHUNK_SIZE;
    
    if (cX < 0 or cY < 0 or cZ < 0 or cZ >= chunks.size() or cX >= chunks.at(0).size() or cY >= chunks.at(0).at(0).size())
        return 0;
    else
        return chunks.at(cZ).at(cX).at(cY).getBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE);
    return 0;
}

void Terrain::setBlock(int x, int y, int z,GLubyte blockID)
{
    int cX = (x - originX)/CHUNK_SIZE;
    int cY = (y - originY)/CHUNK_SIZE;
    int cZ = (z - originZ)/CHUNK_SIZE;
    
    if (cX < 0 or cY < 0 or cZ < 0 or cZ >= chunks.size() or cX >= chunks.at(0).size() or cY >= chunks.at(0).at(0).size())
        std::cout << "WARNING: TRIED TO MODIFY BLOCK OUTSIDE WORLD\n";
    else
        chunks.at(cZ).at(cX).at(cY).setBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE, blockID);
}

void Terrain::draw()
{
    for (int z = 0; z < chunks.size(); z++)
    {
        for (int x = 0; x < chunks.at(z).size(); x++)
        {
            for (int y = 0; y < chunks.at(z).at(x).size(); y++)
            {
                chunks.at(z).at(x).at(y).draw();
            }
        }
    }
     
}
