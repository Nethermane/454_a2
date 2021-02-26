// Pass 3 fragment shader
//
// Output fragment colour based using
//    (a) Cel shaded diffuse surface
//    (b) wide silhouette in black

#version 300 es

precision mediump float;

uniform mediump vec3 lightDir;     // direction toward the light in the VCS
uniform mediump vec2 texCoordInc;  // texture coord difference between adjacent texels

in mediump vec2 texCoords;              // texture coordinates at this fragment

// The following four textures are now available and can be sampled
// using 'texCoords'

uniform sampler2D colourSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;
uniform sampler2D laplacianSampler;

out mediump vec4 outputColour;          // the output fragment colour as RGBA with A=1


void main()

{
  mediump vec2 dummy = texCoords;  // REMOVE THIS ... It's just here because MacOS complains otherwise

  // [0 marks] Look up values for the depth and Laplacian.  Use only
  // the R component of the texture as texture2D( ... ).r
  float depth = texture(depthSampler,texCoords).r;
  float lapla = texture(laplacianSampler,texCoords).r;

  // [1 mark] Discard the fragment if it is a background pixel not
  // near the silhouette of the object.

  if(lapla == 0.0 && depth == 1.0){
    discard;
  }
  

  // [0 marks] Look up value for the colour and normal.  Use the RGB
  // components of the texture as texture2D( ... ).rgb or texture2D( ... ).xyz.

  vec3 normal = texture(normalSampler,texCoords).xyz;
  vec3 colour = texture(colourSampler,texCoords).rgb;

  // [2 marks] Compute Cel shading, in which the diffusely shaded
  // colour is quantized into four possible values.  Do not allow the
  // diffuse component, N dot L, to be below 0.2.  That will provide
  // some ambient shading.  Your code should use the 'numQuata' below
  // to have that many divisions of quanta of colour.  Do not use '3'
  // in your code; use 'numQuanta'.  Your code should be very efficient.

  // YOUR CODE HERE
  const int numQuanta = 3;
  //fragLaplacian = total > 0.85 ? vec3(total,total,total): vec3(0.0,0.0,0.0);

  float diffuseIntensity = (normal.x*lightDir.x) + (normal.y*lightDir.y) + (normal.z*lightDir.z);
  diffuseIntensity = max(diffuseIntensity, 0.2);


  float level = min((floor(float(numQuanta)*diffuseIntensity))/(float(numQuanta-1)),1.0);
  level = max(level, 0.2);
  

  //????


  // [2 marks] Look at the fragments in the 3x3 neighbourhood of
  // this fragment.  Your code should use the 'kernelRadius'
  // below and check all fragments in the range
  //
  //    [-kernelRadius,+kernelRadius] x [-kernelRadius,+kernelRadius]
  //
  // around this fragment.
  //
  // Find the neighbouring fragments with a Laplacian beyond some
  // threshold.  Of those fragments, find the distance to the closest
  // one.  That distance, divided by the maximum possible distance
  // inside the kernel, is the blending factor.
  //
  // You can use a large kernelRadius here (e.g. 10) to see that
  // blending is being done correctly.  Do not use '3.0' or '-0.1' in
  // your code; use 'kernelRadius' and 'threshold'.

  const mediump float kernelRadius = 3.0;
  const mediump float threshold = -0.1;

  float dist = kernelRadius;
  float blendingFactor = -1.0;

  for(int i = (-int(kernelRadius)); i <= int(kernelRadius); i++){
    for(int j = (-int(kernelRadius)); j <= int(kernelRadius); j++){

        if(texture(laplacianSampler,vec2(texCoords.x + float(i)*texCoordInc.x, texCoords.y + float(j)*texCoordInc.y)).r >= (-threshold)){
          if(dist > sqrt(float(i*i + j*j))){
            dist = sqrt(float(i*i + j*j));
            blendingFactor = dist/(sqrt( float((int(kernelRadius*kernelRadius))*2) ));
          }
        }
    }
  }

  

  // YOUR CODE HERE

  // [1 mark] Output the fragment colour.  If there is an edge
  // fragment in the 3x3 neighbourhood of this fragment, output a grey
  // colour based on the blending factor.  The grey should be
  // completely black for an edge fragment, and should blend to the
  // Phong colour as distance from the edge increases.  If these is no
  // edge in the neighbourhood, output the cel-shaded colour.
  
  // YOUR CODE HERE
  if(blendingFactor == -1.0){
   outputColour = vec4( level*colour.x, level*colour.y, level*colour.z, 1.0 );
  }
  else{
    outputColour = vec4(mix(vec3(0.0,0.0,0.0), level*colour.xyz,blendingFactor),1.0);
  }

  //outputColour = vec4(mix( level*colour.xyz,vec3(0.0,0.0,0.0),1.0-blendingFactor),1.0);
}
