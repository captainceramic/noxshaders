// This code is based on the tutorial at:
// www.headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

const std::string programName = "SDL OpenGLES Shaders";

/* Set some parameters if necessary */
const bool scale_down = true;
const int num_seconds = 40;

const int sizeX = 1024;
const int sizeY = 768;

/* Globals for the window and context */
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

  std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "version: " <<  glGetString(GL_VERSION) << std::endl;
  std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  /* Set up the object to draw! (just a big rectangle / two triangles) */
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f};

  GLuint VBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
   	       vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
   			3 * sizeof(GLfloat), (void*)0);

  glEnableVertexAttribArray(0);

  /* Now we see if we can draw something! */
  int frames = 0;
  int timeDiff;
  time_t startTime, endTime;

  bool should_exit = false;
  startTime = time(NULL);

  // This sets how long (approx) the shader will run.
  // 60fps, by a number of seconds.
  int max_frames = 60 * num_seconds;
  int full_frames = 0;

  while(false == should_exit) {

    float frac = (float) full_frames / (float) max_frames;
    
    glClearColor(frac, 1.0f - frac, frac, 1.0f - frac);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
      
    if (full_frames > max_frames) {
      should_exit = true;
    }
    full_frames++;

    /* Handle FPS */
    frames += 1;
    endTime = time(NULL);
    timeDiff = (int)(endTime - startTime);
    if(timeDiff >= 3) {
      printf("Current FPS is: %0.3f\n", (float) frames / (float) timeDiff);
      startTime = time(NULL);
      frames = 0;
    }

  }

  glDeleteBuffers(1, &VBO);
 
  // Cleanup
  glDeleteBuffers(1, &VBO);
  SDL_GL_DeleteContext( mainContext );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return EXIT_SUCCESS;
 
}
