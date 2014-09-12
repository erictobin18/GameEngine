//
//  GameObject.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "GameObject.h"

void GameObject::init()
{
    
}

void GameObject::addComponent(bit_field type, unsigned int componentID)
{
    if ((type & 0x1) > 0)
    {
        componentIDs[1] = componentID;
    }
    else if ((type & 0x2) > 0)
    {
        componentIDs[2] = componentID;
    }
    else if ((type & 0x4) > 0)
    {
        componentIDs[4] = componentID;
    }
    components = components | type; //currently does not test that componentID is correct type of component
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

unsigned int GameObject::getComponentID(bit_field type)
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