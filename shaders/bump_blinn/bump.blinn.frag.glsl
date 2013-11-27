#version 120

varying vec3 world_normal;
varying vec3 world_pos;
varying vec2 texcoord;

uniform sampler2D bump_map;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main()
{

  vec3 bump_val = texture2D(bump_map, texcoord).xyz;

  vec3 ref_normal = normalize(world_normal);
  // Compute Tangent Space
  vec3 tangent;
  vec3 bitangent;
  vec3 c1 = cross(ref_normal, vec3(0.f, 0.f, 1.f));
  vec3 c2 = cross(ref_normal, vec3(0.f, 1.f, 0.f));
  if (length(c1) > length(c2))
  {
    tangent = normalize(c1);
  } else {
    tangent = normalize(c2);
  }
  bitangent = normalize(cross(ref_normal, tangent));

  vec3 tbn_normal = normalize(bump_val - vec3(0.5f));
  vec3 normal = mat3(tangent, bitangent, ref_normal) * tbn_normal;

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
