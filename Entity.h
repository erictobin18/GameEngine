//
//  Entity.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <iostream>
#include "GlobalConstants.h"

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

#endif