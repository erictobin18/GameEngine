//
//  ServerGL.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "ServerGL.h"
#include "Shader.h"
#include "Engine.h"
#include "System.h"
#include <math.h>

#define BUFFER_OFFSET(offset) ((void *)(offset))
#define RESTART_CHAR 0xFFFFFFFF

#define CHUNK_SIZE 16

using namespace std;

GLuint ServerGL::normalProgram = 0;
GLuint ServerGL::terrainProgram = 0;

Engine *ServerGL::gameEngine;

void errorCallback(int error, const char *description)
{
    cout << "RUNTIME ERROR " << error;
    fputs(description, stderr);
    cout << '\n';
} //errorCallback



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
    glClearDepth(1.0f);
    
    
    //glEnable(GL_CULL_FACE); //<-----------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<<    ***CULL FACE IS HERE***
    //glFrontFace(GL_CCW);
    //lCullFace(GL_BACK);
    
    //glEnable(GL_DEPTH_TEST);
    
    //glDepthMask(GL_TRUE);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_CHAR);
    
    const GLubyte *message;
    message = glGetString(GL_VERSION);
    
    if (verbose)
        cout << "OpenGL Version:\n" << message << '\n';
    
    reportGLError();
    
    Shader shn = Shader("EntityShader");
    
    normalProgram = shn.getProgramObject();
    
    Shader sht = Shader("Terrain");
    
    terrainProgram = sht.getProgramObject();
    glUseProgram(normalProgram);
    if (verbose)
        cout << "Running...\n";
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

void ServerGL::prepareForDrawing()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    windowOpen = !glfwWindowShouldClose(window);
}

void ServerGL::draw()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    //GraphicsObject::instance++;
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

void ServerGL::setGameEngine(Engine *gEngine)
{
    gameEngine = gEngine;
}

GLubyte GraphicsObject::instance = 0;

GraphicsObject::GraphicsObject()
{
    vertexArrayObject = 0;
    textureID = 0;
}

GraphicsObject::~GraphicsObject()
{
    glDeleteBuffers(2, buffers);
}

GraphicsObject::GraphicsObject(mesh m)
{
    //--------------------------------------------------VERTEX-----------------------------------------------------------------
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    //VERTEX COORDS
    
    glGenBuffers(2, &buffers[0]); //I realize this is the same as glGenBuffers(1, buffer). Maybe I just like it this way alright
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size()*sizeof(vertex), m.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(0));
    
    //TEX COORDS
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), BUFFER_OFFSET(12));
    
    //INDICES
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size()*sizeof(unsigned int), m.indices.data(), GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    
    //--------------------------------------------------TEXTURE-----------------------------------------------------------------
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m.texWidth,m.texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m.texture.data());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glBindVertexArray(0);
    
    ServerGL::reportGLError();
}
void GraphicsObject::matrixMultiply(GLfloat matOut[4][4], GLfloat matLeft[4][4], GLfloat matRight[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matOut[j][i] = matLeft[0][i]*matRight[j][0] + matLeft[1][i]*matRight[j][1] + matLeft[2][i]*matRight[j][2] + matLeft[3][i]*matRight[j][3];
        }
    }
}

void printMatrix(GLfloat matrix[4][4])
{
    cout << "Printing Matrix: \n";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << matrix[i][j] << '\t';
        }
        cout << '\n';
    }
    cout << '\n';
}

