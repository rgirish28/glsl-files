/*
 * shader.cpp
 * Handles compiling shaders
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#include "shader.h"
#include <string>
#include <fstream>
#include <cstring>

std::string textFileRead (const char * filename) 
{
  std::string str, ret = "" ; 
  std::ifstream in ; 
  in.open(filename) ; 
  if (in.is_open()) {
    getline (in, str) ; 
    while (in) {
      ret += str + "\n" ; 
      getline (in, str) ; 
    }
    return ret ; 
  }
  else {
    std::cerr << "Unable to Open File " << filename << std::endl; 
    throw 2 ; 
  }
}

static void validateShader(GLuint shader, const char* file = 0) 
{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;

  // Get the log associated with the shader
  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
  if (length > 0) 
    std::cout << "Shader " << shader << " (" << (file?file:"") 
      << ") compile error: " << buffer << std::endl; 
}

static void validateProgram(GLuint program) 
{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;

  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer); 
  if (length > 0) 
    std::cout << "Program " << program << " link error: " << buffer 
      << std::endl; 

  glValidateProgram(program); 
  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status); 
  if (status == GL_FALSE)
    std::cout << "Error validating shader " << program << std::endl;
}

Shader::Shader(std::string shader_name) 
{
  attribute_v_coord = -1;
  attribute_v_normal = -1;
  name = shader_name;
  shader_id = glCreateProgram();
}

Shader::Shader(std::string shader_name,
    std::string vertex_filename,
    std::string fragment_filename)
{
  attribute_v_coord = -1;
  attribute_v_normal = -1;
  name = shader_name;
  shader_id = glCreateProgram();
  initShader(GL_VERTEX_SHADER, vertex_filename.c_str());
  initShader(GL_FRAGMENT_SHADER, fragment_filename.c_str());
  initProgram();
}

void Shader::initShader(unsigned int type, const char * file) 
{
  unsigned int shader = glCreateShader(type);
  std::string str = textFileRead(file);
  const char * cstr = str.c_str();
  glShaderSource(shader, 1, &cstr, NULL);
  glCompileShader(shader);
  validateShader(shader, file);
  shaders.push_back(shader);
}

void Shader::initProgram()
{
  std::cout << "Initializing: " << name << std::endl;
  for (std::vector<unsigned int>::iterator it = shaders.begin();
      it != shaders.end(); ++it)
  {
    glAttachShader(shader_id, *it);
  }

  glLinkProgram(shader_id);
  validateProgram(shader_id);

  std::vector<std::string> attrib_names;

  attrib_names.push_back("");

  const char* attribute_name;

  attribute_name = "vp_loc";
  attribute_v_coord = glGetAttribLocation(shader_id, attribute_name);

  attribute_name = "vp_norm";
  attribute_v_normal = glGetAttribLocation(shader_id, attribute_name);

  attribute_name = "vp_texcoord";
  attribute_v_texcoord = glGetAttribLocation(shader_id, attribute_name);

  // Would normally have error checking like the following
  // However, not all shaders use all of these attributes
  /*
  if (attribute_v_texcoord == - 1)
  {
    std::cerr << "Could not bind attribute " << attribute_name << std::endl;
  }
  */
}

Shader::~Shader() 
{
  for (std::vector<unsigned int>::iterator it = shaders.begin();
      it != shaders.end(); ++it)
  {
    glDetachShader(shader_id, *it);
    glDeleteShader(*it);
  }

  glDeleteProgram(shader_id);
}


unsigned int Shader::id() 
{
  return shader_id; 
}

void Shader::bind() 
{ 
  glUseProgram(shader_id);
}

void Shader::unbind() 
{
  glUseProgram(0);
}
