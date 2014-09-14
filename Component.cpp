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

state PhysicsComponent::getState()
{
    return entityState;
}

void PhysicsComponent::setState(state s)
{
    entityState = s;
}

vector<vertex> GraphicsComponent::getVertices()
{
    return vertices;
}

void GraphicsComponent::setVertices(vector<vertex> v)
{
    vertices = v;
}