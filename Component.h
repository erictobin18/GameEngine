//
//  Component.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <iostream>
#include "GlobalConstants.h"
#include "ServerGL.h" //required for some reason to declare: GraphicsObject obj;

class Component //contains data required to update a specific type of system (e.g. "Physics")
{
public:
    Component(componentID cid, entityID eid); 
    componentID getIdentity(); //returns this component's componentID (index in the component table in the appropriate system)
    entityID getEntity(); //returns the index of the associated entity in the objectTable (in the engine)
    
protected:
    componentID identity;
    entityID entityIdentity;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(componentID cid, entityID eid, state s); 
    PhysicsComponent(componentID cid, entityID eid);
    
    state *getState(); //accessors for entityState
    void setState(state);
protected:
    state entityState;
};

class GraphicsComponent : public Component
{
public:
    GraphicsComponent(componentID cid, entityID eid, mesh m);
    GraphicsComponent(componentID cid, entityID eid);
    
    mesh getMesh(); //accessors for componentMesh
    void setMesh(mesh newMesh);
    void draw(vect position, quaternion orientation); //draws the component by invoking OpenGL server drawing command
    
protected:
    mesh componentMesh;
    GraphicsObject obj;
};

class LogicComponent : public Component
{
public:
    LogicComponent(componentID cid, entityID eid);
};

#endif