// G-buffer renderer

#ifndef RENDER_H
#define RENDER_H


#include "wavefront.h"
#include "gpuProgram.h"
#include "gbuffer.h"


class Renderer {

  enum { COLOUR_GBUFFER,
	 NORMAL_GBUFFER,
	 DEPTH_GBUFFER,
	 LAPLACIAN_GBUFFER,
	 NUM_GBUFFERS };

  GPUProgram *pass1Prog, *pass2Prog, *pass3Prog, *dummyProg;
  GBuffer    *gbuffer;

  int windowWidth, windowHeight;

 public:

  int debug;

  Renderer( int width, int height, GLFWwindow *window ) {

    windowWidth = width;
    windowHeight = height;
    gbuffer = new GBuffer( width, height, NUM_GBUFFERS, window );
    pass1Prog = new GPUProgram( "shaders/pass1.vert", "shaders/pass1.frag" );
    pass2Prog = new GPUProgram( "shaders/pass2.vert", "shaders/pass2.frag" );
    pass3Prog = new GPUProgram( "shaders/pass3.vert", "shaders/pass3.frag" );
    dummyProg = new GPUProgram( "shaders/dummy.vert", "shaders/dummy.frag" );
    debug = 3;  // initially show output of pass 3
  }

  ~Renderer() {
    delete gbuffer;
    delete pass3Prog;
    delete pass2Prog;
    delete pass1Prog;
    delete dummyProg;
  }

  void reshape( int width, int height, GLFWwindow *window ) {
    windowWidth = width;
    windowHeight = height;
    delete gbuffer;
    gbuffer = new GBuffer( width, height, NUM_GBUFFERS, window );
  }

  void render( wfModel *obj, mat4 &M, mat4 &MV, mat4 &MVP, vec3 &lightDir );

  void incDebug() {
    debug = (debug+1) % 4; // cycle in 0,1,2,3
  }

  void makeStatusMessage( char *buffer ) {
    if (debug == 0)
      sprintf( buffer, "Program output" );
    else
      sprintf( buffer, "After pass %d", debug );
  }
};

#endif
