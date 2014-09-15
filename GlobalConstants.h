//
//  GlobalConstants.h
//  CppProgram
//
//  Created by Eric Tobin on 9/13/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//


#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#endif


#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#endif


typedef enum componentType {noneType = 0, physicsType = 1, graphicsType = 2, physics_and_graphicsType = 3, logicType = 4, physics_and_logicType = 5, graphics_and_logicType = 6, all = 7} componentType;
typedef struct quaternion{float s; float i; float j; float k;}quaternion;

typedef struct vect{float x; float y; float z;} vect;

typedef struct state{vect pos; vect vel; quaternion orientation; vect omega;}state;

typedef struct vertex{float x; float y; float z; float texX; float texY;}vertex;

typedef struct rgb_value{unsigned char r; unsigned char g; unsigned char b;}rgb_value;

typedef struct mesh{std::vector<vertex> vertices; std::vector<unsigned int> indices; std::vector<unsigned char>texture; unsigned int texWidth; unsigned int texHeight;}mesh;

typedef struct file{state s; mesh m;}file;

typedef unsigned int componentID;

typedef unsigned int entityID;

typedef unsigned char bit_field;

typedef unsigned int textureID;