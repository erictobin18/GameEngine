//
//  Engine.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
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

using namespace std;

Physics Engine::gamePhysics;
Graphics Engine::gameGraphics;
Logic Engine::gameLogic;

ServerGL Engine::openGLServer;

Terrain Engine::gameTerrain;

Entity Engine::player(0,"Player");

file readFile(string filename)
{
    ifstream str("Objects/" + filename + ".json");
    
    if (!str)
    {
        cout << "Failed to find file named " + filename + " in objects directory.\n";
        return file();
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
    
    vector<vertex> vertices = *new vector<vertex>(verts.size());
    
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
     
    mesh m = {vertices, indices, texture, texWidth, texHeight};
     
    file f = {m};
    return f;
}

Engine::Engine()
{
    vector<Entity> objectTable;
    cout << "Added player to newly created objectTable with size " << objectTable.size() << '\n';
}

void Engine::init()
{
    System::setGameEngine(this);
    ServerGL::setGameEngine(this);
    gameTerrain.init();
    cout << "1objectTable size is " << objectTable.size() << '\n'; //returns 1
    createCamera();
    cout << "2objectTable size is " << objectTable.size() << '\n'; //returns 1
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    (gamePhysics.getComponent(getPhysicsComponent(0)))->setAlpha((vect){0,0,0}); //some state gets set in this call
    
    while (openGLServer.windowOpen) //animation loop
    {
        openGLServer.prepareForDrawing(); //clears display, checks if window should closes
        float dt = glfwGetTime() - time;
        time = glfwGetTime();
        
        //vect newAlpha = (vect){(std::rand()%9-4)/20.0f,(std::rand()%9-4)/20.0f,(std::rand()%9-4)/20.0f};
        
        //cout << objectTable.size() - 1 << '\n';
        
        //(gamePhysics.getComponent(getPhysicsComponent(0)))->setAlpha(newAlpha);
        
        gamePhysics.update(dt);
        gameLogic.update(dt);
        
        gameTerrain.draw();
        
        gameGraphics.update(dt); //draws
        openGLServer.draw();//Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
entityID Engine::createObject()
{
    return createObject("rotatingCube");
}

entityID Engine::createCamera()
{
    state s = (state){(vect){0,-2,0},(vect){0,0,0},(quaternion){1,-0.785398f,0,0},(vect){0.2,0,0}};
    
    componentID physComp = gamePhysics.newComponent(0, s);
    cout << "Created Physics Component for Player\n";
    
    player.addPhysicsComponent(physComp);
    cout << "Added Player Physics Component to Player\n";
    
    cout << "3objectTable size is " << objectTable.size() << '\n'; //returns 0
    objectTable.push_back(player);
    
    return 0;
}

entityID Engine::createObject(string filename, state s)
{
    cout << "5objectTable size is " << objectTable.size() << '\n';
    entityID eid = static_cast<entityID>(objectTable.size());
    cout << "Adding new Entity to objectTable. EntityID is " << eid << '\n';
    Entity obj(eid, filename + to_string(eid));
    cout << "Created Entity of type " << filename << '\n';
    
    file f = readFile(filename);
    
    componentID physComp = gamePhysics.newComponent(eid, s);
    cout << "Added new PhysicsComponent to gamePhysics with ID " << physComp << '\n';
    obj.addPhysicsComponent(physComp);
    cout << "Added PhysicsComponent with ID " << physComp << " to Entity " << &obj << " with entityID " << eid << '\n';
    
    componentID graphComp = gameGraphics.newComponent(eid, f.m);
    cout << "Added new GraphicsComponent to gameGraphics with ID " << graphComp << '\n';
    obj.addGraphicsComponent(graphComp);
    cout << "Added GraphicsComponent with ID " << graphComp << " to Entity " << &obj << " with entityID " << eid << '\n';
    
    componentID logComp = gameLogic.newComponent(eid);
    cout << "Added new LogicComponent to gameGraphics with ID " << logComp << '\n';
    obj.addLogicComponent(logComp);
    cout << "Added LogicComponent with ID " << logComp << " to Entity " << &obj << " with entityID " << eid << '\n';
    
    cout << "4objectTable size is " << objectTable.size() << '\n';
    
    objectTable.push_back(obj);
    
    cout << "Added Entity " << &obj << " to objectTable with size " << objectTable.size() << '\n';
    return eid;
}

entityID Engine::createObject(string filename)
{
    state s = (state){(vect){0,0,0} , (vect){1,0,0} , (quaternion){1.0, 0.0, 0.0, 0.0} , (vect){0,0.5,0.25}};
    return createObject(filename, s);
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