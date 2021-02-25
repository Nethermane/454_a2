// Pass 1 fragment shader
//
// Outputs colour, normal, depth to three textures

#version 300 es

in mediump vec3 colour;
in mediump vec3 normal;
in mediump float depth;

// Output to the three textures.  Location i corresponds to
// GL_COLOUR_ATTACHMENT + i in the Framebuffer Object (FBO).

layout (location = 0) out mediump vec3 fragColour;
layout (location = 1) out mediump vec3 fragNormal;
layout (location = 2) out mediump vec3 fragDepth;

// Interpolated inputs as simply copied to the three outputs.  You do
// not have to modify this shader.

void main()

{
  fragColour = colour;
  fragNormal = normal;
  fragDepth  = vec3( depth );   // depth is stored (inefficiently) in an RGB texture
}

