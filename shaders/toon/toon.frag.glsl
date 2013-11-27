#version 120
// This toon shading is just a banded diffuse calculation

varying vec3 world_normal;
varying vec3 world_pos;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 diffuse;

void main()
{
  vec3 normal = normalize(world_normal);
  vec3 light_dir = normalize(light_pos - world_pos);

  float n_dot_l = dot(normal, light_dir);

  float intensity = max(n_dot_l,0.f);

  vec3 color;

  if (intensity > 0.95)
    color = diffuse * light_color;
  else if (intensity > 0.5)
    color = diffuse * light_color * vec3(0.6);
  else if (intensity > 0.25)
    color = diffuse * light_color * vec3(0.4);
  else
    color = diffuse * light_color * vec3(0.2);
  
  gl_FragColor = vec4(color,1.0);
}
