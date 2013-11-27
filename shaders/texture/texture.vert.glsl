#version 120

varying vec2 texcoord;

attribute vec3 vp_loc;
attribute vec2 vp_texcoord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vp_loc,1);

  texcoord = vp_texcoord;
}

