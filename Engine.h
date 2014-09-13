//
//  Engine.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__Engine__
#define __CppProgram__Engine__

#include <iostream>
#include "GameObject.h"
#include "System.h"
#ifndef Included_Component_H
#define Included_Component_H
#include "Component.h"
#endif
#include "ServerGL.h"
#include <vector>

#endif /* defined(__CppProgram__Engine__) */


using namespace std;

class Engine
{
public:
    void mainloop();
protected:
    void update();
    void init();
    
    Physics physics;
    Graphics graphics;
    GameLogic gameLogic;
    
    ServerGL server;
    
    vector<GameObject> objectTable;
};