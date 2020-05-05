// This code is based on the tutorial at:
// www.headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

const std::string programName = "SDL OpenGLES Shaders";

const int sizeX = 1024;
const int sizeY = 768;

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
			    SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN );
  if ( window == nullptr ) {
    std::cout << "Failed to create window : " << SDL_GetError();
    return EXIT_FAILURE;
  }

  // Start setting up some OpenGLES stuff
  mainContext = SDL_GL_CreateContext( window );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(1);

  // Do something
  glClearColor(0.2, 0.6, 0.2, 1.0);
  glClear( GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow( window );
  SDL_Delay( 6000 );

  // Cleanup
  SDL_GL_DeleteContext( mainContext );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return EXIT_SUCCESS;
 
}
