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

GLuint ServerGL::normalProgram = 0;
GLuint ServerGL::terrainProgram = 0;

GLfloat ServerGL::glTransform[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

Engine *ServerGL::gameEngine;

void errorCallback(int error, const char *description)
{
    std::cout << "RUNTIME ERROR " << error;
    fputs(description, stderr);
    std::cout << '\n';
} //errorCallback



ServerGL::ServerGL(): windowOpen(false), verbose(false)
{
    std::time_t currentTime;
    std::time(&currentTime);
    std::srand((int)currentTime);
    
    glfwSetErrorCallback(errorCallback);
    if (verbose)
        std::cout << "Initiating GLFW\n";
    if (!glfwInit()) //GLFW Initiate
    {
        std::cout << "Failed to Initiate GLFW\n";
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (verbose)
        std::cout << "Initiating Window\n";
    window = glfwCreateWindow(600, 600, "THE WINDOW", NULL, NULL);
    
    if (!window) //Window creation
    {
        glfwTerminate();
        std::cout << "Failed to create window\n";
    } //if
    windowOpen = true;
    
    if (verbose)
        std::cout << "Initiating OpenGL\n";
    glfwMakeContextCurrent(window);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 300, 300);
    
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //sets clear color
    glClearDepth(0.0f);
    
    
    glEnable(GL_CULL_FACE); //<-----------------------------------<<<<<<<<<<<<<<<<<<<<<<<<<<    ***CULL FACE IS HERE***
    //glFrontFace(GL_CCW);
    //lCullFace(GL_BACK);
    
    glEnable(GL_DEPTH_TEST);
    
    glDepthMask(GL_TRUE);
    
    glDepthFunc(GL_GREATER);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_CHAR);
    
    const GLubyte *message;
    message = glGetString(GL_VERSION);
    
    if (verbose)
        std::cout << "OpenGL Version:\n" << message << '\n';
    
    gMath::reportGLError();
    
    Shader shn = Shader("EntityShader");
    
    normalProgram = shn.getProgramObject();
    
    Shader sht = Shader("Terrain");
    
    terrainProgram = sht.getProgramObject();
    glUseProgram(normalProgram);
    if (verbose)
        std::cout << "Running...\n";
}

ServerGL::~ServerGL()
{
    glfwDestroyWindow(window);
    windowOpen = false;
    
    gMath::reportGLError();
    if (verbose)
        std::cout << "GL Server Halting\n"; //Finish
    glfwTerminate();
    
}

void ServerGL::prepareForDrawing()
{
    gMath::vect posC = (gameEngine->gamePhysics).getComponent(gameEngine->getPhysicsComponent(gameEngine->cameraEntity))->getState()->pos;
    gMath::quaternion oC =(gameEngine->gamePhysics).getComponent(gameEngine->getPhysicsComponent(gameEngine->cameraEntity))->getState()->orientation;
    
    //PhysicsComponent *inComp =(gameEngine->gamePhysics).getComponent(gameEngine->getPhysicsComponent(gameEngine->cameraEntity));
    
    //std::cout << "Orientation: " << oC.s << '\t' << oC.i << '\t' << oC.j << '\t' << oC.k << '\n';
    
    //VIEW MATRIX
    
    GLfloat znear, zfar, width, height;
    
    width = height = .173;
    
    znear = .1f;
    
    zfar = 100.0;
    
    GLfloat viewMatrix[4][4] =
    {
        {
            -2*oC.i*oC.j + 2*oC.k*oC.s,
        oC.i*oC.i - oC.j*oC.j + oC.k*oC.k - oC.s*oC.s,
            -2*oC.j*oC.k - 2*oC.i*oC.s,
            -posC.y*oC.i*oC.i + 2*posC.x*oC.i*oC.j + posC.y*oC.j*oC.j + 2*posC.z*oC.j*oC.k - posC.y*oC.k*oC.k + 2*posC.z*oC.i*oC.s - 2*posC.x*oC.k*oC.s + posC.y*oC.s*oC.s
        },
        {
            2*oC.i*oC.k + 2*oC.j*oC.s,
            2*oC.j*oC.k - 2*oC.i*oC.s,
            -oC.i*oC.i - oC.j*oC.j + oC.k*oC.k + oC.s*oC.s,
            posC.z*oC.i*oC.i + posC.z*oC.j*oC.j - 2*posC.x*oC.i*oC.k - 2*posC.y*oC.j*oC.k - posC.z*oC.k*oC.k + 2*posC.y*oC.i*oC.s - 2*posC.x*oC.j*oC.s - posC.z*oC.s*oC.s
        },
        {
            oC.i*oC.i - oC.j*oC.j - oC.k*oC.k + oC.s*oC.s,
            2*oC.i*oC.j + 2*oC.k*oC.s,
            2*oC.i*oC.k - 2*oC.j*oC.s,
            -posC.x*oC.i*oC.i - 2*posC.y*oC.i*oC.j + posC.x*oC.j*oC.j - 2*posC.z*oC.i*oC.k + posC.x*oC.k*oC.k + 2*posC.z*oC.j*oC.s - 2*posC.y*oC.k*oC.s - posC.x*oC.s*oC.s
        },
        {
            0,
            0,
            0,
            1
        }
    };
    
    //PERSPECTIVE MATRIX
    //THE UNIT QUATERNION ORIENTS THE CAMERA IN THE +x DIRECTION
    //ROTATIONS ARE DONE AROUND WORLD AXES
    //FOR ALT/AZIMUTH: 1. ROTATE AROUND Z 2. COMPUTE EYE +x DIRECTION 3. ROTATE AROUND EYE +x DIRECTION
    
    GLfloat perspectiveMatrix[4][4] =
    {
        {
            (2 * znear)/width,
            0,
            0,
            0
        },
        {
            0,
            (2 * znear)/height,
            0,
            0
        },
        {
            0,
            0,
            (-zfar - znear)/(zfar - znear),
            (2 * zfar * znear)/(zfar - znear)
        },
        {
            0,
            0,
            1,
            0
        }
    };
    
    gMath::matrixMultiply(glTransform, perspectiveMatrix, viewMatrix);
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    windowOpen = !glfwWindowShouldClose(window);
}

