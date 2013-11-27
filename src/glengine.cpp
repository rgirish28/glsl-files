/*
 * glengine.cpp
 * The point at which we start calling OpenGL things. We draw, and initialize
 * here.
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#define _USE_MATH_DEFINES
#include "glengine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>
#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>

GLEngine::GLEngine()
{
}

GLEngine::~GLEngine()
{
}


void GLEngine::loadShaders()
{
  // Add a new shader like this:
  /*
     shaders.push_back(new Shader("Shader Name",
     "path/to/vertex/shader",
     "path/to/fragment/shader"));
  */

  // Simple shader, constant color
  shaders.push_back(new Shader("Simple",
        "shaders/simple/simple.vert.glsl", 
        "shaders/simple/simple.frag.glsl"));
  // (We'll also use this to render text)
  font_shader = shaders.back();

  // Shader displaying the XYZ of normals as RGB
  // This should not appear to rotate
  shaders.push_back(new Shader("Normals",
        "shaders/normal/normal.vert.glsl", 
        "shaders/normal/normal.frag.glsl"));

  // Simple texture - display the color given by a texture
  shaders.push_back(new Shader("Simple Texture",
        "shaders/texture/texture.vert.glsl", 
        "shaders/texture/texture.frag.glsl"));

  // Using Blinn-Phong in Gouraud Shader (Vertex Shader)
  shaders.push_back(new Shader("Blinn-Phong (Gouraud)",
        "shaders/blinn_gouraud/blinn.gouraud.vert.glsl", 
        "shaders/blinn_gouraud/blinn.gouraud.frag.glsl"));

  // Using Blinn-Phong in Phong Shading (Fragment Shader)
  shaders.push_back(new Shader("Blinn-Phong (Phong)",
        "shaders/blinn/blinn.vert.glsl", 
        "shaders/blinn/blinn.frag.glsl"));

  // Simple Toon shading
  shaders.push_back(new Shader("Toon",
        "shaders/toon/toon.vert.glsl", 
        "shaders/toon/toon.frag.glsl"));

  // Bump mapped Blinn-Phong
  shaders.push_back(new Shader("Bump, Blinn",
        "shaders/bump_blinn/bump.blinn.vert.glsl", 
        "shaders/bump_blinn/bump.blinn.frag.glsl"));

  // Bump mapped textured Blinn-Phong
  // Using multiple textures
  shaders.push_back(new Shader("Bump, Textured, Blinn",
        "shaders/bump_texture_blinn/bump.texture.blinn.vert.glsl", 
        "shaders/bump_texture_blinn/bump.texture.blinn.frag.glsl"));

  std::cout << shaders.size() << " shaders loaded!" << std::endl;
}

void GLEngine::setupScene()
{
  //Set up our shaders!
  loadShaders();

  // Load textures
  // As an example, we are using the following:
  // Texture 0: Color
  // Texture 1: Normal/Bump Map
  loadTexture("data/face.jpg");
  loadTexture("data/brick_normal_map.jpg");

  // Create the vertices, normals, texture coords, and elements of our sphere
  sph.create(20, 20);

  // Initialize some default values
  scene_scale = 1.;
  animating = true;
  time_cur = glutGet(GLUT_ELAPSED_TIME)/1000.;
  time_prev = glutGet(GLUT_ELAPSED_TIME)/1000.;
  time = 0.;

  // OpenGL Settings
  glClearColor(0.4f, 0.6f, 0.9f, 0.0f); 
  glEnable(GL_DEPTH_TEST);
}


void GLEngine::display()
{
  // Clear Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

  // Update Time
  time_prev = time_cur;
  time_cur = glutGet(GLUT_ELAPSED_TIME);
  if (animating)
  {
    // Time is given in seconds
    time += (time_cur-time_prev)/1000.;
  }

  const float rot_radius = 2.f;
  view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f))
    * glm::scale(glm::mat4(1.f), glm::vec3(scene_scale));

  // Create one sphere per shader, and divide a circle equally to show
  // all the shaders.
  int num_shaders = shaders.size();
  float phase_delta = M_PI * 2.f/num_shaders;

  Shader * cur_shader;

  for (int i = 0; i < num_shaders; ++i)
  {
    // Update the Model Matrix to move the sphere to its correct location
    glm::mat4 rot_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(rot_radius*cos(time + i * phase_delta),
          rot_radius*sin(time + i * phase_delta),0));
    model_matrix = rot_matrix
      * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f))
      * glm::rotate(glm::mat4(1.f), 100.f*time, glm::vec3(0,1,0));
    normal_matrix = glm::transpose(glm::inverse(view_matrix*model_matrix));
    
    // Grab the current shader 
    cur_shader = shaders[i];
    // Enable the shader
    cur_shader->bind();
    // Send the shader our variables
    loadUniforms(cur_shader);
    // Draw the sphere
    sph.draw(cur_shader);

    // Draw the text label for this sphere (semi-hacky)
    font_shader->bind();
    model_matrix = glm::translate(glm::mat4(1.f), 
        glm::vec3(-0.7f/scene_scale,-0.8f,0)) 
      * model_matrix;
    loadUniforms(font_shader);
    glRasterPos2f(0,0);

    // Center the text with some padding
    const int max_strlen = 25;
    int to_show = std::min(max_strlen, (int)cur_shader->name.size());
    int padding = (max_strlen-to_show)/2;
    for (int si = 0; si < padding; ++si)
    {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    }
    for (int si = 0 ; si < to_show; ++si)
    {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 
          cur_shader->name[si]);
    }
  }
  glutSwapBuffers();
}


