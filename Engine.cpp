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
#include "lodepng.h"
#include <json/json.h>
#include <fstream>

using namespace std;

Physics Engine::gamePhysics;
Graphics Engine::gameGraphics;
Logic Engine::gameLogic;

ServerGL Engine::openGLServer;

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
    System::setGameEngine(this);
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    while (openGLServer.windowOpen) //animation loop
    {
        openGLServer.prepareForDrawing(); //clears display, checks if window should closes
        float dt = glfwGetTime() - time;
        time = glfwGetTime();
        gamePhysics.update(dt);
        gameLogic.update(dt);
        gameGraphics.update(dt); //draws
        openGLServer.draw();//Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
void Engine::createObject()
{
    createObject("rotatingCube");
    
}

void Engine::createObject(string filename, state s)
{
    entityID eid = static_cast<entityID>(objectTable.size());
    Entity obj(eid, filename + to_string(eid));
    
    
    file f = readFile(filename);
    
    componentID physComp = gamePhysics.newComponent(eid, s);
    obj.addPhysicsComponent(physComp);
    
    componentID graphComp = gameGraphics.newComponent(eid, f.m);
    obj.addGraphicsComponent(graphComp);
    
    componentID logComp = gameLogic.newComponent(eid);
    obj.addLogicComponent(logComp);
    
    objectTable.push_back(obj);

}

void Engine::createObject(string filename)
{
    state s = (state){(vect){0,0,0} , (vect){0,0,0} , (quaternion){1.0, 0.0, 0.0, 0.0} , (vect){0,0.5,0.25}};
    createObject(filename, s);
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