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

#define BUFFER_OFFSET(offset) ((void *)(offset))
#define RESTART_CHAR 0xFFFFFFFF

using namespace std;
ServerGL *ServerGL::graphicsServer = new ServerGL();

void errorCallback(int error, const char *description)
{
    cout << "RUNTIME ERROR " << error;
    fputs(description, stderr);
    cout << '\n';
} //errorCallback

GraphicsObject::GraphicsObject(mesh m)
{
    //--------------------------------------------------VERTEX-----------------------------------------------------------------
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    unsigned int buffer[2];
    
    glGenBuffers(1, &buffer[0]); //I realize this is the same as glGenBuffers(1, buffer). Maybe I just like it this way alright
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size()*sizeof(vertex), &m.vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 5*sizeof(double), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 5*sizeof(double), BUFFER_OFFSET(3));
    
    glGenBuffers(1, &buffer[1]);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
    
    numIndices = static_cast<unsigned int>(m.indices.size());
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), &m.indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    //--------------------------------------------------TEXTURE-----------------------------------------------------------------
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m.texWidth, m.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m.texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
void GraphicsObject::draw(vect position, quaternion orientation)
{
    glBindVertexArray(vertexArrayObject);
    glVertexAttrib3d(3, position.x, position.y, position.z);
    glVertexAttrib4d(4, orientation.s, orientation.i, orientation.j, orientation.k);
    
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}




ServerGL::~ServerGL()
{
    glfwDestroyWindow(window);
    windowOpen = false;
    
    reportGLError();
    if (verbose)
        cout << "GL Server Halting\n"; //Finish
    glfwTerminate();

}
ServerGL::ServerGL(): windowOpen(false), verbose(false)
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

void ServerGL::prepareForDrawing()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    windowOpen = !glfwWindowShouldClose(window);
}

void ServerGL::draw()
{
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