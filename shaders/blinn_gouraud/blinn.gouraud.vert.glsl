#version 120
// Gouraud Shading: do all the lighting calculations in the vertex shader

varying vec3 color;

attribute vec3 vp_loc;
attribute vec3 vp_norm;
attribute vec2 vp_texcoord;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

void main()
{
  gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vp_loc,1);
  vec4 world_pos_h = view_matrix * model_matrix * vec4(vp_loc,1);

  vec3 world_pos = world_pos_h.xyz/world_pos_h.w;

  vec3 world_normal = normalize((normal_matrix * vec4(vp_norm,0)).xyz);

  const vec3 eye = vec3(0.f);
  vec3 eye_dir = normalize(eye - world_pos);
  vec3 light_dir = normalize(light_pos - world_pos);

  vec3 half_vec = normalize(light_dir + eye_dir);

  float n_dot_l = dot(world_normal, light_dir);
  float n_dot_h = dot(world_normal, half_vec);

  vec3 ambient_color = ambient * light_color;
  vec3 diffuse_color = diffuse * light_color * max(n_dot_l,0.f);
  vec3 specular_color = specular * light_color 
    * pow(max(n_dot_h,0.f), shininess);

  color = ambient_color + diffuse_color + specular_color;


}

