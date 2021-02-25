// phong vertex shader WITH TEXTURES

#version 300 es

uniform mat4 MVP;		// OCS-to-CCS
uniform mat4 MV;		// OCS-to-VCS

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoords; // just another vertex attribute

smooth out mediump vec3 colour;	// smooth = interpolated across triangle
smooth out mediump vec3 normal;
smooth out mediump vec2 texCoords;
smooth out mediump float depth;

void main()

{
  // calc vertex position in CCS

  mediump vec4 ccs_pos = MVP * vec4( vertPosition, 1.0f );
  gl_Position = ccs_pos;

  // Depth

  depth = 0.5 * ((ccs_pos.z / ccs_pos.w) + 1.0); /* depth in [0,1] */

  // assign (r,g,b) colour

  colour = 2.0 * vec3( 0.33, 0.42, 0.18 );

  // calc normal in VCS
  //
  // To do this, apply the non-translational parts of MV to the model
  // normal.  The 0.0 as the fourth component of the normal ensures
  // that no translational component is added.

  normal = vec3( MV * vec4( vertNormal, 0.0 ) );

  // Texture coordinates

  texCoords = vertTexCoords;
}
