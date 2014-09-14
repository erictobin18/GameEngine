//
//  Shader.h
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H
#endif

#ifndef GLFW3_H
#define GLFW3_H
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>


#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED //NOTE!!! GLFW INCLUDES GL.h, but GL 3.2+ functionality requires gl3. This is related to how GL versions are different on OSX. Ignoring this for now...GLFW should detect this. All other GL headers must be BEFORE GLFW.
#include <GLFW/glfw3.h>
#endif

#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <iostream>
#endif

#ifndef FSTREAM_H
#define FSTREAM_H
#include <fstream>
#endif


class Shader
{
public:
    Shader(std::string filename);
    
    GLuint getProgramObject();
protected:
    void init();
    GLuint loadShader(GLenum theShaderType, std::string theShader, GLint *theShaderCompiled);
    void linkProgram(GLuint program, GLint *theProgramLinked);
    
    std::string fragmentShaderSource;		// the GLSL source for the fragment Shader
    std::string vertexShaderSource;			// the GLSL source for the vertex Shader
    GLuint program;
    bool verbose;
    std::string filename;
};