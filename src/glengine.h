/*
 * glengine.h
 * The point at which we start calling OpenGL things. We draw, and initialize
 * here.
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#ifndef __GLENGINE_H_
#define __GLENGINE_H_

#include "shader.h"
#include "sphere.h"
#include <vector>
#include <glm/glm.hpp>

class GLEngine {
  public:
    GLEngine(); 
    ~GLEngine(); 

    // The "important" functions
    void display();
    void setupScene();
    void loadShaders();
    void loadUniforms(Shader * cur_shader);

    // The less important functions
    void loadTexture(std::string filename);
    void reshape(int w, int h);
    void keyboard(unsigned char key, int x, int y);
    void mouseFunc(int button, int state, int x, int y);
    void activeMotion(int x, int y);
    void passiveMotion(int x, int y);
    void clear();

  private:
    // Window Properties
    int window_width; 
    int window_height;
    glm::vec2 mouse_pos;

    // Sphere
    // (note that we only save *one* sphere, and draw it multiple times)
    Sphere sph;

    // Shaders
    std::vector<Shader *> shaders;
    Shader * font_shader;

    // Model, View, Projection Matrices
    // Normal matrix as well (inverse tranpose of MV matrix)
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 model_matrix;
    glm::mat4 normal_matrix;

    // Textures
    std::vector<GLuint> textures;

    // Scene properties
    float time;
    float time_prev;
    float time_cur;

    bool animating;
    float scene_scale;

};

#endif
