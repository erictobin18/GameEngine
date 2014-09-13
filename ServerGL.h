//
//  ServerGL.h
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SERVER_GL
#define SERVER_GL
#endif 

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
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
    ServerGL() : windowOpen(false), verbose(false)
    {
        
    }
    ~ServerGL();
    void mainLoop();
    void serverInit();
    bool windowOpen;
    
protected:
    void reportGLError();
    GLFWwindow *window;
    bool verbose;
};