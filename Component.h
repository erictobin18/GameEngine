//
//  Component.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

class Component
{
public:
    Component(componentID cid, entityID eid) : identity(cid), entityIdentity(eid)
    {
        
    }
    componentID getIdentity();
    entityID getEntity();
    
protected:
    componentID identity;
    entityID entityIdentity;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(componentID cid, entityID eid, state s) : Component(cid, eid), entityState(s)
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
    GraphicsComponent(componentID cid, entityID eid) : Component(cid, eid)
    {
        
    }
    std::vector<face> getFaces();
    void setFaces(std::vector<face> f);
protected:
    std::vector<face> faces;
};

class GameLogicComponent : public Component
{
public:
    GameLogicComponent(componentID cid, entityID eid) : Component(cid, eid)
    {
        
    }
};