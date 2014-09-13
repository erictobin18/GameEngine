//
//  System.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

#ifndef COMPONENT_H
#define COMPONENT_H
#include "Component.h"
#endif

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

class PhysicsComponent;
class GraphicsComponent;
class LogicComponent;

class System
{
public:
    System()
    {
        
    }
    // All systems must update each game loop
    virtual void update( double dt )=0;
    
    // It's good practice to separate the construction and initialization code.
    virtual void init( void )=0;
    
    // This recieves any messages sent to the core engine in Engine.cpp
    //virtual void SendMessage( /*message *msg */ )=0;
    
    componentID newComponent(entityID eid);
    void removeComponent(componentID cid);
        
    virtual ~System()
    {
        
    }
    
protected:
    std::vector<Component> components;
};

class Physics : System
{
public:
    Physics() : System()
    {
        
    }
    void update( double dt);
    void init();
    componentID newComponent(entityID eid);
    componentID newComponent(entityID eid, state s);
    static Physics *gamePhysics;
    
    ~Physics();
    Physics& operator=(Physics other);
protected:
    std::vector<PhysicsComponent> components;
};

class Graphics : System
{
public:
    Graphics() : System()
    {
        
    }
    void update( double dt);
    void init();
    componentID newComponent(entityID eid);
    static Graphics *gameGraphics;
    
    ~Graphics();
    Graphics& operator=(Graphics other);
protected:
    std::vector<GraphicsComponent> components;
};

class Logic : System
{
public:
    Logic() : System()
    {
        
    }
    
    void update( double dt);
    void init();
    componentID newComponent(entityID eid);
    static Logic *gameLogic;
    
    ~Logic();
    Logic& operator=(Logic other);
protected:
    std::vector<LogicComponent> components;
};
