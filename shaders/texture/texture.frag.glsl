#version 120

varying vec2 texcoord;
uniform sampler2D texture_map;

void main()
{
  gl_FragColor = vec4(texture2D(texture_map,texcoord));
}
