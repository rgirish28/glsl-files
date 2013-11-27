/*
 * sphere.cpp
 * Draw a sphere
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */
#define _USE_MATH_DEFINES
#include "sphere.h"
#include <glm/glm.hpp>

Sphere::Sphere()
{
  vertices.clear();
  normals.clear();
  texcoords.clear();
  indices.clear();
}

void Sphere::create(unsigned int lat_bands, unsigned int long_bands)
{
  vertices.clear();
  normals.clear();
  texcoords.clear();
  indices.clear();

  for(unsigned int lat_num = 0; lat_num <= lat_bands; ++lat_num)
  {
    float theta = lat_num * M_PI / lat_bands;
    float sin_theta = sin(theta);
    float cos_theta = cos(theta);

    for(unsigned int long_num = 0; long_num <= long_bands; ++long_num)
    {
      float phi = long_num * 2 * M_PI / long_bands;
      float sin_phi = sin(phi);
      float cos_phi = cos(phi);

      float x = cos_phi * sin_theta;
      float y = cos_theta;
      float z = sin_phi * sin_theta;

      float u = 1.f - ((float)long_num / long_bands);
      float v = 1.f - ((float)lat_num / lat_bands);

      normals.push_back(x);
      normals.push_back(y);
      normals.push_back(z);

      texcoords.push_back(u);
      texcoords.push_back(v);

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
    }
  }

  for(unsigned int lat_num = 0; lat_num < lat_bands; ++lat_num)
  {
    for(unsigned int long_num = 0; long_num < long_bands; ++long_num)
    {
      unsigned int first = (lat_num * (long_bands + 1)) + long_num;
      unsigned int second = first + long_bands + 1;
      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(second);
      indices.push_back(second + 1);
      indices.push_back(first + 1);

    }
  }

  // OpenGL 3+ style rendering:
  // Create arrays of values (above), and bind appropriate buffers.
  // Here, we bind the buffers:
  // vertices, normals, texture coordinates,
  // elements (indices to connect polygons)

  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
      vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_normals);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]),
      normals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_texcoords);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(texcoords[0]),
      texcoords.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &ibo_elements);
  glBindBuffer(GL_ARRAY_BUFFER, ibo_elements);
  glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
      indices.data(), GL_STATIC_DRAW);

}

void Sphere::draw(Shader * shader)
{
  // Rendering in OpenGL 3+
  // Bind the attribute locations in the shader
  // Point to the appropriate buffers created earlier
  int v_coord_attrib = shader->attribute_v_coord;
  int v_normal_attrib = shader->attribute_v_normal;
  int v_texcoord_attrib = shader->attribute_v_texcoord;
  glEnableVertexAttribArray(v_coord_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glVertexAttribPointer(
      v_coord_attrib,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      0
      );

  glEnableVertexAttribArray(v_normal_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
  glVertexAttribPointer(
      v_normal_attrib,
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      0
      );

  glEnableVertexAttribArray(v_texcoord_attrib);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
  glVertexAttribPointer(
      v_texcoord_attrib,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      0
      );


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  int size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(v_coord_attrib);
  glDisableVertexAttribArray(v_normal_attrib);
  glDisableVertexAttribArray(v_texcoord_attrib);
}

