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
#include "GraphicsMath.h"
#include "ServerGL.h" //required for some reason to declare: GraphicsObject obj;

class Component //contains data required to update a specific type of system (e.g. "Physics")
{
public:
    Component(gMath::componentID cid, gMath::entityID eid);
    gMath::componentID getIdentity(); //returns this component's componentID (index in the component table in the appropriate system)
    gMath::entityID getEntity(); //returns the index of the associated entity in the objectTable (in the engine)
    
protected:
    gMath::componentID identity;
    gMath::entityID entityIdentity;
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent(gMath::componentID cid, gMath::entityID eid, gMath::state s);
    PhysicsComponent(gMath::componentID cid, gMath::entityID eid);
    
    gMath::state *getState(); //accessors for entityState
    void setState(gMath::state);
    void setAlpha(gMath::vect alpha);
    gMath::vect getAlpha(); //PASS BY VALUE!!! CANNOT ACCESS!!! FUCK JAVA
    void setAcceleration(gMath::vect accel);
    gMath::vect getAcceleration(); //PASS BY VALUE!!! CANNOT ACCESS!!! FUCK JAVA
    void setOmega(gMath::vect omega);
    gMath::vect getOmega();
    void killMotion();
    
    bool gravityEnabled;
    bool terrainEnabled;
    bool mouseControl;
    
    bool onGround;
protected:
    gMath::state entityState;
    gMath::vect entityAlpha;
    gMath::vect entityAccel;
};

class GraphicsComponent : public Component
{
public:
    GraphicsComponent(gMath::componentID cid, gMath::entityID eid, gMath::mesh m);
    GraphicsComponent(gMath::componentID cid, gMath::entityID eid);
    
    gMath::mesh getMesh(); //accessors for componentMesh
    void setMesh(gMath::mesh newMesh);
    void draw(gMath::vect position, gMath::quaternion orientation); //draws the component by invoking OpenGL server drawing command
    
protected:
    gMath::mesh componentMesh;
    GraphicsObject obj;
};

class LogicComponent : public Component
{
public:
    LogicComponent(gMath::componentID cid, gMath::entityID eid);
};

class InputComponent : public Component
{
public:
    InputComponent(gMath::componentID cid, gMath::entityID eid);
};

#endif