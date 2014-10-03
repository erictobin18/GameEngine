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
#include "GraphicsMath.h"
#include "Entity.h" //required for objectTable declaration

class Physics;
class Graphics;
class Logic;
class ServerGL;
class Terrain; //forward declaration of classes to keep System.h and ServerGL.h out of header file includes

class Engine
{
public:
    Engine();
    
    void init();
    
    void mainloop(); //contains infinite while loop that runs game
    gMath::entityID createObject(); //adds a new entity to the objectTable
    gMath::entityID createObject(std::string filename);
    gMath::entityID createObject(std::string filename, gMath::state s);
    
    gMath::entityID createCamera();
    double getTime(); //wraps the GLFW command
    
    gMath::componentID getPhysicsComponent(gMath::entityID eid); //looks up component associated with an entity in the objectTable
    gMath::componentID getGraphicsComponent(gMath::entityID eid);
    gMath::componentID getLogicComponent(gMath::entityID eid);
    
    static Physics gamePhysics; //Physics system
    static Graphics gameGraphics; //Graphics system
    static Logic gameLogic; //Logic system
    static ServerGL openGLServer; //GL interface class
    static Terrain gameTerrain;
    
    static Entity player;
protected:
    float time; //elapsed time, seconds
    std::vector<Entity> objectTable; //vector of all entities in the Engine
};

#endif