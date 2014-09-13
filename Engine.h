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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "GameObject.h"
#endif

#ifndef SYSTEM_H
#define SYSTEM_H
#include "System.h"
#endif


class ServerGL;
class Physics;
class Graphics;
class Logic;

class Engine
{
public:
    Engine()
    {
        
    }
    void mainloop();
    double time;
    
    componentID newPhysicsComponent(entityID eid);
    componentID newGraphicsComponent(entityID eid);
    componentID newLogicComponent(entityID eid);
    
    static Engine *gameEngine;
    
protected:
    void init();
    void createRotatingCube(entityID eid);
    
    //Physics *physics;
    //Graphics *graphics;
    //Logic *Logic;
    
    ServerGL *server;
    
    std::vector<GameObject> objectTable;
};