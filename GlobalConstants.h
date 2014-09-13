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
typedef struct quaternion{double s; double i; double j; double k;}quaternion;

typedef struct vect{double x; double y; double z;} vect;

typedef struct state{vect pos; vect vel; quaternion orientation; vect omega;}state;

typedef struct vertex{double x; double y; double z; double texX; double texY;}vertex;

typedef struct face{std::vector<vertex> vtx; int texID;}face;

typedef unsigned int componentID;

typedef unsigned int entityID;

typedef unsigned char bit_field;