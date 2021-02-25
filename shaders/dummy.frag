// Dummy fragment shader
//
// For debugging: Just pass through the colour

#version 300 es

in mediump vec3 colour;
out mediump vec4 outputColour;

void main()

{
  outputColour = vec4( colour, 1.0 );
}