void ServerGL::draw()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ServerGL::setGameEngine(Engine *gEngine)
{
    gameEngine = gEngine;
}

GraphicsObject::GraphicsObject()
{
    vertexArrayObject = 0;
    textureID = 0;
}

GraphicsObject::~GraphicsObject()
{
    glDeleteBuffers(2, buffers);
}

GraphicsObject::GraphicsObject(gMath::mesh m)
{
    //--------------------------------------------------VERTEX-----------------------------------------------------------------
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    //VERTEX COORDS
    
    glGenBuffers(2, &buffers[0]); //I realize this is the same as glGenBuffers(1, buffer). Maybe I just like it this way alright
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size()*sizeof(gMath::vertex), m.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gMath::vertex), BUFFER_OFFSET(0));
    
    //TEX COORDS
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gMath::vertex), BUFFER_OFFSET(12));
    
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
    
    gMath::reportGLError();
}



void GraphicsObject::draw(gMath::vect pos, gMath::quaternion o)
{
    if (vertexArrayObject == 0)
    {
        std::cout << "WARNING: Tried to draw a graphics object with no VAO. Probably means the object was not properly initialized.";
    }
    
    glBindVertexArray(vertexArrayObject);
    
    //MODEL TRANSFORM MATRIX
    
    GLfloat modelTransform[4][4] = {
        {o.s*o.s + o.i*o.i - o.j*o.j - o.k*o.k, 2*o.i*o.j - 2*o.s*o.k, 2*o.i*o.k + 2*o.s*o.j, pos.x},
        {2*o.i*o.j + 2*o.s*o.k, o.s*o.s - o.i*o.i + o.j*o.j - o.k*o.k, 2*o.j*o.k - 2*o.s*o.i, pos.y},
        {2*o.i*o.k - 2*o.s*o.j, 2*o.j*o.k + 2*o.s*o.i, o.s*o.s - o.i*o.i - o.j*o.j + o.k*o.k, pos.z},
        {                    0,                     0,              0,                          1.0}
    };
    
    //TOTAL TRANSFORM
    //TRANSFORM = GLTRANSFORM * MODELTRANSFORM
    
    GLfloat matrix[4][4];
    
    gMath::matrixMultiply(matrix, ServerGL::glTransform, modelTransform);

    GLfloat matTrans[4][4];
    
    gMath::matrixTranspose(matTrans, matrix);
    
    glVertexAttrib4fv(2, matTrans[0]);
    glVertexAttrib4fv(3, matTrans[1]);
    glVertexAttrib4fv(4, matTrans[2]);
    glVertexAttrib4fv(5, matTrans[3]);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
    gMath::reportGLError();
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

GLubyte Chunk::getBlock(unsigned char x, unsigned char y, unsigned char z)
{
    //std::cout << "Hello\n\n";
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
    
    //std::cout << (int)blocks[1][1][0] << "\n\n\n";
    
    for (unsigned char x = 0; x < CHUNK_SIZE; x++)
    {
        for (unsigned char y = 0; y < CHUNK_SIZE; y++)
        {
            for (unsigned char z = 0; z < CHUNK_SIZE; z++)
            {
                if (!blocks[x][y][z]) continue;
                
                //std::cout << "x: " << x << " y: " << y << " z: " << z << " i: " << i << '\n';
                
                /*
                
                for (int j = i; j < 24; j++)
                {
                    vertices[j] = gMath::vec4(0, 0, 0, 0);
                    i++;
                }
                 
                */
               //  /*
                vertices[i++] = gMath::vec4r(x    , y    , z    , blocks[x][y][z]);  //endian-ness
                vertices[i++] = gMath::vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z    , blocks[x][y][z]);
                
                vertices[i++] = gMath::vec4r(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z    , blocks[x][y][z]);
                
              // */
                
                vertices[i++] = gMath::vec4r(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z    , blocks[x][y][z]);
                
                
                
                vertices[i++] = gMath::vec4r(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = gMath::vec4r(x    , y    , z + 1, blocks[x][y][z]);
                 
                
            }
        }
    }
    numElements = i;
    
    gMath::reportGLError();
    
    
    glBindVertexArray(vertexArrayObject);
    
    gMath::reportGLError();
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    //for (int b = 0; b < numElements; b++)
    //{
        //std::std::cout << (vertices[b] >> 24) << '\n';
    //}
    
    gMath::reportGLError();
    
    glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    
    gMath::reportGLError();
    
}

void Chunk::needsUpdate()
{
    modified = true;
}

void Chunk::init()
{
    modified = true;
    numElements = 0;
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
    
    for (unsigned char x = 0; x < CHUNK_SIZE; x++)
    {
        //std::cout << (int)x << '\n';
        for (unsigned char y = 0; y < CHUNK_SIZE; y++)
        {
            //unsigned char tempHeight = (unsigned char)(8*(sin(M_PI*2*(x + y + 5)/(24.0))+1) + 1);
            //unsigned char tempHeight = 16;
            unsigned char tempHeight = 14 + std::rand()%3;
            if (tempHeight > 16)
            {
                tempHeight = 16;
            }
            for (unsigned char z = 0; z < tempHeight; z++)
            {
                std::rand();
                this->setBlock(x, y, z, std::rand()%256 );
            }
            for (unsigned char z = tempHeight; z < CHUNK_SIZE; z++)
            {
                this->setBlock(x, y, z, 0);
            }
        }
    }
    
}

void Chunk::draw()
{
    
    glUseProgram(ServerGL::terrainProgram);
    glBindVertexArray(vertexArrayObject);
    
    gMath::reportGLError();
    
    if (modified) update();
    
    if (!numElements) return;
    
    gMath::reportGLError();
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    
    gMath::reportGLError();
    
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    gMath::reportGLError();
    
    //MODEL
    
    GLfloat modelTransform[4][4] = {
        {1,0,0,chunkX*16.0f},
        {0,1,0,chunkY*16.0f},
        {0,0,1,chunkZ*16.0f},
        {0,0,0,1}
    };

    //TOTAL TRANSFORM
    
    GLfloat matrix[4][4];
    gMath::matrixMultiply(matrix, ServerGL::glTransform, modelTransform);
    
    GLfloat matTrans[4][4];
    
    gMath::matrixTranspose(matTrans, matrix);
    
    glVertexAttrib4fv(2, matTrans[0]);
    glVertexAttrib4fv(3, matTrans[1]);
    glVertexAttrib4fv(4, matTrans[2]);
    glVertexAttrib4fv(5, matTrans[3]);
    
    gMath::reportGLError();
    
    glEnableVertexAttribArray(0);
    
    gMath::reportGLError();
    
    glDrawArrays(GL_TRIANGLES, 0, numElements);
    
    gMath::reportGLError();
    
    glBindVertexArray(0);
    glUseProgram(ServerGL::normalProgram);
    gMath::reportGLError();
}

