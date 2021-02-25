// Pass 2 fragment shader
//
// Outputs the Laplacian, computed from depth buffer

#version 300 es

precision highp float;

// texCoordInc = the x and y differences, in texture coordinates,
// between one texel and the next.  For a window that is 400x300, for
// example, texCoordInc would be (1/400,1/300).

uniform mediump vec2 texCoordInc;

// texCoords = the texture coordinates at this fragment

in mediump vec2 texCoords;

// depthSampler = texture sampler for the depths.

uniform mediump sampler2D depthSampler;

// fragLaplacian = an RGB value that is output from this shader.  All
// three components should be identical.  This RGB value will be
// stored in the Laplacian texture.

layout (location = 0) out mediump vec3 fragLaplacian;


void main()

{
  // YOUR CODE HERE.  You will have to compute the Laplacian by
  // evaluating a 3x3 filter kernel at the current texture
  // coordinates.  The Laplacian weights of the 3x3 kernel are
  //
  //      -1  -1  -1
  //      -1   8  -1
  //      -1  -1  -1
  //
  // Store a signed value for the Laplacian; do not take its absolute
  // value.
  vec4 lapTL = -texture(depthSampler, vec2(texCoords.x-texCoordInc.x,texCoords.y-texCoordInc.y));
  vec4 lapTC = -texture(depthSampler, vec2(texCoords.x,texCoords.y-texCoordInc.y));
  vec4 lapTR = -texture(depthSampler, vec2(texCoords.x+texCoordInc.x,texCoords.y-texCoordInc.y));
  vec4 lapCL = -texture(depthSampler, vec2(texCoords.x-texCoordInc.x,texCoords));
  vec4 lapCC = 8.0 * texture(depthSampler, texCoords);
  vec4 lapCR = -texture(depthSampler, vec2(texCoords.x+texCoordInc.x,texCoords.y));
  vec4 lapBL = -texture(depthSampler, vec2(texCoords.x-texCoordInc.x,texCoords.y+texCoordInc.y));
  vec4 lapBC = -texture(depthSampler, vec2(texCoords.x,texCoords.y+texCoordInc.y));
  vec4 lapBR = -texture(depthSampler, vec2(texCoords.x+texCoordInc.x,texCoords.y+texCoordInc.y));
  vec4 color = lapTL + lapTC + lapTR + lapCL + lapCC + lapCR + lapBL + lapBC + lapBR;
  fragLaplacian = color.z < 0.67 ? vec3(0.0,0.0,0.0): vec3(color.z,color.z,color.z);
}