void GLEngine::loadUniforms(Shader * cur_shader)
{
  // Push variables onto the shader
  // Grab the "location" of a variable on the shader
  int projectionMatrixLocation = glGetUniformLocation(cur_shader->id(), 
      "projection_matrix");
  int viewMatrixLocation = glGetUniformLocation(cur_shader->id(), 
      "view_matrix");
  int modelMatrixLocation = glGetUniformLocation(cur_shader->id(), 
      "model_matrix");
  int normalMatrixLocation = glGetUniformLocation(cur_shader->id(), 
      "normal_matrix");
  // Set the value of the variable
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, 
      &projection_matrix[0][0]);
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &view_matrix[0][0]);  
  glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model_matrix[0][0]);  
  glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, &normal_matrix[0][0]);

  // Repeat...
  int ambient_loc = glGetUniformLocation(cur_shader->id(), 
      "ambient");
  int diffuse_loc = glGetUniformLocation(cur_shader->id(), 
      "diffuse");
  int specular_loc = glGetUniformLocation(cur_shader->id(), 
      "specular");
  int shininess_loc = glGetUniformLocation(cur_shader->id(), 
      "shininess");
  glUniform3f(ambient_loc, 0.1f,0.2f,0.1f);
  glUniform3f(diffuse_loc, 0.f,0.6f,0.3f);
  glUniform3f(specular_loc, 0.5f, 0.5f, 0.5f);
  glUniform1f(shininess_loc, 20);

  int light_pos_loc = glGetUniformLocation(cur_shader->id(), 
      "light_pos");
  int light_color_loc = glGetUniformLocation(cur_shader->id(), 
      "light_color");
  glm::vec4 light_pos_h = view_matrix * glm::vec4(0.f,0.f,1.f,1.f);
  glm::vec3 light_pos = glm::vec3(light_pos_h.x, light_pos_h.y, light_pos_h.z)
    / light_pos_h.w;
  glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_pos));
  glUniform3f(light_color_loc, 1,1,1);

  int texmap_loc = glGetUniformLocation(cur_shader->id(), 
      "texture_map");
  int bumpmap_loc = glGetUniformLocation(cur_shader->id(), 
      "bump_map");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glUniform1i(texmap_loc, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glUniform1i(bumpmap_loc, 1);

  int window_size_loc = glGetUniformLocation(cur_shader->id(),
      "window_size");
  glUniform2f(window_size_loc, window_width, window_height);

}


void GLEngine::reshape(int w, int h)
{
  window_width = w; 
  window_height = h;

  projection_matrix = glm::perspective(60.0f, 
     1.f, 0.1f, 100.f);  
  //projection_matrix = glm::ortho(-3.f, 3.f, -3.f, 3.f, 3.f, 100.f);
}

void GLEngine::mouseFunc(int button, int state, int x, int y)
{
}

void GLEngine::activeMotion(int x, int y)
{
  // Active motion is when the mouse is pressed down
  glm::vec2 new_mouse = glm::vec2((double) x / window_width,
      (double) y / window_height);
  glm::vec2 diff = new_mouse - mouse_pos;

  float len = glm::dot(diff, diff);
  // Find the movement from the last frame, and find the
  // length^2 of vector

  if ( len > 0.00001)
  {
    if (scene_scale + diff.x > 0)
    {
      // change the scene's scaling based on horizontal movements of the mouse
      scene_scale += diff.x;
    }
  }

  mouse_pos = new_mouse;

  glutPostRedisplay();
}

void GLEngine::passiveMotion(int x, int y)
{
  // Update the mouse location when not clicking as well
  glm::vec2 new_mouse = glm::vec2((double) x / window_width,
      (double) y / window_height);
  mouse_pos = new_mouse;

  glutPostRedisplay();
}

void GLEngine::keyboard(unsigned char key, int x, int y)
{
  switch(key) 
  {
    case 'a':
      animating ^= 1;
      break;
    case 27:
      clear();
      exit(0);
      break;
  }
  glutPostRedisplay();
}



void GLEngine::clear()
{
  // Cleanup
  for (std::vector<Shader *>::iterator shad = shaders.begin();
      shad != shaders.end(); ++shad)
  {
    delete(*shad);
  }
}

void GLEngine::loadTexture(std::string filename)
{
  // Use FreeImage to load a texture
  // Load the image
  FIBITMAP *bitmap = NULL;

  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
  if (format == FIF_UNKNOWN)
    format = FreeImage_GetFIFFromFilename(filename.c_str());
  if (format == FIF_UNKNOWN)
    return;

  bitmap = FreeImage_Load(format, filename.c_str());
  if (!bitmap)
    return;

  FIBITMAP *bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
  FreeImage_Unload(bitmap);
  if (!bitmap32)
    return;
  bitmap = bitmap32;

  BYTE *bits = FreeImage_GetBits(bitmap);
  int w = FreeImage_GetWidth(bitmap);
  int h = FreeImage_GetHeight(bitmap);

  // R and B are switched in FreeImage

  int order = GL_BGRA;

  GLuint texture;

  // Generate and bind the texture for OpenGL
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
      GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, order, GL_UNSIGNED_BYTE,
      (GLvoid*)bits);

  // Add the Texture ID to our list of textures
  textures.push_back(texture);

  FreeImage_Unload(bitmap);

  return;
}
