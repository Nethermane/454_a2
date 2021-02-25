/*
    Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "headers.h"
#include "gbuffer.h"


GBuffer::GBuffer( unsigned int width, unsigned int height, int nTextures, GLFWwindow *window )

{
  // Check that we can attach at least 5 FBOs

  GLint maxAttachments;
  glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxAttachments );
  if (maxAttachments < 5) {
    cerr << "Can only attach " << maxAttachments << " to an FBO in this version of OpenGL, but 5 are needed." << endl;
    exit(1);
  }

  // Get framebuffer size (which can be DIFFERENT than the window size, and *is* different on Macs!)

  glfwGetFramebufferSize( window, &fbWidth, &fbHeight );
  
  numTextures = nTextures;

  // Create the FBO

  glGenFramebuffers( 1, &FBO );
  glBindFramebuffer( GL_FRAMEBUFFER, FBO );

  // Create the gbuffer textures

  textures = new GLuint[ numTextures ];
  glGenTextures( numTextures, textures );

  for (int i = 0 ; i < numTextures; i++) {

    glBindTexture( GL_TEXTURE_2D, textures[i] );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, fbWidth, fbHeight, 0, GL_RGB, GL_FLOAT, NULL );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glGenerateMipmap( GL_TEXTURE_2D );

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0 );
  }

  // depth

  glGenTextures( 1, &depthTexture );

  glBindTexture( GL_TEXTURE_2D, depthTexture );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, fbWidth, fbHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glGenerateMipmap( GL_TEXTURE_2D );

  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0 );

  // Declare the drawBuffers

  GLenum *drawBuffers = new GLenum[numTextures];

  for (int i=0; i<numTextures; i++)
    drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

  glDrawBuffers( numTextures, drawBuffers );

  // Check

  GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

  if (status != GL_FRAMEBUFFER_COMPLETE) {

    fprintf (stderr, "ERROR: incomplete framebuffer\n");
    if (GL_FRAMEBUFFER_UNDEFINED == status)
      fprintf (stderr, "GL_FRAMEBUFFER_UNDEFINED\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER == status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER== status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
    else if (GL_FRAMEBUFFER_UNSUPPORTED == status)
      fprintf (stderr, "GL_FRAMEBUFFER_UNSUPPORTED\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE == status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
    else if (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS == status)
      fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
    else
      cerr << "glCheckFramebufferStatus() failed.  Status = " << status << endl;
    exit(1);
  }

  // Done

  glBindTexture( GL_TEXTURE_2D, 0 );

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


GBuffer::~GBuffer()

{
  glDeleteFramebuffers( 1, &FBO );
  glDeleteTextures( numTextures, textures );
  glDeleteTextures( 1, &depthTexture );
}


void GBuffer::BindForWriting()

{
  glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );
}


void GBuffer::BindForReading()

{
  glBindFramebuffer( GL_READ_FRAMEBUFFER, FBO );
}


void GBuffer::BindTexture( int textureNumber )

{
  glActiveTexture( GL_TEXTURE0 + textureNumber );
  glBindTexture( GL_TEXTURE_2D, textures[ textureNumber ] );
}


void GBuffer::SetReadBuffer( int textureNumber )

{
  glReadBuffer( GL_COLOR_ATTACHMENT0 + textureNumber );
}


void GBuffer::setDrawBuffers( int numDrawBuffers, int *bufferIDs )

{
  GLenum *drawBuffers = new GLenum[numDrawBuffers];

  for (int i=0; i<numDrawBuffers; i++)
    drawBuffers[i] = GL_COLOR_ATTACHMENT0 + bufferIDs[i];

  glDrawBuffers( numDrawBuffers, drawBuffers );

  delete [] drawBuffers;
}


// Debugging output
//
// LL = position
// UL = colour
// UR = normal
// LR = depth


void GBuffer::DrawGBuffers()

{
  // Clear window

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Blit textures onto window

  glBindFramebuffer( GL_READ_FRAMEBUFFER, FBO );

  GLsizei halfWidth = (GLsizei)(fbWidth / 2.0f);
  GLsizei halfHeight = (GLsizei)(fbHeight / 2.0f);

  SetReadBuffer( 0 );
  glBlitFramebuffer(0, 0, fbWidth, fbHeight, 0, 0,                 halfWidth, halfHeight,      GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer( 1 );
  glBlitFramebuffer(0, 0, fbWidth, fbHeight, 0, halfHeight,         halfWidth, fbHeight,   GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer( 2 );
  glBlitFramebuffer(0, 0, fbWidth, fbHeight, halfWidth, halfHeight, fbWidth, fbHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SetReadBuffer( 3 );
  glBlitFramebuffer(0, 0, fbWidth, fbHeight, halfWidth, 0,          fbWidth, halfHeight,   GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
