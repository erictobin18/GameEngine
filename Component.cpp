//
//  Component.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Component.h"

using namespace std;

Component::Component(componentID cid, entityID eid) : identity(cid), entityIdentity(eid)
{
    cout << "Component with componentID " << cid << " linked to Entity with entityID " << eid << " created.\n";
}

componentID Component::getIdentity()
{
    return identity;
}
entityID Component::getEntity()
{
    return entityIdentity;
}

PhysicsComponent::PhysicsComponent(componentID cid, entityID eid, state s): Component(cid, eid), entityState(s)
{
    
}
PhysicsComponent::PhysicsComponent(componentID cid, entityID eid) : Component(cid, eid)
{
    state s;
    PhysicsComponent(cid, eid, s);
}

state * PhysicsComponent::getState()
{
    return & entityState;
}

void PhysicsComponent::setState(state s)
{
    entityState = s;
}

void PhysicsComponent::setAlpha(vect alpha)
{
    entityAlpha = alpha;
    //cout << "Set Alpha for PhysicsComponent " << this << '\n';
}
vect PhysicsComponent::getAlpha()
{
    return entityAlpha;
}
void PhysicsComponent::setAcceleration(vect accel)
{
    entityAccel = accel;
}
vect PhysicsComponent::getAcceleration()
{
    return entityAccel;
}
void PhysicsComponent::killMotion()
{
    entityAccel = {0,0,0};
    entityAlpha = {0,0,0};
    entityState.vel = {0,0,0};
    entityState.omega = {0,0,0};
}


GraphicsComponent::GraphicsComponent(componentID cid, entityID eid, mesh m) : Component(cid, eid), componentMesh(m)
{
    GraphicsObject gObject(m);
    obj = gObject;
}
GraphicsComponent::GraphicsComponent(componentID cid, entityID eid) : Component(cid, eid)
{
    mesh m;
    GraphicsComponent(cid, eid, m);
}


mesh GraphicsComponent::getMesh()
{
    return componentMesh;
}

void GraphicsComponent::setMesh(mesh newMesh)
{
    componentMesh = newMesh;
}

void GraphicsComponent::draw(vect position, quaternion orientation)
{
    obj.draw(position, orientation);
}

LogicComponent::LogicComponent(componentID cid, entityID eid) : Component(cid, eid)
{
    
}