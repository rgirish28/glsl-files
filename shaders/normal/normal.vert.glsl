#version 120

varying vec3 world_normal;

attribute vec3 vp_loc;
attribute vec3 vp_norm;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

void main()
{
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vp_loc,1);
  // Normal transformation is done offline (inverse transpose)
  world_normal = normalize((normal_matrix * vec4(vp_norm, 0)).xyz);
}

