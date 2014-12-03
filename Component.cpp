//
//  Component.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Component.h"

using namespace std;

Component::Component(gMath::componentID cid, gMath::entityID eid) : identity(cid), entityIdentity(eid)
{
    cout << "Component with componentID " << cid << " linked to Entity with entityID " << eid << " created.\n";
}

gMath::componentID Component::getIdentity()
{
    return identity;
}
gMath::entityID Component::getEntity()
{
    return entityIdentity;
}

PhysicsComponent::PhysicsComponent(gMath::componentID cid, gMath::entityID eid, gMath::state s): Component(cid, eid), entityState(s), rollEnabled(true),gravityEnabled(true)
{
    
}
PhysicsComponent::PhysicsComponent(gMath::componentID cid, gMath::entityID eid) : Component(cid, eid)
{
    gMath::state s;
    PhysicsComponent(cid, eid, s);
}

gMath::state * PhysicsComponent::getState()
{
    return & entityState;
}

void PhysicsComponent::setState(gMath::state s)
{
    entityState = s;
}

void PhysicsComponent::setAlpha(gMath::vect alpha)
{
    entityAlpha = alpha;
    //cout << "Set Alpha for PhysicsComponent " << this << '\n';
}
gMath::vect PhysicsComponent::getAlpha()
{
    return entityAlpha;
}
void PhysicsComponent::setOmega(gMath::vect omega)
{
    entityState.omega = omega;
}
gMath::vect PhysicsComponent::getOmega()
{
    return entityState.omega;
}

void PhysicsComponent::setAcceleration(gMath::vect accel)
{
    entityAccel = accel;
}
gMath::vect PhysicsComponent::getAcceleration()
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


GraphicsComponent::GraphicsComponent(gMath::componentID cid, gMath::entityID eid, gMath::mesh m) : Component(cid, eid), componentMesh(m)
{
    GraphicsObject gObject(m);
    obj = gObject;
}
GraphicsComponent::GraphicsComponent(gMath::componentID cid, gMath::entityID eid) : Component(cid, eid)
{
    gMath::mesh m;
    GraphicsComponent(cid, eid, m);
}


gMath::mesh GraphicsComponent::getMesh()
{
    return componentMesh;
}

void GraphicsComponent::setMesh(gMath::mesh newMesh)
{
    componentMesh = newMesh;
}

void GraphicsComponent::draw(gMath::vect position, gMath::quaternion orientation)
{
    obj.draw(position, orientation);
}

LogicComponent::LogicComponent(gMath::componentID cid, gMath::entityID eid) : Component(cid, eid)
{
    
}

InputComponent::InputComponent(gMath::componentID cid, gMath::entityID eid) : Component(cid, eid)
{
    
}