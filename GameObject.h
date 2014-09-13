//
//  GameObject.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

class GameObject : public Entity
{
public:
    GameObject(entityID eid) : Entity(eid), identity(eid),components(0), name("NULL")
    {
        init();
    }
    
    void addComponent(bit_field type);
    void removeComponent(bit_field type);
    bool hasComponent(bit_field type);
    componentID getComponentID(bit_field type);
    void init();
    
    bit_field components;
    std::string name;
    
protected:
    int numComponents();
    componentID componentIDs[8];
    entityID identity; //index in the Master Object Table
};