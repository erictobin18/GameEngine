//
//  Engine.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef ENGINE_H
#define ENGINE_H
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

#ifndef ENTITY_H
#define ENTITY_H
#include "Entity.h"
#endif

#ifndef SYSTEM_H
#define SYSTEM_H
#include "System.h"
#endif


class Physics;
class Graphics;
class Logic;

class Engine
{
public:
    Engine();
    void mainloop();
    double time;
    void createObject();
    void createObject(std::string filename);
    
    componentID getPhysicsComponent(entityID eid);
    componentID getGraphicsComponent(entityID eid);
    componentID getLogicComponent(entityID eid);
    
    static Engine *gameEngine;
    
protected:
    std::vector<Entity> objectTable;
};