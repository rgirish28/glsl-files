#version 120

varying vec3 world_normal;
varying vec3 world_pos;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main()
{
  // Simple Blinn-Phong calculation
  // Something to note that we are in camera space, so the eye is at the origin
  // Also, renormalize the interpolated normal vector
  vec3 normal = normalize(world_normal);
  const vec3 eye = vec3(0.f);
  vec3 eye_dir = normalize(eye - world_pos);
  vec3 light_dir = normalize(light_pos - world_pos);

  vec3 half_vec = normalize(light_dir + eye_dir);

  float n_dot_l = dot(normal, light_dir);
  float n_dot_h = dot(normal, half_vec);

  vec3 ambient_color = ambient * light_color;
  vec3 diffuse_color = diffuse * light_color * max(n_dot_l,0.f);
  vec3 specular_color = specular * light_color 
    * pow(max(n_dot_h,0.f), shininess);

  vec3 color = ambient_color + diffuse_color + specular_color;
  
  gl_FragColor = vec4(color,1.0);
}
