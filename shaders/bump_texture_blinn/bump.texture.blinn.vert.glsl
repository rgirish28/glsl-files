#version 120

varying vec3 world_normal;
varying vec3 world_pos;
varying vec2 texcoord;

attribute vec3 vp_loc;
attribute vec3 vp_norm;
attribute vec2 vp_texcoord;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

void main()
{
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vp_loc,1);
  vec4 world_pos_h = view_matrix * model_matrix * vec4(vp_loc,1);
  world_pos = world_pos_h.xyz/world_pos_h.w;

  world_normal = normalize((normal_matrix * vec4(vp_norm,0)).xyz);
  texcoord = vp_texcoord;
}

