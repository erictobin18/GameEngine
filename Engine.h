//
//  Engine.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include "GlobalConstants.h"
#include "Entity.h"
#include "System.h"
#include "lodepng.h"

class Physics;
class Graphics;
class Logic;

class Engine
{
public:
    Engine();
    void mainloop();
    float time;
    int num;
    void createObject();
    void createObject(std::string filename);
    void createObject(std::string filename, state s);
    double getTime();
    
    componentID getPhysicsComponent(entityID eid);
    componentID getGraphicsComponent(entityID eid);
    componentID getLogicComponent(entityID eid);
    
    static Engine *gameEngine;
    
protected:
    std::vector<Entity> objectTable;
};

#endif