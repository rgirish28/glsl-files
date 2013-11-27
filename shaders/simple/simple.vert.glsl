#version 120

// We are using GLSL 1.20, which corresponds to OpenGL version 3.0
// This version was chosen for compatability with most systems
// There are newer versions of GLSL, along with newer OpenGL versions.
// Newer versions of GLSL have different syntax.

// Attributes are passed in, generally per-vertex
attribute vec3 vp_loc;

// Uniforms are also passed in, but are generally constant for a group
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

// Varying (not shown here) are variables output from the vertex shader
// and interpolated, and given to the fragment shader.

void main()
{
  // Vertex Shaders output gl_Position, the transformed position of the vertex
  gl_Position = projection_matrix * view_matrix * model_matrix 
    * vec4(vp_loc,1);
}

