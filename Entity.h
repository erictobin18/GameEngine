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
    Entity(entityID eid, std::string n);
    Entity(entityID eid);
    
    void addPhysicsComponent(componentID cid); //Each entity can only have at most one component of each type
    void addGraphicsComponent(componentID cid); //These methods add them to the entity if they do not already exist
    void addLogicComponent(componentID cid);
    void removeComponent(bit_field type); //removes a component of a specific type
    bool hasComponent(bit_field type); //checks whether a component of a specified type is associated with this entity
    componentID getComponentID(bit_field type);
    
    bit_field components; //keeps track of which component types are associated with this entity.
                          //may be faster to mask this bitfield than to call "hasComponent"
    
protected:
    int numComponents();
    componentID componentIDs[8];
    entityID identity; //index in the objectTable (in engine)
    std::string name; //entity name
};

#endif