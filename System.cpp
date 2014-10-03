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

Engine *System::gameEngine;

System::System()
{
    
}

void System::setGameEngine(Engine *gEngine)
{
    gameEngine = gEngine;
}

void System::removeComponent(gMath::componentID cid)
{
    components.erase(components.begin() + cid); //this method will cause problems because all componentID's larger than cid in the table are now invalid. Need to either update all cid's higher than cid OR modify component creation method and update mehtod so they handle a sparse component array
}

System::~System()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Physics::Physics() : System()
{
    std::vector<PhysicsComponent> components;
}

void Physics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        gMath::state *s = components.at(i).getState();
        
        s->orientation = scalarMultiply(s->orientation,1/gMath::magnitude(s->orientation)); //normalizes quaternion
        s->orientation = hMultiply(padVector(gMath::scalarMultiply(s->omega,dt/2), 1),s->orientation);
        
        //cout << "Updating PhysicsComponent with ID " << i << '\n' << "Address: " << &components.at(i);
        //cout << "Omega Components initial: " << s->omega.x << ", " << s->omega.y << ", " << s->omega.z << '\n';
        
        s->omega = addVect(s->omega, gMath::scalarMultiply(components.at(i).getAlpha(), dt));
        
        //cout << "Omega Components final: " << s->omega.x << ", " << s->omega.y << ", " << s->omega.z << "\n\n";
        
        s->pos = addVect(s->pos, gMath::scalarMultiply(s->vel, dt)); // x = x + dt*(dx/dt)
        
        //s->vel = addVect(s->vel, scalarMultiply(components.at(i).getAcceleration(), dt));
    }
}
gMath::componentID Physics::newComponent(gMath::entityID eid)
{
    gMath::state s;
    return newComponent(eid, s);
}

gMath::componentID Physics::newComponent(gMath::entityID eid, gMath::state s)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    PhysicsComponent newComp = PhysicsComponent(cid, eid, s);
    components.push_back(newComp);
    std::cout << "PhysicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Physics components list.\n";
    return cid;
}
PhysicsComponent *Physics::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Physics::~Physics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Graphics::Graphics() : System()
{
    std::vector<GraphicsComponent> components;
}

void Graphics::update(float dt)
{
    for (int i = 0; i < components.size(); i++)
    {
        gMath::componentID phys = gameEngine->getPhysicsComponent(components.at(i).getEntity());
        PhysicsComponent physComp = *Engine::gamePhysics.getComponent(phys);
        gMath::vect p = physComp.getState()->pos;
        gMath::quaternion q = physComp.getState()->orientation;
        
        components.at(i).draw(p, q);
    }
}

gMath::componentID Graphics::newComponent(gMath::entityID eid)
{
    gMath::mesh m;
    return newComponent(eid, m);
}
gMath::componentID Graphics::newComponent(gMath::entityID eid, gMath::mesh m)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    GraphicsComponent newComp(cid, eid, m);
    components.push_back(newComp);
    std::cout << "GraphicsComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Graphics components list.\n";
    return cid;
}
GraphicsComponent *Graphics::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Graphics::~Graphics()
{
    
}

//---------------------------------------------------------------------------------------------------------------------

Logic::Logic() : System()
{
    std::vector<LogicComponent> components;
}
void Logic::update(float dt)
{
    //no gamelogic
}

gMath::componentID Logic::newComponent(gMath::entityID eid)
{
    gMath::componentID cid = (gMath::componentID)components.size();
    LogicComponent newComp = LogicComponent(cid, eid);
    components.push_back(newComp);
    std::cout << "LogicComponent with componentID " << cid << " linked to Entity with entityID " << eid << " added to Logic components list.\n";
    return cid;
}
LogicComponent *Logic::getComponent(gMath::componentID cid)
{
    return &components.at(cid);
}
Logic::~Logic()
{
    
}