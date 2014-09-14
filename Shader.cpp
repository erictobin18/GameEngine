//
//  Shader.cpp
//  CppProgram
//
//  Created by Eric Tobin on 9/12/14.
//  Copyright (c) 2014 omnisciendus. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H
#include "Shader.h"
#endif

using namespace std;

Shader::Shader(string filename): filename(filename)
{
    bool loadedShaders = true;
    
    //Load vertex and fragment shader
    //set (vertex + fragment)ShaderSource
    ifstream source(filename + "/" + filename + ".vs");
    
    if (source)
    {
        source.seekg(ios::end);
        
        vertexShaderSource.resize(/*source.tellg()*/ 313);
        source.seekg(source.beg);
        source.read(&vertexShaderSource[0], vertexShaderSource.size());
        source.close();
    }
    else
    {
        loadedShaders = false;
        cout << "Failed to load vertex shader source file.\n";
        source.close();
    }
    
    source.open(filename + "/" + filename + ".fs");
    if (source)
    {
        source.seekg(ios::end);
        fragmentShaderSource.resize(/*source.tellg()*/ 229);
        source.seekg(ios::beg);
        source.read(&fragmentShaderSource[0], fragmentShaderSource.size());
        source.close();
    }
    else
    {
        loadedShaders = false;
        cout << "Failed to load fragment shader source file.\n";
        source.close();
    }
    
    
    
    //compile shaders
    GLint compiled = 0;
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource, &compiled);
    loadedShaders = loadedShaders && compiled;
    
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource, &compiled);
	loadedShaders = loadedShaders && compiled;
    
	// Create a program object and link both shaders
	
	program = glCreateProgram();
	
	glAttachShader(program, vertexShader);
	glDeleteShader(vertexShader);   // Release
	
	glAttachShader(program, fragmentShader);
	glDeleteShader(fragmentShader); // Release
    
    GLint linked = 0;
    linkProgram(program, &linked);
    loadedShaders = loadedShaders && linked;
    
    
    if ( !loadedShaders )
        cout << ">> WARNING: Failed to load GLSL \"" << filename << "\" fragment & vertex shaders!\n";
    

}

GLuint Shader::loadShader(GLenum theShaderType, string theShader, GLint *theShaderCompiled)
{
    GLuint shaderObject = 0;
    const char *c_string = theShader.c_str();
    
    if( c_string != NULL )
    {
        GLint infoLogLength = 0;
        
        shaderObject = glCreateShader(theShaderType);
        
        glShaderSource(shaderObject, 1, &c_string, NULL);
        glCompileShader(shaderObject);
        
        glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);
        
        if( infoLogLength > 0 )
        {
            GLchar *infoLog = (GLchar *)malloc(infoLogLength);
            
            if( infoLog != NULL )
            {
                glGetShaderInfoLog(shaderObject,
                                   infoLogLength,
                                   &infoLogLength,
                                   infoLog);
                
                cout << ">> Shader compile log:\n" << infoLog << '\n';
                
                free(infoLog);
            } // if
        } // if
        
        glGetShaderiv(shaderObject,GL_COMPILE_STATUS,theShaderCompiled);
        
        if( *theShaderCompiled == 0 )
        {
            cout << ">> Failed to compile shader " << theShader << '\n';
        } // if
    } // if
    else
    {
        *theShaderCompiled = 1;
    } // else
    
    return shaderObject;
}

void Shader::linkProgram(GLuint program, GLint *theProgramLinked)
{
    GLint  infoLogLength = 0;
    
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if( infoLogLength >  0 )
    {
        GLchar *infoLog = (GLchar *)malloc(infoLogLength);
        
        if( infoLog != NULL)
        {
            glGetProgramInfoLog(program,
                                infoLogLength,
                                &infoLogLength,
                                infoLog);
            
            cout << ">> Program link log:\n" << infoLog << '\n';
            
            free(infoLog);
        } // if
    } // if
    
    glGetProgramiv(program, GL_LINK_STATUS, theProgramLinked);
    
    if( *theProgramLinked == 0 )
    {
        cout << ">> Failed to link program 0x" << &program << '\n';
    } // if
}

GLuint Shader::getProgramObject()
{
    return program;
}

