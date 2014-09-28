//
//  GlobalConstants.h
//  CppProgram
//
//  Created by Eric Tobin on 9/13/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//


#ifndef _GLOBAL_CONSTANTS_H_
#define _GLOBAL_CONSTANTS_H_

#include <vector>


//make this a namespace
//don't need typedefs


typedef enum componentType {noneType = 0, physicsType = 1, graphicsType = 2, physics_and_graphicsType = 3, logicType = 4, physics_and_logicType = 5, graphics_and_logicType = 6, all = 7} componentType;

struct quaternion
{
    float s;
    float i;
    float j;
    float k;
};

struct vect{float x; float y; float z;};

struct state{vect pos; vect vel; quaternion orientation; vect omega;};

 struct vertex{float x; float y; float z; float texX; float texY;};

 struct rgb_value{unsigned char r; unsigned char g; unsigned char b;};

 struct mesh{std::vector<vertex> vertices; std::vector<unsigned int> indices; std::vector<unsigned char>texture; unsigned int texWidth; unsigned int texHeight;};

 struct file{mesh m;};

typedef unsigned int componentID;

typedef unsigned int entityID;

typedef unsigned char bit_field;

typedef unsigned int textureID;

#endif