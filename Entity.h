//
//  Entity.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef ENTITY_H
#define ENTITY_H
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif


class Engine;

class Entity
{
public:
    Entity(entityID eid, std::string n) : identity(eid),components(0x0), name(n)
    {
        for (int i = 0; i < 8; i++)
        {
            componentIDs[i] = 0;
        }
    }
    Entity(entityID eid)
    {
        Entity(eid, "NULL");
    }
    
    void addPhysicsComponent(componentID cid);
    void addGraphicsComponent(componentID cid);
    void addLogicComponent(componentID cid);
    
    void removeComponent(bit_field type);
    bool hasComponent(bit_field type);
    componentID getComponentID(bit_field type);
    
    bit_field components;
    
protected:
    int numComponents();
    componentID componentIDs[8];
    entityID identity; //index in the Master Object Table
    std::string name;
};