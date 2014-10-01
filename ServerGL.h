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

class Engine;

class ServerGL
{
public:
    ServerGL();
    ~ServerGL();
    
    void prepareForDrawing(); //called every drawing cycle BEFORE any components can draw
    void draw(); //last call to openGL in a given drawing cycle
    bool windowOpen; //true iff window is open
    static void reportGLError(); //Prints error type to cout
    static void setGameEngine(Engine *gEngine);
    
    static GLuint normalProgram;
    static GLuint terrainProgram;
    
    static Engine *gameEngine;
    
protected:
    GLFWwindow *window;
    bool verbose;
    
};

class GraphicsObject //every GraphicsComponent has a GraphicsObject
{
public:
    GraphicsObject(mesh m);
    GraphicsObject();
    ~GraphicsObject();
    
    void draw(vect position, quaternion orientation); //must be called after ServerGL::prepareForDrawing but before ServerGL::draw
    static GLubyte instance;
    
    static void matrixMultiply(GLfloat matOut[4][4], GLfloat matLeft[4][4], GLfloat matRight[4][4]);

protected:
   
    GLuint vertexArrayObject; //Every GraphicsObject has a VAO
    GLuint textureID; //Every GraphicsObject has a (not necessarily unique) texture
    GLuint buffers[2];

};

class Chunk
{
public:
    Chunk(GLint x, GLint y, GLint z);
    ~Chunk();
    
    GLuint getBlock(unsigned char x, unsigned char y, unsigned char z);
    void setBlock(unsigned char x, unsigned char y, unsigned char z, unsigned char blockID);
    
    void update();
    
    void needsUpdate();
    
    void init();
    
    void draw();
    
    
protected:
    GLint chunkX;
    GLint chunkY;
    GLint chunkZ;
    
    GLubyte blocks[16][16][16];
    GLuint vertexArrayObject;
    GLuint bufferID;
    unsigned int numElements;
    bool modified;
    
};


#endif