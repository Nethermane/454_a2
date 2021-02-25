// Phong fragment shader WITH TEXTURES
//
// Phong shading with diffuse and specular components

#version 300 es

uniform sampler2D texUnitID;

uniform mediump vec3 lightDir;

uniform mediump vec2 texDim;

uniform mediump vec3 kd;
uniform mediump vec3 ks;
uniform mediump vec3 Ia;
uniform mediump vec3 Ei;
uniform mediump float shininess;

uniform int mode;

smooth in mediump vec3 colour;
smooth in mediump vec3 normal;
smooth in mediump vec2 texCoords;
smooth in mediump float depth;

out mediump vec4 outputColour;



// Get the mipmap level
//
// From https://stackoverflow.com/questions/24388346/how-to-access-automatic-mipmap-level-in-glsl-fragment-shader-texture


mediump float getMipMapLevel( sampler2D texUnitID, vec2 texCoords )

{
  // For GLSL >= 4.0
  //
  // return textureQueryLod(myTexture, textureCoord).x

  // For GLSL < 4.0

  mediump vec2  dx_vtc        = dFdx( texCoords );
  mediump vec2  dy_vtc        = dFdy( texCoords );
  mediump float delta_max_sqr = max( dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc) );
  mediump float mml = 0.5 * log2(delta_max_sqr);
  
  return max( 0.0, mml );
}


const mediump vec3 colours[6] =
  vec3[6]( vec3( 129.0/255.0,  15.0/255.0, 124.0/255.0 ),
	   vec3( 136.0/255.0,  86.0/255.0, 167.0/255.0 ),
	   vec3( 140.0/255.0, 150.0/255.0, 198.0/255.0 ),
	   vec3( 158.0/255.0, 188.0/255.0, 218.0/255.0 ),
	   vec3( 191.0/255.0, 211.0/255.0, 230.0/255.0 ),
	   vec3( 237.0/255.0, 248.0/255.0, 251.0/255.0 ) );

void main()

{
  // Look up texture value of this fragment

  mediump vec3 texColour = texture( texUnitID, texCoords ).rgb;

  if (mode == 0)
    // 1. Show texture without any shading
    outputColour = vec4( texColour, 1.0f ); // show texture

  else if (mode == 1)
    // 2. show (x,y) texture coordinate as (red,green)
    outputColour = vec4( fract(texCoords.x), fract(texCoords.y), 0.0f, 1.0f );

  else if (mode == 6) {

    // show mipmap levels

    int level = int( floor( getMipMapLevel( texUnitID, texCoords*texDim ) ) );
    if (level > 5)
       level = 5;

    outputColour = vec4( colours[level], 1.0 );

  } else {

    // 3. Standard Phong diffuse+specular calculation

    mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 ); // incoming light

    mediump vec3 N = normalize( normal );

    mediump float NdotL = dot( N, lightDir );

    // Compute the outgoing colour

    mediump vec3 Iout = Ia;

    if (NdotL > 0.0) {

      if (mode == 2)
	// no texturing
	Iout += NdotL * (kd * Iin);

      if (mode == 3)
	// 4. "replace" background with texture
	Iout += NdotL * (texColour * Iin);

      else if (mode == 4)
	// 5. "decal" over background (= (0,4,0) pixels) with texture
	//    (try with Kd = 1,1,1 for tree1 in cube.mtl)
    
	if (texColour == vec3(0,4.0/255.0,0))
	  Iout += NdotL * (kd * Iin);
	else
	  Iout += NdotL * (texColour * Iin);

      else if (mode == 5)
	// 6. "modulate" background with texture
	Iout += NdotL * (kd * (texColour * Iin));

      // specular component

      mediump vec3 R = (2.0 * NdotL) * N - lightDir;
      mediump vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS

      mediump float RdotV = dot( R, V );

      if (RdotV > 0.0)
	Iout += pow( RdotV, shininess ) * (ks * Iin);
    }

    outputColour = vec4( Iout, 1.0f );
  }
}


