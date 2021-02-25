// GPUProgram class

#ifndef SHADER_H
#define SHADER_H


#include "headers.h"


class GPUProgram {

  unsigned int program_id;
  unsigned int shader_vp;
  unsigned int shader_fp;

 public:

  GPUProgram() {};

  GPUProgram( const char *vsFile, const char *fsFile ) {
    initFromFile( vsFile, fsFile );
  }

  ~GPUProgram() {
    glDetachShader( program_id, shader_vp );
    glDeleteShader( shader_vp );

    glDetachShader( program_id, shader_fp );
    glDeleteShader( shader_fp );

    glDeleteProgram( program_id );
  }

  void init( char *vsText, char *fsText );

  int id() {
    return program_id;
  }

  void activate() {
    glUseProgram( program_id );
  }

  void deactivate() {
    glUseProgram( 0 );
  }

  char* textFileRead(const char *fileName);

  void setMat4( char *name, mat4 &M ) {
    glUniformMatrix4fv( glGetUniformLocation( program_id, name ), 1, GL_TRUE, &M[0][0] );
  }

  void setVec3( char *name, vec3 v ) {
    glUniform3fv( glGetUniformLocation( program_id, name ), 1, &v[0] );
  }

  void setVec2( char *name, vec2 v ) {
    glUniform2fv( glGetUniformLocation( program_id, name ), 1, &v[0] );
  }

  void setVec4( char *name, vec4 v ) {
    glUniform4fv( glGetUniformLocation( program_id, name ), 1, &v[0] );
  }

  void setFloat( char *name, float f ) {
    glUniform1f( glGetUniformLocation( program_id, name ), f );
  }

  void setInt( char *name, int i ) {
    glUniform1i( glGetUniformLocation( program_id, name ), i );
  }

  void glErrorReport( char *where ) {

    GLuint errnum;
    bool gotErrors = false;
    
    while ((errnum = glGetError())) {
      std::cerr << where << ": OpenGL error " << errnum << std::endl;
      gotErrors = true;
    }
    
    if (gotErrors)
      exit(1);
  }

  void initFromFile( const char *vsFile, const char *fsFile );
  void validateShader( GLuint shader, const char* file = 0 );
  void validateProgram();
};

#endif
