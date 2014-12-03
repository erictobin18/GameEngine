//
//  Engine.cpp
//  CppProgram
//
//  Created by                                                               Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "Engine.h"
#include "ServerGL.h"
#include "System.h"
#include "Entity.h"
#include "Terrain.h"
#include "lodepng.h"
#include <json/json.h>
#include <fstream>

Physics Engine::gamePhysics;
Graphics Engine::gameGraphics;
Logic Engine::gameLogic;
Input Engine::gameInput;

ServerGL Engine::openGLServer;

Terrain Engine::gameTerrain;

gMath::entityID Engine::cameraEntity = 0;

gMath::file readFile(std::string filename)
{
    using namespace std;
    ifstream str("Objects/" + filename + ".json");
    
    if (!str)
    {
        cout << "Failed to find file named " + filename + " in objects directory.\n";
        return gMath::file();
    }
    
    str.seekg(0,ios_base::end);
    string source = string(str.tellg(),'0');
    str.seekg(ios_base::beg);
    str.read(&source[0], source.size());
    
    Json::Value root;
    Json::Reader reader;
    
    bool parsedSuccess = reader.parse(source, root, false);
    
    if (!parsedSuccess)
        cout << "Parse failure\n";
    
    const Json::Value verts = root["vertices"];
    
    vector<gMath::vertex> vertices = *new vector<gMath::vertex>(verts.size());
    
    for (int i = 0; i < verts.size(); i++)
    {
        vertices.at(i).x = verts[i][0].asFloat();
        vertices.at(i).y = verts[i][1].asFloat();
        vertices.at(i).z = verts[i][2].asFloat();
        vertices.at(i).texX = verts[i][3].asFloat();
        vertices.at(i).texY = verts[i][4].asFloat();
    }
    
    const Json::Value ind = root["indices"];
    
    vector<unsigned int> indices = *new vector<unsigned int>(ind.size());

    for (int i = 0; i < ind.size(); i++)
    {
        indices.at(i) = ind[i].asUInt();
    }
    
    vector<unsigned char>texture;
    
    unsigned int texWidth, texHeight;
    
    unsigned error = lodepng::decode(texture, texWidth, texHeight, "Objects/" + filename + ".png");
    
    if (error)
        cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
     
    gMath::mesh m = {vertices, indices, texture, texWidth, texHeight};
     
    gMath::file f = {m};
    return f;
}

Engine::Engine()
{
    std::vector<Entity> objectTable;
}

void Engine::init()
{
    System::setGameEngine(this);
    ServerGL::setGameEngine(this);
    gameTerrain.init();
    
    Entity player(0,"Player");
    
    cameraEntity = 0;
    
    gMath::state s = (gMath::state){(gMath::vect){8,8,18.5},(gMath::vect){0,0,0},(gMath::quaternion){1,0,0,0},(gMath::vect){0,0,0}};
    
    gMath::componentID physComp = gamePhysics.newComponent(0, s);
    gameInput.newComponent(0);
    
    gamePhysics.getComponent(physComp)->rollEnabled = false;
    gamePhysics.getComponent(physComp)->gravityEnabled = true;
    gamePhysics.getComponent(physComp)->terrainEnabled = true;
    
    player.addPhysicsComponent(physComp);
    
    objectTable.push_back(player);
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    (gamePhysics.getComponent(getPhysicsComponent(0)))->setAlpha((gMath::vect){0,0,0}); //some state gets set in this call
    
    while (openGLServer.windowOpen) //animation loop
    {
        openGLServer.prepareForDrawing(); //clears display, checks if window should close
        float dt = glfwGetTime() - time;
        time = glfwGetTime();
        
        gameInput.update(dt);
        
        gamePhysics.update(dt);
        gameLogic.update(dt);
        
        gameTerrain.draw();
        
        gameGraphics.update(dt); //draws
        openGLServer.draw();//Graphics must be last call in animation loop
    } //while
    std::cout << "Execution Terminated\n"; //Finish
}
gMath::entityID Engine::createObject()
{
    return createObject("rotatingCube");
}

gMath::entityID Engine::createObject(std::string filename, gMath::state s)
{
    gMath::entityID eid = static_cast<gMath::entityID>(objectTable.size());
    Entity obj(eid, filename + std::to_string(eid));
    
    gMath::file f = readFile(filename);
    
    gMath::componentID physComp = gamePhysics.newComponent(eid, s);
    obj.addPhysicsComponent(physComp);
    
    gMath::componentID graphComp = gameGraphics.newComponent(eid, f.m);
    obj.addGraphicsComponent(graphComp);
    
    gMath::componentID logComp = gameLogic.newComponent(eid);
    obj.addLogicComponent(logComp);
    
    objectTable.push_back(obj);
    
    return eid;
}

gMath::entityID Engine::createObject(std::string filename)
{
    gMath::state s = (gMath::state){(gMath::vect){0,0,0} , (gMath::vect){1,0,0} , (gMath::quaternion){1.0, 0.0, 0.0, 0.0} , (gMath::vect){0,0.5,0.25}};
    return createObject(filename, s);
}


gMath::componentID Engine::getPhysicsComponent(gMath::entityID eid)
{
    return objectTable.at(eid).getComponentID(gMath::physicsType);
}
gMath::componentID Engine::getGraphicsComponent(gMath::entityID eid)
{
    return objectTable.at(eid).getComponentID(gMath::graphicsType);
}
gMath::componentID Engine::getLogicComponent(gMath::entityID eid)
{
    return objectTable.at(eid).getComponentID(gMath::logicType);
}