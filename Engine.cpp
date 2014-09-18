//
//  Engine.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef ENGINE_H
#define ENGINE_H
#include "Engine.h"
#endif

#ifndef SERVER_GL_H
#define SERVER_GL_H
#include "ServerGL.h"
#endif

#ifndef JSONCPP_H
#define JSONCPP_H
#include <json/json.h>
#endif

#ifndef FSTREAM_H
#define FSTREAM_H
#include <fstream>
#endif



using namespace std;
Engine *Engine::gameEngine = new Engine();

file readFile(string filename)
{
    ofstream blah("hooplah.obj");
    blah.close();
    
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
    
    // Let's extract the array contained
    // in the root object
    
    /*
    const Json::Value array = root["array"];
    
    // Iterate over sequence elements and
    // print its values
    for(unsigned int index=0; index<array.size();
        ++index)
    {
        cout<<"Element "
        <<index
        <<" in array: "
        <<array[index].asString()
        <<endl;
    }
     */
    
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
    
    /*
    vector<unsigned char> texture {
        255,000,000,    255,127,000,    255,255,000,    127,255,000, // tex coords 0.0, 0.0 (left) to 1.0, 0.0 (right)
        255,000,127,    063,063,063,    127,127,127,    000,255,000,
        255,000,255,    190,190,190,    255,255,255,    000,255,127,
        127,000,255,    000,000,255,    000,127,255,    000,255,255, //tex coords 0.0, 1.0 (left) to 1.0, 1.0 (right)
    };
    unsigned int texWidth = 4;
    unsigned int texHeight = 4;
    
     */
     
    mesh m = {vertices, indices, texture, texWidth, texHeight};
    

     
    file f = {m};
    return f;
}

Engine::Engine()
{
    objectTable = *new vector<Entity>;
}

void Engine::mainloop()
{
    time = glfwGetTime();
    
    while (ServerGL::graphicsServer->windowOpen) //animation loop
    {
        ServerGL::graphicsServer->prepareForDrawing(); //clears display, checks if window should closes
        float dt = glfwGetTime() - time;
        time = glfwGetTime();
        Physics::gamePhysics->update(dt);
        Logic::gameLogic->update(dt);
        Graphics::gameGraphics->update(dt); //draws
        ServerGL::graphicsServer->draw();//Graphics must be last call in animation loop
    } //while
    cout << "Execution Terminated\n"; //Finish
}
void Engine::createObject()
{
    createObject("rotatingCube");
}

void Engine::createObject(string filename)
{
    entityID eid = static_cast<entityID>(objectTable.size());
    Entity obj = *new Entity(eid, filename + to_string(eid));
    
    
    file f = readFile(filename);
    
    vect pos = {0.0,0.0,0.0};
    vect vel = {0.0,0.0,1.0};
    quaternion orientation = {1.0, 0.0, 0.0, 0.0};
    vect omega = {1.0, 1.0, 0.0};
    
    state s = {pos,vel,orientation,omega};

    
    componentID physComp = Physics::gamePhysics->newComponent(eid, s);
    obj.addPhysicsComponent(physComp);
    
    componentID graphComp = Graphics::gameGraphics->newComponent(eid, f.m);
    obj.addGraphicsComponent(graphComp);
    
    componentID logComp = Logic::gameLogic->newComponent(eid);
    obj.addLogicComponent(logComp);
    
    objectTable.push_back(obj);
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