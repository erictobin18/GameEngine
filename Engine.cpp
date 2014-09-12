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
        server.mainLoop(); //last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
void Engine::update()
{
    
}

void Engine::init()
{
    vector<GameObject> temp(2);
    objectTable = temp;
    
    objectTable.at(0).addComponent(physics,0);
    objectTable.at(1).addComponent(physics,1);
    objectTable.at(0).addComponent(graphics,0);
    objectTable.at(1).addComponent(graphics,1);
    objectTable.at(0).addComponent(gamelogic,0);
    objectTable.at(1).addComponent(gamelogic,1);
    
    
    objectTable.at(0).name = "cube1";
    objectTable.at(1).name = "cube2";
    
    objectTable.at(0).identity = 0;
    objectTable.at(1).identity = 1;
    
    
}