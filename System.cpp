//
//  System.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include "System.h"
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

using namespace std;
Physics *Physics::gamePhysics = new Physics();
Graphics *Graphics::gameGraphics = new Graphics();
Logic *Logic::gameLogic = new Logic();



quaternion hMultiply(quaternion left, quaternion right)
{
    return (quaternion)
    {
        left.s*right.s - left.i*right.i - left.j*right.j - left.k*right.k,
        left.s*right.i + left.i*right.s + left.j*right.k - left.k*right.j,
        left.s*right.j - left.i*right.k + left.j*right.s + left.j*right.i,
        left.s*right.k + left.i*right.j - left.j*right.i + left.k*right.s
    };
}
vect addVect(vect a, vect b)
{
    return (vect){a.x + b.x, a.y + b.y, a.z + b.z};
}
vect scalarMultiply(vect v, double s)
{
    return (vect){v.x*s,v.y*s,v.z*s};
}
vect im(quaternion q)
{
    return (vect){q.i,q.j,q.k};
}
vect crossProduct(vect left, vect right)
{
    return (vect)
    {
        left.y*right.z - left.z*right.y,
        left.z*right.x - left.x*right.z,
        left.x*right.y - left.y*right.x
    };
}
vect applyQuaternion(vect initial, quaternion q)
{
    //v + 2rx(r x v + w v)
    return addVect(initial, scalarMultiply(crossProduct(im(q), addVect(crossProduct(im(q), initial), scalarMultiply(initial, q.s))), 2.0));
}
quaternion padVector(vect v, double p)
{
    return (quaternion){p,v.x,v.y,v.z};
}
double magnitude(vect v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


void System::removeComponent(componentID cid)
{
    components.erase(components.begin() + cid);
}

//---------------------------------------------------------------------------------------------------------------------

void Physics::update(double dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        state s = components.at(i).getState();
        s.pos = addVect(s.pos, scalarMultiply(s.vel, dt)); // x = x + dt*(dx/dt)
        vect unit = (vect){s.omega.x/magnitude(s.omega),s.omega.y/magnitude(s.omega),s.omega.z/magnitude(s.omega)};
        s.orientation = hMultiply(padVector(scalarMultiply(unit, dt), 1.0), s.orientation);
    }
}
void Physics::init()
{
    
}
componentID Physics::newComponent(entityID eid)
{
    state s = {0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0,0.0, 0.0,0.0,0.0};
    return newComponent(eid, s);
}

componentID Physics::newComponent(entityID eid, state s)
{
    componentID cid = (componentID)components.size();
    PhysicsComponent newComp = PhysicsComponent(cid, eid, s);
    components.push_back(newComp);
    return cid;
}
PhysicsComponent Physics::getComponent(componentID cid)
{
    return components.at(cid);
}
Physics::~Physics()
{
    
}
Physics& Physics::operator=(Physics other)
{
    swap(components, other.components);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------

void Graphics::update(double dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        componentID phys = Engine::gameEngine->getPhysicsComponent(components.at(i).getIdentity());
        PhysicsComponent physComp = Physics::gamePhysics->getComponent(phys);
        vect p = physComp.getState().pos;
        quaternion q = physComp.getState().orientation;
        
        
    }
}

void Graphics::drawFace(face f, vect pos, quaternion rot)
{
    
}
void Graphics::init()
{
    
}
componentID Graphics::newComponent(entityID eid)
{
    componentID cid = (componentID)components.size();
    GraphicsComponent newComp = GraphicsComponent(cid, eid);
    components.push_back(newComp);
    return cid;
}
GraphicsComponent Graphics::getComponent(componentID cid)
{
    return components.at(cid);
}
Graphics::~Graphics()
{
    
}
Graphics& Graphics::operator=(Graphics other)
{
    swap(components, other.components);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------

void Logic::update(double dt)
{
    //no gamelogic
}
void Logic::init()
{
    
}
componentID Logic::newComponent(entityID eid)
{
    componentID cid = (componentID)components.size();
    LogicComponent newComp = LogicComponent(cid, eid);
    components.push_back(newComp);
    return cid;
}
LogicComponent Logic::getComponent(componentID cid)
{
    return components.at(cid);
}
Logic::~Logic()
{
    
}
Logic& Logic::operator=(Logic other)
{
    swap(components, other.components);
    return *this;
}