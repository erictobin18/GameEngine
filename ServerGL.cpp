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

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#define BUFFER_OFFSET(offset) ((void *)(offset))
#define RESTART_CHAR 0xFFFFFFFF

using namespace std;
ServerGL *ServerGL::graphicsServer = new ServerGL();

int x = 0;

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
    
    //VERTEX COORDS
    
    glGenBuffers(2, &buffer[0]); //I realize this is the same as glGenBuffers(1, buffer). Maybe I just like it this way alright
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

    glBufferData(GL_ARRAY_BUFFER, m.vertices.size()*sizeof(vertex), m.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    
    //TEX COORDS
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
    
    //INDICES
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size()*sizeof(unsigned int), m.indices.data(), GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //--------------------------------------------------TEXTURE-----------------------------------------------------------------
  
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m.texWidth,m.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m.texture.data());
    
    cout << m.texWidth; cout << m.texHeight;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glBindVertexArray(0);
}
void GraphicsObject::draw(vect pos, quaternion o)
{
    glBindVertexArray(vertexArrayObject);
    
    GLfloat matrix[4][4] = {
        {o.s*o.s + o.i*o.i - o.j*o.j - o.k*o.k, 2*o.i*o.j - 2*o.s*o.k, 2*o.i*o.k + 2*o.s*o.j, 0},
        {2*o.i*o.j + 2*o.s*o.k, o.s*o.s - o.i*o.i + o.j*o.j - o.k*o.k, 2*o.j*o.k - 2*o.s*o.i, 0},
        {2*o.i*o.k - 2*o.s*o.j, 2*o.j*o.k + 2*o.s*o.i, o.s*o.s - o.i*o.i - o.j*o.j + o.k*o.k, 0},
        {0.0,                   0.0,                   0.0,                                   1.0}
    };
    
    glVertexAttrib4fv(2, matrix[0]);
    glVertexAttrib4fv(3, matrix[1]);
    glVertexAttrib4fv(4, matrix[2]);
    glVertexAttrib4fv(5, matrix[3]);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
        
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    ServerGL::graphicsServer->reportGLError();
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
    window = glfwCreateWindow(600, 600, "THE WINDOW", NULL, NULL);
    
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
    //glFrontFace(GL_CW);
    
    glDepthMask(GL_TRUE);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_CHAR);
    
    const GLubyte *message;
    message = glGetString(GL_VERSION);
    
    if (verbose)
        cout << "OpenGL Version:\n" << message << '\n';
    
    reportGLError();
    
    Shader sh = Shader("Shader");
    
    glUseProgram(sh.getProgramObject());
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