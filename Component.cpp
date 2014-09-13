//
//  Component.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Component.h"
unsigned int Component::getIdentity()
{
    return identity;
}
unsigned int Component::getEntity()
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

vector<face> GraphicsComponent::getFaces()
{
    return faces;
}

void GraphicsComponent::setFaces(vector<face> f)
{
    faces = f;
}