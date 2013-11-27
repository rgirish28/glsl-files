/*
 * Some GLSL example code
 *
 * main.cpp
 * The entry point to our program. Most of this calls functions in GLEngine
 *
 *  Created on: Feb 13, 2013
 *      Author: Brandon Wang
 */

#include "glengine.h"
#include "sphere.h"
#include <GL/glew.h>
#include <GL/glut.h>

// Handle everything through GLEngine
GLEngine engine;

void keyboard(unsigned char key, int x, int y)
{
  engine.keyboard(key,x,y);
}

void display()
{
  engine.display();
}

void reshape(int w, int h)
{
  engine.reshape(w,h);
}

void mouseFunc(int button, int state, int x, int y)
{
  engine.mouseFunc(button, state, x, y);
}

void activeMotion(int x, int y)
{
  engine.activeMotion(x,y);
}

void passiveMotion(int x, int y)
{
  engine.passiveMotion(x,y);
}


int main(int argc, char** argv)
{
  const unsigned int width = 880;
  const unsigned int height = 800;

  glutInit(&argc, argv);
  

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(width,height);
  glutCreateWindow("CS184 Sp13 Shader Homework");
  
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "GLEW Error: " << glewGetString(err) << std::endl;
  }

  engine.reshape(width, height);
  engine.setupScene();

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard); 
  glutMouseFunc(mouseFunc);
  glutMotionFunc(activeMotion);
  glutPassiveMotionFunc(passiveMotion);
  glutIdleFunc(display);

  glutMainLoop();
  return 0;
}
