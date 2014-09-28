//
//  System.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _SYSTEM_H_
#define _SYSTEM_H_


#include <iostream>
#include "GlobalConstants.h"
#include "Component.h"
#include "Engine.h"

class PhysicsComponent;
class GraphicsComponent;
class LogicComponent;

class System
{
public:
    System();
    // All systems must update each game loop
    virtual void update( float dt )=0;
    
    // This recieves any messages sent to the core engine in Engine.cpp
    //virtual void SendMessage( /*message *msg */ )=0;
    
    componentID newComponent(entityID eid);
    void removeComponent(componentID cid);
    
    static void setGameEngine(Engine *gEngine);
    
    static Engine *gameEngine;
        
    virtual ~System()
    {
        
    }
    
protected:
    std::vector<Component> components;
};

class Physics : System
{
public:
    Physics();
    void update( float dt);
    PhysicsComponent getComponent(componentID cid);
    
    componentID newComponent(entityID eid);
    componentID newComponent(entityID eid, state s);
    
    ~Physics();
    //Physics& operator=(Physics other);
protected:
    std::vector<PhysicsComponent> components;
};

class Graphics : System
{
public:
    Graphics();
    void update( float dt);
    GraphicsComponent getComponent(componentID cid);
    
    componentID newComponent(entityID eid);
    componentID newComponent(entityID eid, mesh m);
    
    ~Graphics();
    //Graphics& operator=(Graphics other);
protected:
    std::vector<GraphicsComponent> components;
};

class Logic : System
{
public:
    Logic();
    void update( float dt);
    LogicComponent getComponent(componentID cid);
    
    componentID newComponent(entityID eid);
    
    ~Logic();
    //Logic& operator=(Logic other);
protected:
    std::vector<LogicComponent> components;
};

#endif
