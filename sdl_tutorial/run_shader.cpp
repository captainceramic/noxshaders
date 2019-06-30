// This code is based on the tutorial at:
// www.headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>

const std::string programName = "SDL OpenGL Shaders";

const int sizeX = 512;
const int sizeY = 512;

SDL_Window* window;
SDL_GLContext mainContext;
  

int main( int argc, char* args[] ) {
  
  // Start up SDL video
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    std::cout << "Failed to initialize SDL : " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  // Create the window
  window = SDL_CreateWindow(programName.c_str(),
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    sizeX, sizeY,
			    SDL_WINDOW_OPENGL);
  if ( window == nullptr ) {
    std::cout << "Failed to create window : " << SDL_GetError();
    return EXIT_FAILURE;
  }

  // Start setting up some OpenGL stuff
  mainContext = SDL_GL_CreateContext( window );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);
  glewInit();

  // Do something
  glClearColor(0.5, 0.2, 0.0, 1.0);
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( window );
  SDL_Delay( 6000 );

  // Cleanup
  SDL_GL_DeleteContext( mainContext );
  SDL_DestroyWindow( window );
  SDL_Quit();
  

  return EXIT_SUCCESS;
 
}