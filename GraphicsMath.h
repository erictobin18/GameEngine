//
//  GraphicsMath.h
//  CppProgram
//
//  Created by Eric Tobin on 10/3/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef _GraphicsMath_
#define _GraphicsMath_

#include <iostream>
#include <vector>

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED //NOTE!!! GLFW INCLUDES GL.h, but GL 3.2+ functionality requires gl3. This is related to how GL versions are different on OSX. Ignoring this for now...GLFW should detect this. All other GL headers must be BEFORE GLFW.
#include <GLFW/glfw3.h>

#include <math.h>

namespace gMath
{
    typedef enum componentType {noneType = 0, physicsType = 1, graphicsType = 2, physics_and_graphicsType = 3, logicType = 4, physics_and_logicType = 5, graphics_and_logicType = 6, all = 7} componentType;
    
    struct quaternion //used to compute rotations, Wikipedia has a good article
    {
        float s;
        float i;
        float j;
        float k;
    };
    
    struct vect //"vector" is already used :)
    {
        float x;
        float y;
        float z;
    };
    
    struct state //The "state" of an object consists of all its current kinematical properties
    {
        vect pos;
        vect vel;
        quaternion orientation; //using quaternions is more efficient and solves gimbal lock
        vect omega; //this is the vector indicating rotation. Its magnitude is the angular speed,
        //and its direction indicates the normal to the plane of rotation
    };
    
    struct vertex
    {
        float x;
        float y;
        float z;
        float texX;
        float texY;
    }; //used in object models. There is likely a "right" way to do this that I'm unaware of,
    //but this should be just as efficient. The first three floats are the spacial location
    //of a vertex in the model, and the last two are the coordinates in its texture the
    //vertex is associated with.
    
     struct rgb_value //kept for future use
     {
     unsigned char r;
     unsigned char g;
     unsigned char b;
     }; //stores a color in RGB space.
     
    
    struct mesh //contains all information necessary to render a model, except its state
    {
        std::vector<vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<unsigned char>texture;
        unsigned int texWidth;
        unsigned int texHeight;
    };
    
    struct file //currently, object files only contain the mesh data
    {
        mesh m;
    };
    
    typedef unsigned int componentID;
    
    typedef unsigned int entityID; //This and componentID are temporary until handles are implemented
    
    typedef unsigned char bit_field; //Used for efficient switching based on what components an entity has
    
    typedef unsigned int textureID; //values generated by OpenGL
    
    quaternion hMultiply(quaternion left, quaternion right);
    vect addVect(vect a, vect b);
    vect scalarMultiply(vect v, float s);
    quaternion scalarMultiply(quaternion q, float n);
    vect im(quaternion q);
    quaternion padVector(vect v, float p);
    float magnitude(quaternion q);
    
    void reportGLError();
    void matrixMultiply(GLfloat matOut[4][4], GLfloat matLeft[4][4], GLfloat matRight[4][4]);

    void printMatrix(GLfloat matrix[4][4]);
    void matrixTranspose(GLfloat matOut[4][4], GLfloat theMatrix[4][4]);
    unsigned int vec4(unsigned char x, unsigned char y, unsigned char z, unsigned char val);
    unsigned int vec4r(unsigned char x, unsigned char y, unsigned char z, unsigned char val);

}
#endif
