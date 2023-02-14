#define SOKOL_IMPL
#define SOKOL_GLCORE33
// NOTE(cs): Linking like this because 
#include "../vendor/sokol_gp/thirdparty/sokol_gfx.h"
#include "../vendor/sokol_gp/sokol_gp.h"
#include "../vendor/sokol_gp/thirdparty/sokol_app.h"
#include "../vendor/sokol_gp/thirdparty/sokol_glue.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// GAME LOOP
static void frame(void) {
  
  int width = sapp_width(), height = sapp_height();
  float ratio = width/(float)height;
  
  sgp_begin(width, height);
  sgp_viewport(0, 0, width, height);
  sgp_project(-ratio, ratio, 1.0f, -1.0f);
  
  // NOTE(cs): ANIMATE THINGS
  float time = sapp_frame_count() * sapp_frame_duration();
  //
  float r = sinf(time)*0.5+0.5, g = cosf(time)*0.5+0.5;
  
  sgp_set_color(g, r, 0.6f, 1.0f);
  sgp_clear();
  
  sgp_set_color(r, g, 0.3f, 1.0f);
  
  {
    float xx = time*0.1f;
  
    sgp_draw_filled_rect(xx, -0.5, 0.1f, 0.1f);
  
    // NOTE(cs): WILL MOVE LEFT BECAUSE TIME IS NEGATIVE 
    float xy = time*-0.1f;
    // NOTE(cs): ACTUAL X {POSITION}
    float ox = -0.1;
    sgp_draw_filled_rect(ox+xy, -0.5, 0.1f, 0.1f);
  }

  // RENDER
  sg_pass_action pass_action = {0};
  sg_begin_default_pass(&pass_action, width, height);
  sgp_flush();
  sgp_end();
  sg_end_pass();
  sg_commit();
}

static void init(void) {
  // Initialize Sokol GFX.
  sg_desc sgdesc = {.context = sapp_sgcontext()};
  sg_setup(&sgdesc);
  if(!sg_isvalid()) {
    fprintf(stderr, "Failed to create Sokol GFX context!\n");
    exit(-1);
  }
  
  // Initialize Sokol GP, adjust the size of command buffers for your own use.
  sgp_desc sgpdesc = {0};
  sgp_setup(&sgpdesc);
  if(!sgp_is_valid()) {
    fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
    exit(-1);
  }
}

static void cleanup(void) {
  // Cleanup Sokol GP and Sokol GFX resources.
  sgp_shutdown();
  sg_shutdown();
}

// Implement application main through Sokol APP.
sapp_desc sokol_main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  return (sapp_desc){
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .window_title = "Learning C Graphics Programming",
    .sample_count = 4, // Enable anti aliasing.
  };
}
