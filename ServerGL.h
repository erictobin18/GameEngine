//
//  ServerGL.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SERVER_GL_H
#define SERVER_GL_H
#endif 

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H
#include "GlobalConstants.h"
#endif

#ifndef GLFW3_H
#define GLFW3_H
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED //NOTE!!! GLFW INCLUDES GL.h, but GL 3.2+ functionality requires gl3. This is related to how GL versions are different on OSX. Ignoring this for now...GLFW should detect this. All other GL headers must be BEFORE GLFW.
#include <GLFW/glfw3.h>
#endif

class ServerGL
{
public:
    ServerGL();
    ~ServerGL();
    
    void prepareForDrawing();
    void draw();
    bool windowOpen;
    
    static ServerGL *graphicsServer;
    
protected:
    void reportGLError();
    GLFWwindow *window;
    bool verbose;
};

class GraphicsObject
{
public:
    GraphicsObject(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<rgb_value> texture, unsigned int texWidth, unsigned int texHeight);
    GraphicsObject()
    {
        GraphicsObject(*new std::vector<vertex>, *new std::vector<unsigned int>, *new std::vector<rgb_value>, 0, 0);
    }
    void draw(vect position, quaternion orientation);

protected:
    GLuint vertexArrayObject;
    GLuint textureID;
    GLuint numIndices;
    
    };