void GraphicsObject::draw(vect pos, quaternion o)
{
    if (vertexArrayObject == 0)
    {
        cout << "WARNING: Tried to draw a graphics object with no VAO. Probably means the object was not properly initialized.";
    }
    
    glBindVertexArray(vertexArrayObject);
    
    //MODEL TRANSFORM MATRIX
    
    GLfloat modelTransform[4][4] = {
        {o.s*o.s + o.i*o.i - o.j*o.j - o.k*o.k, 2*o.i*o.j + 2*o.s*o.k, 2*o.i*o.k - 2*o.s*o.j, 0},
        {2*o.i*o.j - 2*o.s*o.k, o.s*o.s - o.i*o.i + o.j*o.j - o.k*o.k, 2*o.j*o.k + 2*o.s*o.i, 0},
        {2*o.i*o.k + 2*o.s*o.j, 2*o.j*o.k - 2*o.s*o.i, o.s*o.s - o.i*o.i - o.j*o.j + o.k*o.k, 0},
        {pos.x,                  pos.y,                  pos.z,                             1.0}
    }; //matrix should be correct now. Obtained by taking the transpose (OPENGL IS COLUMN MAJOR, C++ IS ROW MAJOR) of the appropriate model transform
    
    
    vect posP = (ServerGL::gameEngine->gamePhysics).getComponent((ServerGL::gameEngine->player).getComponentID(physicsType))->getState()->pos;
    quaternion oP =(ServerGL::gameEngine->gamePhysics).getComponent((ServerGL::gameEngine->player).getComponentID(physicsType))->getState()->orientation;
    
    //VIEW TRANSFORM MATRIX
    
    GLfloat viewTransform[4][4] = {
        {oP.s*oP.s + oP.i*oP.i - oP.j*oP.j - oP.k*oP.k, 2*oP.i*oP.j + 2*oP.s*oP.k, -2*oP.i*oP.k + 2*oP.s*oP.j, 0},
        {2*oP.i*oP.j - 2*oP.s*oP.k, oP.s*oP.s - oP.i*oP.i + oP.j*oP.j - oP.k*oP.k, -2*oP.j*oP.k - 2*oP.s*oP.i, 0},
        {2*oP.i*oP.k + 2*oP.s*oP.j, 2*oP.j*oP.k - 2*oP.s*oP.i, -oP.s*oP.s + oP.i*oP.i + oP.j*oP.j - oP.k*oP.k, 0},
        {0,                      0,                         0,                                              1.0}
    };
    
    
    viewTransform[3][0] = -viewTransform[0][0]*posP.x - viewTransform[0][1]*posP.y - viewTransform[0][2]*posP.z;
    viewTransform[3][1] = -viewTransform[1][0]*posP.x - viewTransform[1][1]*posP.y - viewTransform[1][2]*posP.z;
    viewTransform[3][2] = viewTransform[2][0]*posP.x + viewTransform[2][1]*posP.y + viewTransform[2][2]*posP.z;
    
    //matrix should be correct now. Obtained by taking the transpose (OPENGL IS COLUMN MAJOR, C++ IS ROW MAJOR) of the matrix product of a reflection about the z axis on the left (to change to left-handed coordinates) and the appropriate view transform
    
    
    
    //MODELVIEW MATRIX = VIEW * MODEL
    
    GLfloat modelView[4][4];
    
    matrixMultiply(modelView, viewTransform, modelTransform);
    
    //PERSPECTIVE MATRIX
    
    GLfloat znear, zfar, width, height;
    
    width = height = .173;
    
    znear = .1f;
    
    zfar = 10.0;
    
    GLfloat perspective[4][4] = {
        {2*znear/width,              0,                             0,   0},
        {            0, 2*znear/height,                             0,   0},
        {            0,              0,-(zfar + znear)/(zfar - znear),-1.0},
        {            0,              0,   2*zfar*znear/(zfar - znear),   0}
    };
    
    //TOTAL TRANSFORM
    //TRANSFORM = PERSPECTIVE * MODELVIEW
    
    GLfloat matrix[4][4];
    
    matrixMultiply(matrix, perspective, modelView);
    
    printMatrix(modelTransform);
    
    printMatrix(viewTransform);
    
    printMatrix(modelView);
    
    printMatrix(perspective);
    
    printMatrix(matrix);
    
    cout << "\n\n\n";
    
    ///*
    glVertexAttrib4fv(2, matrix[0]);
    glVertexAttrib4fv(3, matrix[1]);
    glVertexAttrib4fv(4, matrix[2]);
    glVertexAttrib4fv(5, matrix[3]);
   // */
    /*
    glVertexAttrib4fv(2, modelView[0]);
    glVertexAttrib4fv(3, modelView[1]);
    glVertexAttrib4fv(4, modelView[2]);
    glVertexAttrib4fv(5, modelView[3]);
    */
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
    ServerGL::reportGLError();
}

unsigned int vec4(unsigned char x, unsigned char y, unsigned char z, unsigned char val)
{
    return ((unsigned int)x << 24) + ((unsigned int)y << 16) + ((unsigned int)z << 8) + (unsigned int)val;
}

unsigned int vec4r(unsigned char x, unsigned char y, unsigned char z, unsigned char val)
{
    return (val << 24) + (z << 16) + (y << 8) + x;
}

Chunk::Chunk(GLint x, GLint y, GLint z):chunkX(x),chunkY(y),chunkZ(z)
{
    /*
     modified = true;
     glGenVertexArrays(1, &vertexArrayObject);
     glGenBuffers(1, &bufferID);
     */
}

Chunk::~Chunk()
{
    glDeleteBuffers(1, &bufferID);
}

GLuint Chunk::getBlock(unsigned char x, unsigned char y, unsigned char z)
{
    return blocks[x][y][z];
}

void Chunk::setBlock(unsigned char x, unsigned char y, unsigned char z, unsigned char blockID)
{
    blocks[x][y][z] = blockID;
    modified = true;
}

