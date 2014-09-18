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
vect scalarMultiply(vect v, float s)
{
    return (vect){v.x*s,v.y*s,v.z*s};
}
quaternion scalarMultiply(quaternion q, float n)
{
    return (quaternion){q.s*n,q.i*n,q.j*n,q.k*n};
}
vect im(quaternion q)
{
    return (vect){q.i,q.j,q.k};
}
/*
vect crossProduct(vect left, vect right)
{
    return (vect)
    {
        left.y*right.z - left.z*right.y,
        left.z*right.x - left.x*right.z,
        left.x*right.y - left.y*right.x
    };
}
 */
quaternion padVector(vect v, float p)
{
    return (quaternion){p,v.x,v.y,v.z};
}
float magnitude(quaternion q)
{
    return sqrt(q.s * q.s + q.i * q.i + q.j * q.j + q.k * q.k);
}


void System::removeComponent(componentID cid)
{
    components.erase(components.begin() + cid);
}

//---------------------------------------------------------------------------------------------------------------------

Physics::Physics() : System()
{
    components = *new vector<PhysicsComponent>;
}

void Physics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        
        
        state *s = components.at(i).getState();
        
        
        s->orientation = scalarMultiply(s->orientation,1/magnitude(s->orientation));
        s->orientation = hMultiply(padVector(scalarMultiply(s->omega,dt/2), 1),s->orientation);
        
        
        
        //cout << s->pos.x << '\t' << s->pos.y << '\t' << s->pos.z << '\n';
        //cout << s->orientation.s << '\t' << s->orientation.i << '\t' << s->orientation.j << '\t' << s->orientation.k << '\n';
        
        s->pos = addVect(s->pos, scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
        
        //cout << s->pos.x << '\t' << s->vel.x << '\n';
    }
}
componentID Physics::newComponent(entityID eid)
{
    return newComponent(eid, *new state);
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
/*Physics& Physics::operator=(Physics other)
{
    swap(components, other.components);
    return *this;
}*/

//---------------------------------------------------------------------------------------------------------------------

Graphics::Graphics() : System()
{
    components = *new vector<GraphicsComponent>;
}

void Graphics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        componentID phys = Engine::gameEngine->getPhysicsComponent(components.at(i).getIdentity());
        PhysicsComponent physComp = Physics::gamePhysics->getComponent(phys);
        vect p = physComp.getState()->pos;
        quaternion q = physComp.getState()->orientation;
        
        components.at(i).draw(p, q);
    }
}

componentID Graphics::newComponent(entityID eid)
{
    return newComponent(eid, *new mesh);
}
componentID Graphics::newComponent(entityID eid, mesh m)
{
    componentID cid = (componentID)components.size();
    GraphicsComponent newComp = *new GraphicsComponent(cid, eid, m);
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
/*Graphics& Graphics::operator=(Graphics other)
{
    swap(components, other.components);
    return *this;
}*/

//---------------------------------------------------------------------------------------------------------------------

Logic::Logic() : System()
{
    components = *new vector<LogicComponent>;
}
void Logic::update(float dt)
{
    //no gamelogic
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
/*Logic& Logic::operator=(Logic other)
{
    swap(components, other.components);
    return *this;
}*/