//
//  Entity.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

using namespace std;

void Entity::addPhysicsComponent(componentID cid)
{
    componentIDs[1] = cid;
    components = components | physicsType;
}
void Entity::addGraphicsComponent(componentID cid)
{
    componentIDs[2] = cid;
    components = components | graphicsType;
}
void Entity::addLogicComponent(componentID cid)
{
    componentIDs[4] = cid;
    components = components | logicType;
}


void Entity::removeComponent(bit_field type)
{
    if (hasComponent(type))
        components -= type;
}

bool Entity::hasComponent(bit_field type)
{
    return (components & type) > 0;
}

componentID Entity::getComponentID(bit_field type)
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

int Entity::numComponents()
{
    return (components & 0x1) + (components & 0x2) + (components & 0x4);
}