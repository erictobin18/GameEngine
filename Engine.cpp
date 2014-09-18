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

file readFile(string filename)
{
    vect pos = {0.0,0.0,0.0};
    vect vel = {0.1,0.0,0.0};
    quaternion orientation = {1.0, 0.0, 0.0, 0.0};
    vect omega = {0.8, -0.8, 0.0};
    
    state s = {pos,vel,orientation,omega};
    
    vector<vertex>vertices {
        {-.5,-.5,-.5, 0.0,  0.0}, //0
        {-.5,0.5,-.5, 0.0,  .25}, //1
        {0.5,-.5,-.5, 1./3,  0.0}, //2
        {0.5,0.5,-.5, 1./3,  0.25}, //3
        {0.5,-.5,0.5, 2./3,  0.0}, //4
        {0.5,0.5,0.5, 2./3,  .25}, //5
        {-.5,-.5,0.5, 1.0,  0.0}, //6
        {-.5,0.5,0.5, 1.0,  .25}, //7
        
        {0.5,0.5,0.5, 0.0,  .25}, //5
        {0.5,0.5,-.5, 1./3,  .25}, //3
        {-.5,0.5,0.5, 0.0,  .5}, //7
        {-.5,0.5,-.5, 1./3, .5}, //1
        {-.5,-.5,0.5, 0.0,  .75}, //6
        {-.5,-.5,-.5, 1./3,  .75}, //0
        {0.5,-.5,0.5, 0.0,  1.0}, //4
        {0.5,-.5,-.5, 1./3,  1.0}, //2
        
        
        
        

        
    } ;
    vector<unsigned int>indices {
        0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,
        0xFFFFFFFF,
        0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf
    } ;
    
    vector<unsigned char> texture {
        255,000,000,    255,127,000,    255,255,000,    127,255,000, // tex coords 0.0, 0.0 (left) to 1.0, 0.0 (right)
        255,000,127,    063,063,063,    127,127,127,    000,255,000,
        255,000,255,    190,190,190,    255,255,255,    000,255,127,
        127,000,255,    000,000,255,    000,127,255,    000,255,255, //tex coords 0.0, 1.0 (left) to 1.0, 1.0 (right)
    };
    unsigned int texWidth = 3;
    unsigned int texHeight = 4;
    
    mesh m = {vertices, indices, texture, texWidth, texHeight};
    
    file f = {s,m};
    return f;
}


Engine::Engine()
{
    objectTable = *new vector<Entity>;
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    while (ServerGL::graphicsServer->windowOpen) //animation loop
    {
        ServerGL::graphicsServer->prepareForDrawing(); //clears display, checks if window should closes
        float dt = glfwGetTime() - time;
        time = glfwGetTime();
        Physics::gamePhysics->update(dt);
        Logic::gameLogic->update(dt);
        Graphics::gameGraphics->update(dt); //draws
        ServerGL::graphicsServer->draw();//Graphics must be last call in animation loop
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
    
    
    file f = readFile(filename);
    
    componentID physComp = Physics::gamePhysics->newComponent(eid, f.s);
    obj.addPhysicsComponent(physComp);
    
    componentID graphComp = Graphics::gameGraphics->newComponent(eid, f.m);
    obj.addGraphicsComponent(graphComp);
    
    componentID logComp = Logic::gameLogic->newComponent(eid);
    obj.addLogicComponent(logComp);
    
    objectTable.push_back(obj);
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