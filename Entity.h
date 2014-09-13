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
    Entity(entityID eid)
    {
        
    }
    virtual void addComponent(bit_field)=0;
    virtual void removeComponent(bit_field)=0;
    virtual bool hasComponent(bit_field)=0;
    virtual unsigned int getComponentID(bit_field)=0;
    
protected:
    virtual int numComponents()=0;
};