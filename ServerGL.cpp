//
//  ServerGL.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/11/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "ServerGL.h"
#include "Shader.h"
#include <math.h>

#define BUFFER_OFFSET(offset) ((void *)(offset))
#define RESTART_CHAR 0xFFFFFFFF

using namespace std;

GLuint ServerGL::normalProgram = 0;
GLuint ServerGL::terrainProgram = 0;

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
    
    
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    
    glDepthMask(GL_TRUE);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_CHAR);
    
    const GLubyte *message;
    message = glGetString(GL_VERSION);
    
    if (verbose)
        cout << "OpenGL Version:\n" << message << '\n';
    
    reportGLError();
    
    Shader shn = Shader("Shader");
    
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
void GraphicsObject::draw(vect pos, quaternion o)
{
    if (vertexArrayObject == 0)
    {
        cout << "WARNING: Tried to draw a graphics object with no VAO. Probably means the object was not properly initialized.";
    }
    
    glBindVertexArray(vertexArrayObject);
    
    GLfloat matrix[4][4] = {
        {o.s*o.s + o.i*o.i - o.j*o.j - o.k*o.k, 2*o.i*o.j - 2*o.s*o.k, 2*o.i*o.k + 2*o.s*o.j, 0},
        {2*o.i*o.j + 2*o.s*o.k, o.s*o.s - o.i*o.i + o.j*o.j - o.k*o.k, 2*o.j*o.k - 2*o.s*o.i, 0},
        {2*o.i*o.k - 2*o.s*o.j, 2*o.j*o.k + 2*o.s*o.i, o.s*o.s - o.i*o.i - o.j*o.j + o.k*o.k, 0},
        {0.0,                   0.0,                   0.0,                                   1.0}
    };
    
    /*
    GLfloat matrix[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
    }; */
    glVertexAttrib4fv(2, matrix[0]);
    glVertexAttrib4fv(3, matrix[1]);
    glVertexAttrib4fv(4, matrix[2]);
    glVertexAttrib4fv(5, matrix[3]);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glBindVertexArray(0);
    ServerGL::reportGLError();
}

unsigned int vec4(unsigned char x, unsigned char y, unsigned char z, unsigned char val)
{
    return (x << 24) + (y << 16) + (z << 8) + val;
}

Chunk::Chunk()
{
    modified = true;
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &bufferID);
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
    
    unsigned int vertices[16*16*16*6*6];
    int i = 0;
    
    for (unsigned char x = 0; x < 16; x++)
    {
        for (unsigned char y = 0; y < 16; y++)
        {
            for (unsigned char z = 0; z < 16; z++)
            {
                if (!blocks[x][y][z]) continue;
                
                vertices[i++] = vec4(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z + 1, blocks[x][y][z]);
                
                //std::cout << vec4(x, y, z, blocks[x][y][z]) << '\n';
                
                vertices[i++] = vec4(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z    , blocks[x][y][z]);
                
                vertices[i++] = vec4(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z + 1, blocks[x][y][z]);
                
                vertices[i++] = vec4(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z    , blocks[x][y][z]);
                
                vertices[i++] = vec4(x    , y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z    , blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z    , blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y + 1, z    , blocks[x][y][z]);
                
                vertices[i++] = vec4(x + 1, y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x + 1, y    , z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y + 1, z + 1, blocks[x][y][z]);
                vertices[i++] = vec4(x    , y    , z + 1, blocks[x][y][z]);
            }
        }
    }
    
    std::cout << vertices[1] << '\n';
    
    numElements = i;
    
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    for (int b = 0; b < numElements; b++)
    {
        //std::cout << (vertices[b] >> 24) << '\n';
    }
    
    glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    
}

void Chunk::draw()
{
    
    //glUseProgram(ServerGL::normalProgram);
    glBindVertexArray(vertexArrayObject);
    
    if (modified) update();
    
    if (!numElements) return;
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    
    glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    
    
    GLfloat matrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    
    
    glVertexAttrib4fv(2, matrix[0]);
    glVertexAttrib4fv(3, matrix[1]);
    glVertexAttrib4fv(4, matrix[2]);
    glVertexAttrib4fv(5, matrix[3]);
    
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLES, 0, numElements);
    
    glBindVertexArray(0);
    //glUseProgram(ServerGL::normalProgram);
    ServerGL::reportGLError();
}

