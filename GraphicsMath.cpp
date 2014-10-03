//
//  GraphicsMath.cpp
//  CppProgram
//
//  Created by Eric Tobin on 10/3/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#include "GraphicsMath.h"

gMath::quaternion gMath::hMultiply(gMath::quaternion left, gMath::quaternion right)
{
    return (gMath::quaternion)
    {
        left.s*right.s - left.i*right.i - left.j*right.j - left.k*right.k,
        left.s*right.i + left.i*right.s + left.j*right.k - left.k*right.j,
        left.s*right.j - left.i*right.k + left.j*right.s + left.j*right.i,
        left.s*right.k + left.i*right.j - left.j*right.i + left.k*right.s
    };
}

gMath::vect gMath::addVect(gMath::vect a, gMath::vect b)
{
    return (gMath::vect){a.x + b.x, a.y + b.y, a.z + b.z};
}

gMath::vect gMath::scalarMultiply(gMath::vect v, float s)
{
    return (gMath::vect){v.x*s,v.y*s,v.z*s};
}

gMath::quaternion gMath::scalarMultiply(gMath::quaternion q, float n)
{
    return (gMath::quaternion){q.s*n,q.i*n,q.j*n,q.k*n};
}

gMath::vect gMath::im(gMath::quaternion q)
{
    return (gMath::vect){q.i,q.j,q.k};
}

gMath::quaternion gMath::padVector(gMath::vect v, float p)
{
    return (gMath::quaternion){p,v.x,v.y,v.z};
}

float gMath::magnitude(quaternion q)
{
    return sqrt(q.s * q.s + q.i * q.i + q.j * q.j + q.k * q.k);
}

void gMath::reportGLError()
    {
        switch (glGetError())
        {
            case 0x0500:
                std::cout << "GL_INVALID_ENUM​\n";
                
                break;
            case 0x0501:
                std::cout << "GL_INVALID_VALUE​\n";
                
                break;
            case 0x0502:
                std::cout << "GL_INVALID_OPERATION​\n";
                
                break;
            case 0x0503:
                std::cout << "GL_STACK_OVERFLOW\n​​";
                
                break;
            case 0x0504:
                std::cout << "GL_STACK_UNDERFLOW\n​";
                
                break;
            case 0x0505:
                std::cout << "GL_OUT_OF_MEMORY​​\n";
                
                break;
            case 0x0506:
                std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION​​\n";
                
                break;
                
            default:
                //if (_verbose)
                //{
                //    cout <<"No Error");
                //}
                break;
                
        } //switch
    } //reportGLError


void gMath::matrixMultiply(GLfloat matOut[4][4], GLfloat matLeft[4][4], GLfloat matRight[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matOut[i][j] = matLeft[i][0]*matRight[0][j] + matLeft[i][1]*matRight[1][j] + matLeft[i][2]*matRight[2][j] + matLeft[i][3]*matRight[3][j];
        }
    }
}

        
void gMath::printMatrix(GLfloat matrix[4][4])
{
    std::cout << "Printing Matrix: \n";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << '\t';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void gMath::matrixTranspose(GLfloat matOut[4][4], GLfloat theMatrix[4][4])
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matOut[j][i] = theMatrix[i][j];
            }
        }
    }
    
unsigned int gMath::vec4(unsigned char x, unsigned char y, unsigned char z, unsigned char val)
    {
        return ((unsigned int)x << 24) + ((unsigned int)y << 16) + ((unsigned int)z << 8) + (unsigned int)val;
    }
    
unsigned int gMath::vec4r(unsigned char x, unsigned char y, unsigned char z, unsigned char val)
    {
        return (val << 24) + (z << 16) + (y << 8) + x;
    }





