// Draw a quad at the mouse position.  Texture the quad with zoomed-pixels near the mouse position


#include "pixelZoom.h"


#define ZOOM_FACTOR 13          // scale applied to one pixel
#define ZOOM_RADIUS 15          // radius of window, in original pixels


char *PixelZoom::vertShader = "\n\
\n\
#version 300 es\n\
\n\
layout (location = 0) in vec2 vertPosition;\n\
layout (location = 1) in vec2 vertTexCoords;\n\
\n\
smooth out mediump vec2 texCoords;\n\
\n\
void main() {\n\
  gl_Position = vec4( vertPosition.x, vertPosition.y, 0, 1 );\n\
  texCoords = vertTexCoords;\n\
}";


char *PixelZoom::fragShader = "\n\
\n\
#version 300 es\n\
\n\
uniform sampler2D texUnitID;\n\
uniform int drawLine;\n\
\n\
smooth in mediump vec2 texCoords;\n\
out mediump vec4 outputColour;\n\
\n\
void main() {\n\
  if (drawLine == 1)\n\
    outputColour = vec4( 0.5, 0.5, 0.5, 1 );\n\
  else\n\
    outputColour = texture( texUnitID, texCoords );\n\
}";



PixelZoom::PixelZoom()

{
  program.init( vertShader, fragShader );
}


void PixelZoom::zoom( GLFWwindow *window, vec2 mouse, vec2 windowDim )

{
  // Get pixels from the framebuffer

  const int pixelsWidth = 2*ZOOM_RADIUS+1;

  // Build a texture of the pixels

  GLuint texUnitID = 0;
  
  GLuint texID;
  glGenTextures( 1, &texID );

  glActiveTexture( GL_TEXTURE0 + texUnitID );
  glBindTexture( GL_TEXTURE_2D, texID );

  unsigned char *pixels = new unsigned char[ 3 * pixelsWidth * pixelsWidth ];

  for (int i=0; i<3*pixelsWidth*pixelsWidth; i++)
    pixels[i] = 128;
  
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

  // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pixelsWidth, pixelsWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

  glBindFramebuffer(GL_FRAMEBUFFER, 0); 

  int fbWidth, fbHeight;
  glfwGetFramebufferSize( window, &fbWidth, &fbHeight );

  int winWidth, winHeight;
  glfwGetWindowSize( window, &winWidth, &winHeight );
 
  glReadBuffer( GL_BACK);
  
  glCopyTexImage2D( GL_TEXTURE_2D,
  		    0,
  		    GL_RGB,
  		    (int) (mouse.x * (fbWidth/(float)winWidth) - ZOOM_RADIUS),
		    (int) ((windowDim.y-mouse.y) * (fbHeight/(float)winHeight) - ZOOM_RADIUS),
		    pixelsWidth,
		    pixelsWidth,
  		    0 );
  
  // Draw texture on a small quad

  vec2 texPos( mouse.x / windowDim.x, (windowDim.y - mouse.y) / windowDim.y ); // mouse position in texture
  vec2 winPos( 2*texPos.x-1, 2*texPos.y-1); // mouse position in window
    
  vec2 texRadius = vec2( ZOOM_RADIUS / windowDim.x, ZOOM_RADIUS / windowDim.y ); // radius in texture coordinates
  vec2 winRadius( ZOOM_FACTOR*2*texRadius.x, ZOOM_FACTOR*2*texRadius.y ); // radius in window coordinates

  vec2 verts[8] = {
    winPos + vec2( -winRadius.x, -winRadius.y ), // positions
    winPos + vec2( -winRadius.x,  winRadius.y ),
    winPos + vec2(  winRadius.x, -winRadius.y ),
    winPos + vec2(  winRadius.x,  winRadius.y ),

    vec2( 0, 0 ), // texture coordinates
    vec2( 0, 1 ),
    vec2( 1, 0 ),
    vec2( 1, 1 )
  };
    
  GLuint VAO, VBO;

  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );

  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(vec2)*4) );

  // Set up GPU

  glDisable( GL_DEPTH_TEST );
  program.activate();
  program.setInt( "texUnitID", texUnitID );
  program.setInt( "drawLine", 0 );

  // Draw texture

  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  // Draw boundary

  vec2 lineverts[4] = {
    winPos + vec2( -winRadius.x, -winRadius.y ), // positions
    winPos + vec2(  winRadius.x, -winRadius.y ),
    winPos + vec2(  winRadius.x,  winRadius.y ),
    winPos + vec2( -winRadius.x,  winRadius.y )
  };
    
  glBufferData( GL_ARRAY_BUFFER, sizeof(lineverts), lineverts, GL_STATIC_DRAW );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  glDisableVertexAttribArray( 1 );

  program.setInt( "drawLine", 1 );

#ifndef MACOS  
  glLineWidth( 5 );
#endif
  glDrawArrays( GL_LINE_LOOP, 0, 4 );
#ifndef MACOS
  glLineWidth( 1 );
#endif

  // Done

  program.deactivate();

  glEnable( GL_DEPTH_TEST );

  glDeleteBuffers( 1, &VBO );
  glDeleteVertexArrays( 1, &VAO );

  glBindTexture( GL_TEXTURE_2D, 0 );

  delete [] pixels;

  glDeleteTextures( 1, &texID );
}
