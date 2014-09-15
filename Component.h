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

#ifndef SERVER_GL_H
#define SERVER_GL_H
#include "ServerGL.h"
#endif

class Component
{
public:
    Component(componentID cid, entityID eid) : identity(cid), entityIdentity(eid) {}
    componentID getIdentity();
    entityID getEntity();
    
protected:
    componentID identity;
    entityID entityIdentity;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(componentID cid, entityID eid, state s) : Component(cid, eid), entityState(s) {}
    PhysicsComponent(componentID cid, entityID eid) : Component(cid, eid)
    {
        PhysicsComponent(cid, eid, *new state);
    }
    state *getState();
    void setState(state);
protected:
    state entityState;
};

class GraphicsComponent : public Component
{
public:
    GraphicsComponent(componentID cid, entityID eid, mesh m) : Component(cid, eid), componentMesh(m), obj(*new GraphicsObject(m)) {}
    GraphicsComponent(componentID cid, entityID eid) : Component(cid, eid)
    {
        GraphicsComponent(cid, eid, *new mesh);
    }
    
    mesh getMesh();
    void setMesh(mesh newMesh);
    std::vector<unsigned int> getIndices();
    void setIndices(std::vector<unsigned int> i);
    void draw(vect position, quaternion orientation);
    
    
protected:
    mesh componentMesh;
    GraphicsObject obj;
    
    
};

class LogicComponent : public Component
{
public:
    LogicComponent(componentID cid, entityID eid) : Component(cid, eid) {}
};