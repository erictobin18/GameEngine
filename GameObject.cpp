//
//  GameObject.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "GameObject.h"
#endif

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

using namespace std;


void GameObject::addComponent(bit_field type)
{
    if ((type & physicsType) > 0)
    {
        componentIDs[1] = Engine::gameEngine->newPhysicsComponent(identity);
    }
    else if ((type & graphicsType) > 0)
    {
        componentIDs[2] = Engine::gameEngine->newGraphicsComponent(identity);
    }
    else if ((type & logicType) > 0)
    {
        componentIDs[4] = Engine::gameEngine->newLogicComponent(identity);
    }
    components = components | type;
}

void GameObject::removeComponent(bit_field type)
{
    if (hasComponent(type))
        components -= type;
}

bool GameObject::hasComponent(bit_field type)
{
    return (components & type) > 0;
}

componentID GameObject::getComponentID(bit_field type)
{
    if (!hasComponent(type))
        return -1;
    if ((type & 0x1) > 0)
    {
        return componentIDs[1];
    }
    else if ((type & 0x2) > 0)
    {
        return componentIDs[2];
    }
    else if ((type & 0x4) > 0)
    {
        return componentIDs[4];
    }
    return -1;
}

int GameObject::numComponents()
{
    return (components & 0x1) + (components & 0x2) + (components & 0x4);
}