/*
 * shader.h
 * Handles compiling shaders
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


#ifndef __SHADER_H_
#define __SHADER_H_

class Shader {
  public:
    Shader(std::string shader_name=""); 
    Shader(std::string shader_name,
        std::string vertex_filename,
        std::string fragment_filename);
    ~Shader(); 

    // Add a shader
    void initShader(unsigned int type, const char * file);
    // Init program with shaders added so far
    void initProgram();

    // Bind/unbind GLSL Shaders
    void bind(); 
    void unbind(); 

    // Program ID
    unsigned int id(); 

    GLint attribute_v_coord;
    GLint attribute_v_normal;
    GLint attribute_v_texcoord;

    std::string name;

  private:
    std::vector<unsigned int> shaders; 
    unsigned int shader_id; 
};

#endif
