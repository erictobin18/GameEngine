//
//  Entity.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__Entity__
#define __CppProgram__Entity__

#include <iostream>
#ifndef Included_Component_H
#define Included_Component_H
#include "Component.h"
#endif

#endif /* defined(__CppProgram__Entity__) */

typedef unsigned char bit_field;

using namespace std;

class Entity
{
public:
    virtual void addComponent(bit_field, unsigned int componentID)=0;
    virtual void removeComponent(bit_field)=0;
    virtual bool hasComponent(bit_field)=0;
    virtual unsigned int getComponentID(bit_field)=0;
    
protected:
    virtual int numComponents()=0;
};