#include <at/gui/display.h>
#include <at/imgproc/img_io.h>
#include <at/imgproc/color.h>
#include <at/imgproc/drawing.h>
#include <at/imgproc/ift.h>

AtImageWindow*   window;

AtMouseEventType type;
uint8_t          pressed;
AtArrayU8*       smask;

// Draw just 1 seed for background or object
void
on_mouse(AtMouseEvent* event, void* user_data){
  AtVec2I16 center;
  AtVec4U8  color[2] = {{127,255,255,255},{255,0,0,255}};
  AtArrayU8* array = (AtArrayU8*) user_data;

  switch(event->type){
  case AT_MOUSE_LEFT_DOWN:
  case AT_MOUSE_RIGHT_DOWN:
    pressed = true;
    type = event->type;
    center.x = event->x;
    center.y = event->y;
    at_arrayu8_draw_circle(array,center,0,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
    at_arrayu8_draw_circle(smask,center,0,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
    at_display_show_image(window,array);
    break;
  case AT_MOUSE_LEFT_UP:
  case AT_MOUSE_RIGHT_UP:
    pressed = false;
    break;
  case AT_MOUSE_MOVE:
    if(pressed){
      center.x = event->x;
      center.y = event->y;
      at_arrayu8_draw_circle(array,center,0,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
      at_arrayu8_draw_circle(smask,center,0,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
      at_display_show_image(window,array);
    }
    break;
  }
}

// Load an image and find object core for ORFC
int main(int argc, char** argv){
  AtArrayU8* array = at_arrayu8_read_png("teste.png",NULL);
  AtArrayU8* arrayg= at_arrayu8_cvt_color(array,AT_RGBA,AT_GRAY);
  smask = at_arrayu8_zeros(2, array->h.shape);

  window = at_display_show_image_by_name("window", array);
  at_display_set_mouse_callback(window,on_mouse,array);
  at_display_wait_key();

  AtArrayU64* seeds = at_seeds_from_mask(smask);
  AtSCC* scc = at_ift_orfc_core_arrayu8(arrayg, AT_ADJACENCY_4, AT_MINIMIZATION, at_conn_max, at_weighting_diff_abs, seeds, 127, AT_FIFO, AT_SCC_TARJAN);
  uint64_t i;
  for(i = 0; i < smask->h.num_elements; i++) smask->data[i] = scc->l[i];
  at_display_show_image(window,smask);
  at_display_wait_key();

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&arrayg);
  return 0;
}
