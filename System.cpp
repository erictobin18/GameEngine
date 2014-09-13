//
//  System.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "System.h"
void Physics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        state s = components.at(i).getState();
        s.pos.x += dt*s.vel.dx;
        s.pos.y += dt*s.vel.dy;
        s.pos.z += dt*s.vel.dy;
    }
}

void Physics::init()
{
    
}

unsigned int Physics::newComponent()
{
    state s = {0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0};
    unsigned int ident = (unsigned int)components.size();
    PhysicsComponent newComp = PhysicsComponent(ident, s);
    components.push_back(newComp);
    return ident;
}

void Physics::removeComponent(unsigned int componentID)
{
    components.erase(components.begin() + componentID);
}


Physics::~Physics()
{
    
}

void Graphics::update(float dt)
{
    //graphics are static
}
void Graphics::init()
{
    
}
Graphics::~Graphics()
{
    
}

void GameLogic::update(float dt)
{
    //no gamelogic
}
void GameLogic::init()
{
    
}
GameLogic::~GameLogic()
{
    
}