void Chunk::update()
{
    modified = false;
    
    unsigned int vertices[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE*6*6];
    int i = 0;
    
    
    
    for (unsigned char x = 0; x < CHUNK_SIZE; x++)
    {
        for (unsigned char y = 0; y < CHUNK_SIZE; y++)
        {
            for (unsigned char z = 0; z < CHUNK_SIZE; z++)
            {
                if (!blocks[x][y][z]) continue;
                
                
                vertices[i++] = vec4r(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                
                vertices[i++] = vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z    , blocks[x][y][z]);
                
                vertices[i++] = vec4r(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                
                vertices[i++] = vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z    , blocks[x][y][z]);
                
                vertices[i++] = vec4r(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                
                vertices[i++] = vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4r(x    , y    , z + 1, blocks[x][y][z]);
                
            }
        }
    }
    numElements = i;
    
    
    
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    for (int b = 0; b < numElements; b++)
    {
        //std::cout << (vertices[b] >> 24) << '\n';
    }
    
    glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    
}

void Chunk::needsUpdate()
{
    modified = true;
}

void Chunk::init()
{
    modified = true;
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &bufferID);
    /*if (chunkX == 0 && chunkY == 0  && chunkZ == 0 )
     {
     for (unsigned char x = 0; x < 16; x++)
     {
     for (unsigned char y = 0; y < 16; y++)
     {
     for (unsigned char z = 0; z < 16; z++)
     {
     this->setBlock(x, y, z, 0);
     }
     }
     }
     }*/
    for (unsigned char x = 0; x < 16; x++)
    {
        for (unsigned char y = 0; y < 16; y++)
        {
            for (unsigned char z = 0; z < 16; z++)
            {
                this->setBlock(x, y, z, std::rand()%256 );
            }
        }
    }
    
}

void Chunk::draw()
{
    
    glUseProgram(ServerGL::terrainProgram);
    glBindVertexArray(vertexArrayObject);
    
    if (modified) update();
    
    if (!numElements) return;
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    /*
     GLfloat matrix[4][4] = { //ISOMETRIC
     {0.707107,        0, -0.707107, 0},
     {0.408248, 0.816497,  0.408248, 0},
     { 0.57735, -0.57735,   0.57735, 0},
     {       0,        0,         0, 1}
     };
     
     */
    
    
    
    vect posP = (ServerGL::gameEngine->gamePhysics).getComponent((ServerGL::gameEngine->player).getComponentID(physicsType))->getState()->pos;
    quaternion oP =(ServerGL::gameEngine->gamePhysics).getComponent((ServerGL::gameEngine->player).getComponentID(physicsType))->getState()->orientation;
    
    
    
    GLfloat matrixR[4][4] = {
        {oP.s*oP.s + oP.i*oP.i - oP.j*oP.j - oP.k*oP.k, 2*oP.i*oP.j - 2*oP.s*oP.k, 2*oP.i*oP.k + 2*oP.s*oP.j, 0},
        {2*oP.i*oP.j + 2*oP.s*oP.k, oP.s*oP.s - oP.i*oP.i + oP.j*oP.j - oP.k*oP.k, 2*oP.j*oP.k - 2*oP.s*oP.i, 0},
        {2*oP.i*oP.k - 2*oP.s*oP.j, 2*oP.j*oP.k + 2*oP.s*oP.i, oP.s*oP.s - oP.i*oP.i - oP.j*oP.j + oP.k*oP.k, 0},
        {0,                      0,                         0,                                              1.0}
    };
    
    
    
    
    for (int i = 0; i < 3; i++)
    {
        matrixR[3][i] = matrixR[i][0]*posP.x + matrixR[i][1]*posP.y + matrixR[i][2]*posP.z;
    }
    
    
    
    //cout << x << '\t' << y << '\t' << z << '\n';
    
    GLfloat matrixL[4][4] = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {chunkX*1.0f,chunkY*1.0f,chunkZ*1.0f,1}
    };
    
    GLfloat modelViewMatrix[4][4];
    
    GraphicsObject::matrixMultiply(modelViewMatrix, matrixL, matrixR);
    
    GLfloat perspective[4][4] = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1-11.0f/9,20.0f/9},
        {0,0,-1.0f,0}
        
    };
    
    GLfloat matrix[4][4];
    
    GraphicsObject::matrixMultiply(matrix,  modelViewMatrix, perspective);
    
    
    
    glVertexAttrib4fv(2, matrix[0]);
    glVertexAttrib4fv(3, matrix[1]);
    glVertexAttrib4fv(4, matrix[2]);
    glVertexAttrib4fv(5, matrix[3]);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(6);
    
    glDrawArrays(GL_TRIANGLES, 0, numElements);
    
    glBindVertexArray(0);
    glUseProgram(ServerGL::normalProgram);
    ServerGL::reportGLError();
}

