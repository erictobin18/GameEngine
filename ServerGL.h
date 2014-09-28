//
//  ServerGL.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _SERVER_GL_H_
#define _SERVER_GL_H_

#include <iostream>
#include "GlobalConstants.h"
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED //NOTE!!! GLFW INCLUDES GL.h, but GL 3.2+ functionality requires gl3. This is related to how GL versions are different on OSX. Ignoring this for now...GLFW should detect this. All other GL headers must be BEFORE GLFW.
#include <GLFW/glfw3.h>

class ServerGL
{
public:
    ServerGL();
    ~ServerGL();
    
    void prepareForDrawing();
    void draw();
    bool windowOpen;
    
    static ServerGL *graphicsServer;
    
    void reportGLError();
    
protected:
    GLFWwindow *window;
    bool verbose;
};

class GraphicsObject
{
public:
    GraphicsObject(mesh m);
    GraphicsObject()
    {
        GraphicsObject(*new mesh);
    }
    void draw(vect position, quaternion orientation);
    

protected:
    GLuint vertexArrayObject;
    GLuint textureID;
    GLuint numIndices;
    
    };

#endif