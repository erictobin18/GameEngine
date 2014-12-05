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
#include "GraphicsMath.h"
#include "Component.h" //required for component vector definitions

class Engine; //Forward declaration to keep Engine.h out of header includes

class System //Superclass to Physics, Graphics, and Logic; set up to allow easy addition of more Systems
             //and efficient looping over components for updating
{
public:
    System();
    virtual void update( float dt )=0; //All systems must update all their components every loop
    // This recieves any messages sent to the core engine in Engine.cpp
    //virtual void SendMessage( /*message *msg */ )=0;
    virtual gMath::componentID newComponent(gMath::entityID eid)=0; //creates and sets up a new Component of appropriate type
    void removeComponent(gMath::componentID cid); //removes a component from the component table
    static void setGameEngine(Engine *gEngine); //consider working this method into the constructor
    virtual ~System();
    
    static Engine *gameEngine; //pointer to the engine so the System can invoke messages, access data, etc.
    
protected:
    std::vector<Component> components; //every type of System must have a list of components to update
};

class Physics : System
{
public:
    Physics();
    void computeAlpha(PhysicsComponent *comp);
    void computeAccel(PhysicsComponent *comp);
    void update( float dt);
    PhysicsComponent *getComponent(gMath::componentID cid); //looks up a component in its table
    gMath::componentID newComponent(gMath::entityID eid);
    gMath::componentID newComponent(gMath::entityID eid, gMath::state s);
    ~Physics();
    
protected:
    int testBlock(int,int,int,gMath::vect,gMath::vect,gMath::vect,int, bool, bool, bool);
    void impact(int , bool *, bool *, bool *, gMath::state *, gMath::vect, gMath::vect);
    
    std::vector<PhysicsComponent> components; //Physics has Components of type PhysicsComponent
};

class Graphics : System
{
public:
    Graphics();
    void update( float dt);
    GraphicsComponent *getComponent(gMath::componentID cid); //looks up a component in its table
    gMath::componentID newComponent(gMath::entityID eid);
    gMath::componentID newComponent(gMath::entityID eid, gMath::mesh m);
    ~Graphics();
    
protected:
    std::vector<GraphicsComponent> components; //Graphics has Components of type GraphicsComponent
};

class Logic : System
{
public:
    Logic();
    void update( float dt);
    LogicComponent *getComponent(gMath::componentID cid); //looks up a component in its table
    gMath::componentID newComponent(gMath::entityID eid);
    ~Logic();
    
protected:
    std::vector<LogicComponent> components; //Logic has Components of type LogicComponent
};

class Input : System
{
public:
    Input();
    void update( float dt);
    InputComponent *getComponent(); //returns the Input Component
    static void mouseFunction(GLFWwindow * window, double xpos, double ypos);
    static void keyFunction(GLFWwindow * window, int key, int scancode, int action, int modifierKeys);
    gMath::componentID newComponent(gMath::entityID eid);
    
    static double altitude, azimuth;
    
    static bool isJumping;
    ~Input();
protected:
    InputComponent inComp;
    //static gMath::vect omegaUpdate;
    static char xVel;
    static char yVel;
};

#endif
