//
//  System.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "System.h"
#include "Engine.h"
#include <math.h>

using namespace std;

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
quaternion padVector(vect v, float p)
{
    return (quaternion){p,v.x,v.y,v.z};
}
float magnitude(quaternion q)
{
    return sqrt(q.s * q.s + q.i * q.i + q.j * q.j + q.k * q.k);
}

Engine *System::gameEngine;

System::System()
{
    
}

void System::setGameEngine(Engine *gEngine)
{
    gameEngine = gEngine;
}

void System::removeComponent(componentID cid)
{
    components.erase(components.begin() + cid); //this method will cause problems because all componentID's larger than cid in the table are now invalid. Need to either update all cid's higher than cid OR modify component creation method and update mehtod so they handle a sparse component array
}

System::~System()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Physics::Physics() : System()
{
    vector<PhysicsComponent> components;
}

void Physics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        state *s = components.at(i).getState();
        
        
        s->orientation = scalarMultiply(s->orientation,1/magnitude(s->orientation));
        s->orientation = hMultiply(padVector(scalarMultiply(s->omega,dt/2), 1),s->orientation);
        
        //cout << "Updating PhysicsComponent with ID " << i << '\n' << "Address: " << &components.at(i);
        //cout << "Omega Components initial: " << s->omega.x << ", " << s->omega.y << ", " << s->omega.z << '\n';
        
        s->omega = addVect(s->omega, scalarMultiply(components.at(i).getAlpha(), dt));
        
        //cout << "Omega Components final: " << s->omega.x << ", " << s->omega.y << ", " << s->omega.z << "\n\n";
        
        s->pos = addVect(s->pos, scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
        
        //s->vel = addVect(s->vel, scalarMultiply(components.at(i).getAcceleration(), dt));
    }
}
componentID Physics::newComponent(entityID eid)
{
    state s;
    return newComponent(eid, s);
}

componentID Physics::newComponent(entityID eid, state s)
{
    componentID cid = (componentID)components.size();
    PhysicsComponent newComp = PhysicsComponent(cid, eid, s);
    components.push_back(newComp);
    cout << "PhysicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Physics components list.\n";
    return cid;
}
PhysicsComponent *Physics::getComponent(componentID cid)
{
    return &components.at(cid);
}
Physics::~Physics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Graphics::Graphics() : System()
{
    vector<GraphicsComponent> components;
}

void Graphics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        componentID phys = gameEngine->getPhysicsComponent(components.at(i).getEntity());
        PhysicsComponent physComp = *Engine::gamePhysics.getComponent(phys);
        vect p = physComp.getState()->pos;
        quaternion q = physComp.getState()->orientation;
        
        components.at(i).draw(p, q);
    }
}

componentID Graphics::newComponent(entityID eid)
{
    mesh m;
    return newComponent(eid, m);
}
componentID Graphics::newComponent(entityID eid, mesh m)
{
    componentID cid = (componentID)components.size();
    GraphicsComponent newComp(cid, eid, m);
    components.push_back(newComp);
    cout << "GraphicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Graphics components list.\n";
    return cid;
}
GraphicsComponent *Graphics::getComponent(componentID cid)
{
    return &components.at(cid);
}
Graphics::~Graphics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Logic::Logic() : System()
{
    vector<LogicComponent> components;
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
    cout << "LogicComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Logic components list.\n";
    return cid;
}
LogicComponent *Logic::getComponent(componentID cid)
{
    return &components.at(cid);
}
Logic::~Logic()
{
    
}