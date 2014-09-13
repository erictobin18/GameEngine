//
//  Engine.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Engine.h"


void Engine::mainloop()
{
    Engine gameEngine = Engine();
    
    ServerGL server = ServerGL::ServerGL();
    server.serverInit();
    
    
    while (server.windowOpen) //animation loop
    {
        update();
        server.mainLoop(); //Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
void Engine::update()
{
    
}

void Engine::init()
{
    physics = Physics();
    graphics = Graphics();
    gameLogic = GameLogic();
    
    
    vector<GameObject> temp(2);
    objectTable = temp;
    
    objectTable.at(0).addComponent(physicsType,0);
    objectTable.at(1).addComponent(physicsType,1);
    objectTable.at(0).addComponent(graphicsType,0);
    objectTable.at(1).addComponent(graphicsType,1);
    objectTable.at(0).addComponent(gamelogicType,0);
    objectTable.at(1).addComponent(gamelogicType,1);
    
    
    objectTable.at(0).name = "cube1";
    objectTable.at(1).name = "cube2";
    
    objectTable.at(0).identity = 0;
    objectTable.at(1).identity = 1;
    
    
}