// Dummy vertex shader
//
// For debugging: Just computes gl_Position and sets a colour.

#version 300 es

uniform mat4 MVP;

layout (location = 0) in mediump vec3 vertPosition;

out mediump vec3 colour;

void main()

{
  gl_Position = MVP * vec4( vertPosition, 1.0 );
  colour = vec3( 0.66, 0.84, 0.36 );
}
