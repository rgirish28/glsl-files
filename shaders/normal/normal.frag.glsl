#version 120

varying vec3 world_normal;

void main()
{
  // Renormalize the normal, because it may not be unit after interpolation
  gl_FragColor = vec4(normalize(world_normal), 1.f);
}
