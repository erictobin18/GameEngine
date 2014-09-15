//
//  Component.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include "Component.h"
#endif

using namespace std;

componentID Component::getIdentity()
{
    return identity;
}
entityID Component::getEntity()
{
    return entityIdentity;
}

state * PhysicsComponent::getState()
{
    return & entityState;
}

void PhysicsComponent::setState(state s)
{
    entityState = s;
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