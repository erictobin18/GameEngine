//
//  Shader.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef __CppProgram__Shader__
#define __CppProgram__Shader__

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>


#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED //NOTE!!! GLFW INCLUDES GL.h, but GL 3.2+ functionality requires gl3. This is related to how GL versions are different on OSX. Ignoring this for now...GLFW should detect this. All other GL headers must be BEFORE GLFW.
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#endif /* defined(__CppProgram__Shader__) */

using namespace std;

class Shader
{
public:
    Shader(string filename) : filename(filename)
    {
        
    }
    
    GLuint getProgramObject();
protected:
    void init();
    GLuint loadShader(GLenum theShaderType, string theShader, GLint *theShaderCompiled);
    void linkProgram(GLuint program, GLint *theProgramLinked);
    
    string fragmentShaderSource;		// the GLSL source for the fragment Shader
    string vertexShaderSource;			// the GLSL source for the vertex Shader
    GLuint program;
    bool verbose;
    string filename;
};