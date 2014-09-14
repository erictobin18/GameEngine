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

#ifndef SERVER_GL_H
#define SERVER_GL_H
#include "ServerGL.h"
#endif


using namespace std;
Engine *Engine::gameEngine = new Engine();


Engine::Engine()
{
    objectTable = *new vector<Entity>;
    createObject("rotatingCube");
    createObject("rotatingCube");
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    ServerGL server = ServerGL::ServerGL();
    
    while (server.windowOpen) //animation loop
    {
        server.prepareForDrawing(); //clears display, checks if window should closes
        double dt = glfwGetTime() - time;
        time = glfwGetTime();
        Physics::gamePhysics->update(dt);
        Logic::gameLogic->update(dt);
        Graphics::gameGraphics->update(dt); //draws
        server.draw();//Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
void Engine::createObject()
{
    createObject("rotatingCube");
}

void Engine::createObject(string filename)
{
    entityID eid = static_cast<entityID>(objectTable.size());
    Entity obj = *new Entity(eid, filename + to_string(eid));
    objectTable.push_back(obj);
    
    componentID physComp = Physics::gamePhysics->newComponent(eid, /**new state*/);
    obj.addPhysicsComponent(physComp);
    
    componentID graphComp = Graphics::gameGraphics->newComponent(eid);
    obj.addGraphicsComponent(graphComp);
    
    componentID logComp = Logic::gameLogic->newComponent(eid);
    obj.addLogicComponent(logComp);
    
    
    addPhysicsComponent(eid);
    addGraphicsComponent(eid);
    addLogicComponent(eid);
}

void Engine::addPhysicsComponent(entityID eid)
{
    
}
void Engine::addGraphicsComponent(entityID eid)
{
    
}
void Engine::addLogicComponent(entityID eid)
{
    
}


componentID Engine::getPhysicsComponent(entityID eid)
{
    return objectTable.at(eid).getComponentID(physicsType);
}
componentID Engine::getGraphicsComponent(entityID eid)
{
    return objectTable.at(eid).getComponentID(graphicsType);
}
componentID Engine::getLogicComponent(entityID eid)
{
    return objectTable.at(eid).getComponentID(logicType);
}