//
//  Component.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__Component__
#define __CppProgram__Component__

#include <iostream>

#endif /* defined(__CppProgram__Component__) */


typedef enum componentType {none = 0, physics = 1, graphics = 2, physics_and_graphics = 3, gamelogic = 4, physics_and_gamelogic = 5, graphics_and_gamelogic = 6, all = 7} componentType;
typedef struct position{double x; double y; double z;} position;
typedef struct velocity{double dx; double dy; double dz;}velocity;
typedef struct orientation{double alpha; double beta; double gamma;}orientation;
typedef struct angularVelocity{double dthetaX; double dthetaY; double dthetaZ;}angularVelocity;

typedef struct state{position pos; velocity vel; orientation theta; angularVelocity omega;}state;

using namespace std;

class Component
{
public:
    Component(unsigned int ident) : identity(ident), entityIdentity(-1)
    {
        
    }
    unsigned int getIdentity();
    unsigned int getEntity();
    
protected:
    unsigned int identity;
    unsigned int entityIdentity;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(unsigned int ident, state s) : Component(ident), entityState(s)
    {
        
    }
    state getState();
    void setState(state);
protected:
    state entityState;
};

class GraphicsComponent : public Component
{
public:
    GraphicsComponent(unsigned int ident) : Component(ident)
    {
        
    }
};

class GameLogicComponent : public Component
{
public:
    GameLogicComponent(unsigned int ident) : Component(ident)
    {
        
    }
};