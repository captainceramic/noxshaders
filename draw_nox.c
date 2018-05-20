/* This is my attempt to set up an EGL context using the hardware
   accelerated OpenGLES 2.0 driver on the raspberry pi */
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "bcm_host.h"

#include "shader_loader.h"


#define SCREEN_X 800
#define SCREEN_Y 800



const bool scale_down = true;

/* Struct to hold all the global state objects */
typedef struct {
  uint32_t screen_width;
  uint32_t screen_height;
  // OpenGL|ES objects
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  GLuint tex[6];
} APP_STATE_T;

/* I don't quite understand this...
   create a new struct, then create
   a pointer to the start of it. Why can't we
   do this all in one? */
static APP_STATE_T _state, *state = &_state;

static void init_ogl(APP_STATE_T *t) {
  /* Get OpenGLES ticking over. */
  int32_t success = 0;
  EGLBoolean result;
  EGLint num_config;

  static EGL_DISPMANX_WINDOW_T nativewindow;

  DISPMANX_UPDATE_HANDLE_T dispman_update;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  static const EGLint attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_NONE
  };

  static const EGLint context_attributes[] =
   {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
   };

  EGLConfig config;

  // Get a reference to the current display
  state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(state->display != EGL_NO_DISPLAY);

  // Initialise EGL and get the versions.
  EGLint major;
  EGLint minor;
  result = eglInitialize(state->display, &major, &minor);
  assert(result != EGL_FALSE);
  printf("EGL version - major: %d, minor: %d\n", major, minor);


  // Choose an appropriate EGL framebuffer configuration.
  result = eglChooseConfig(state->display, attribute_list,
			   &config, 1, &num_config);
  assert(EGL_FALSE != result);

  /* Appropriate EGL frame buffer configration */
  result = eglBindAPI(EGL_OPENGL_ES_API);
  assert(EGL_FALSE != result);

  /* Get a rendering context */
  state->context = eglCreateContext(state->display, config,
				    EGL_NO_CONTEXT, context_attributes);
  assert(state->context != EGL_NO_CONTEXT);

  /* get the display size */
  success = graphics_get_display_size(0, &state->screen_width, &state->screen_height);
  assert(success >= 0);

  /* Scale down the size for performace. */
  if (scale_down) {
    state->screen_width = SCREEN_X;
    state->screen_height = SCREEN_Y;
  }

  /* Keep creating the window surface */
  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = state->screen_width;
  dst_rect.height = state->screen_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = state->screen_width << 16;
  src_rect.height = state->screen_height << 16;

  state->dispman_display = vc_dispmanx_display_open(0);
  dispman_update = vc_dispmanx_update_start(0);

  state->dispman_element = vc_dispmanx_element_add(dispman_update,
						   state->dispman_display,
						   0, &dst_rect,
						   0, &src_rect,
						   DISPMANX_PROTECTION_NONE,
						   0, 0, 0);

  nativewindow.element = state->dispman_element;
  nativewindow.width = state->screen_width;
  nativewindow.height = state->screen_height;

  vc_dispmanx_update_submit_sync(dispman_update);

  state->surface = eglCreateWindowSurface(state->display, config,
					  &nativewindow, NULL);
  assert(state->surface != EGL_NO_SURFACE);

  // Connect the context to the surface.
  result = eglMakeCurrent(state->display, state->surface,
			  state->surface, state->context);

  assert(EGL_FALSE != result);

  if (EGL_SUCCESS != eglGetError()) {
    printf("egl error!");
  }

  /* Check the renderer information */
  printf("vendor: %s\n", glGetString(GL_VENDOR));
  printf("version: %s\n", glGetString(GL_VERSION));
  printf("renderer: %s\n", glGetString(GL_RENDERER));
  printf("shading language version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

}


int main() {

  /* Set up the broadcom stuff */
  bcm_host_init();
  printf("initialised broadcom driver...\n");

  /* Zero out the initial state of the application */
  memset(state, 0, sizeof(*state));
  init_ogl(state);

  /* Set up the object to draw! */
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

  /* Load up our shaders */
  GLuint shader_program = load_shaders("./shaders/shader.vert",
				       "./shaders/shader.frag");

  printf("Our shader program id is: %d\n", shader_program);

  GLuint t_loc = glGetUniformLocation(shader_program, "u_time");
  GLuint r_loc = glGetUniformLocation(shader_program, "u_resolution");

  printf("Screen width and height is: %d x %d\n",
	 state->screen_width, state->screen_height);

  glViewport(0, 0, state->screen_width, state->screen_height);

  /* Now we see if we can draw something! */
  int frames = 0;
  int timeDiff;
  time_t startTime, endTime;

  glClearColor(0.15f, 0.25f, 0.35f, 1.0f);

  bool should_exit = false;
  startTime = time(NULL);

  // This sets how long the shader will run.
  int max_frames = 60 * 60;
  int full_frames = 0;
  while(false == should_exit) {

    glUseProgram(shader_program);

    // Set the time uniform. (assume 60 FPS)
    glUniform1f(t_loc, (float)full_frames / 60.0);

    // Set the resolution uniform.
    glUniform2f(r_loc,
		(float)state->screen_width,
		(float)state->screen_height);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    eglSwapBuffers(state->display, state->surface);

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

  return 0;

}
