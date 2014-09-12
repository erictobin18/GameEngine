//
//  GameObject.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__GameObject__
#define __CppProgram__GameObject__

#include <iostream>
#include "Entity.h"
#ifndef Included_Component_H
#define Included_Component_H
#include "Component.h"
#endif
#include <vector>

#endif /* defined(__CppProgram__GameObject__) */

using namespace std;

class GameObject : public Entity
{
public:
    GameObject() : Entity(), components(0), name("NULL")
    {
        
    }
    
    void addComponent(bit_field, unsigned int componentID);
    void removeComponent(bit_field);
    bool hasComponent(bit_field);
    unsigned int getComponentID(bit_field);
    void init();
    
    bit_field components;
    string name;
    unsigned int identity; //index in the Master Object Table
    
protected:
    vector<Component> componentTable;
    int numComponents();
    unsigned int componentIDs[];
};