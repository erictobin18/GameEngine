//
//  System.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__System__
#define __CppProgram__System__

#include <iostream>
#include <vector>
#ifndef Included_Component_H
#define Included_Component_H
#include "Component.h"
#endif

#endif /* defined(__CppProgram__System__) */

using namespace std;

class System
{
public:
    System()
    {
        
    }
    // All systems must update each game loop
    virtual void update( float dt )=0;
    
    // It's good practice to separate the construction and initialization code.
    virtual void init( void )=0;
    
    // This recieves any messages sent to the core engine in Engine.cpp
    //virtual void SendMessage( /*message *msg */ )=0;
    
    virtual unsigned int newComponent();
    virtual void removeComponent(unsigned int componentID);
    
    virtual ~System()
    {
        
    }
    
protected:
    vector<Component> components;
};

class Physics : System
{
public:
    Physics() : System()
    {
        
    }
    void update( float dt);
    void init();
    unsigned int newComponent();
    void removeComponent(unsigned int componentID);
    ~Physics();
protected:
    vector<PhysicsComponent> components;
};

class Graphics : System
{
public:
    Graphics() : System()
    {
        
    }
    void update( float dt);
    void init();
    unsigned int newComponent();
    void removeComponent(unsigned int componentID);
    ~Graphics();
protected:
    vector<GraphicsComponent> components;
};

class GameLogic : System
{
public:
    GameLogic() : System()
    {
        
    }
    void update( float dt);
    void init();
    unsigned int newComponent();
    void removeComponent(unsigned int componentID);
    ~GameLogic();
protected:
    vector<GameLogicComponent> components;
};
