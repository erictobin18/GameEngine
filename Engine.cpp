//
//  Engine.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

#ifndef SERVERGL_H
#define SERVERGL_H
#include "ServerGL.h"
#endif

using namespace std;
Engine *Engine::gameEngine = new Engine();

void Engine::mainloop()
{
    time = glfwGetTime();
    
    ServerGL server = ServerGL::ServerGL();
    server.serverInit();
    
    init();
    
    while (server.windowOpen) //animation loop
    {
        double dt = glfwGetTime() - time;
        time = glfwGetTime();
        Physics::gamePhysics->update(dt);
        Logic::gameLogic->update(dt);
        Graphics::gameGraphics->update(dt);
        server.mainLoop(); //Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}

void Engine::init()
{
    objectTable = vector<GameObject>(1, GameObject(0));
    objectTable.push_back(GameObject(1));
    
    createRotatingCube(0);
    createRotatingCube(1);
}

void Engine::createRotatingCube(entityID eid)
{
    GameObject obj = objectTable.at(eid);
    
    obj.addComponent(physicsType);
    obj.addComponent(graphicsType);
    obj.addComponent(logicType);
    
    obj.name = string("cube:") + to_string(eid);
}

componentID Engine::newPhysicsComponent(entityID eid)
{
    return Physics::gamePhysics->newComponent(eid);
}
componentID Engine::newGraphicsComponent(entityID eid)
{
    return Graphics::gameGraphics->newComponent(eid);
}
componentID Engine::newLogicComponent(entityID eid)
{
    return Logic::gameLogic->newComponent(eid);
}