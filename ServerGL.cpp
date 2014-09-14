//
//  ServerGL.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SERVER_GL_H
#define SERVER_GL_H
#include "ServerGL.h"
#endif

#ifndef SHADER_H
#define SHADER_H
#include "Shader.h"
#endif

using namespace std;
ServerGL *ServerGL::graphicsServer = new ServerGL();

void errorCallback(int error, const char *description)
{
    cout << "RUNTIME ERROR " << error;
    fputs(description, stderr);
    cout << '\n';
} //errorCallback


ServerGL::~ServerGL()
{
    glfwDestroyWindow(window);
    windowOpen = false;
    
    reportGLError();
    if (verbose)
        cout << "GL Server Halting\n"; //Finish
    glfwTerminate();

}
void ServerGL::serverInit()
{
    glfwSetErrorCallback(errorCallback);
    if (verbose)
        cout << "Initiating GLFW\n";
    if (!glfwInit()) //GLFW Initiate
    {
        cout << "Failed to Initiate GLFW\n";
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (verbose)
        cout << "Initiating Window\n";
    window = glfwCreateWindow(640, 400, "THE WINDOW", NULL, NULL);
    
    if (!window) //Window creation
    {
        glfwTerminate();
        cout << "Failed to create window\n";
    } //if
    windowOpen = true;
    
    if (verbose)
        cout << "Initiating OpenGL\n";
    glfwMakeContextCurrent(window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //sets clear color
    glClearDepth(0.0f);
    
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    
    glDepthMask(GL_TRUE);
    
    const GLubyte *message;
    message = glGetString(GL_VERSION);
    
    if (verbose)
        cout << "OpenGL Version:\n" << message << '\n';
    
    reportGLError();
    
    Shader sh = Shader("Shader");
    
    if (verbose)
        cout << "Running...\n";
}
    
void ServerGL::mainLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //reportError();
    
    
    windowOpen = !glfwWindowShouldClose(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ServerGL::reportGLError()
{
    switch (glGetError())
    {
        case 0x0500:
            cout << "GL_INVALID_ENUM​\n";
            
            break;
        case 0x0501:
            cout << "GL_INVALID_VALUE​\n";
            
            break;
        case 0x0502:
            cout << "GL_INVALID_OPERATION​\n";
            
            break;
        case 0x0503:
            cout << "GL_STACK_OVERFLOW\n​​";
            
            break;
        case 0x0504:
            cout << "GL_STACK_UNDERFLOW\n​";
            
            break;
        case 0x0505:
            cout << "GL_OUT_OF_MEMORY​​\n";
            
            break;
        case 0x0506:
            cout << "GL_INVALID_FRAMEBUFFER_OPERATION​​\n";
            
            break;
            
        default:
            //if (_verbose)
            //{
            //    cout <<"No Error");
            //}
            break;
            
    } //switch
} //reportGLError