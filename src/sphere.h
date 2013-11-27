/*
 * sphere.h
 * Draw a sphere
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "shader.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class Sphere
{
  protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLuint> indices;
    GLuint vbo_vertices, vbo_normals, vbo_texcoords, ibo_elements;

  public:
  Sphere();
  void create(unsigned int lat_bands, unsigned int long_bands);
  void draw(Shader * shader);
};

#endif
