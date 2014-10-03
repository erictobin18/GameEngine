//
//  Entity.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Entity.h"
#include "Engine.h"

Entity::Entity(gMath::entityID eid, std::string n) : identity(eid),components(0x0), name(n)
{
    for (int i = 0; i < 8; i++)
    {
        componentIDs[i] = 0;
    }
}
Entity::Entity(gMath::entityID eid)
{
    Entity(eid, "NULL");
}

void Entity::addPhysicsComponent(gMath::componentID cid)
{
    componentIDs[1] = cid;
    components = components | gMath::physicsType;
}
void Entity::addGraphicsComponent(gMath::componentID cid)
{
    componentIDs[2] = cid;
    components = components | gMath::graphicsType;
}
void Entity::addLogicComponent(gMath::componentID cid)
{
    componentIDs[4] = cid;
    components = components | gMath::logicType;
}


void Entity::removeComponent(gMath::bit_field type)
{
    if (hasComponent(type))
        components -= type;
}

bool Entity::hasComponent(gMath::bit_field type)
{
    return (components & type) > 0;
}

gMath::componentID Entity::getComponentID(gMath::bit_field type)
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