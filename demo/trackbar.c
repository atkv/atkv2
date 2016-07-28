#include <at/gui.h>

AtArrayU8    * copy;
AtArrayU8    * img;
AtImageWindow* window;
double         increment;

static void
trackbar_changed(double pos){
  uint64_t i, j;
  uint8_t c;
  memcpy(copy->data,img->data,copy->h.num_elements);
  at_arrayu8_add_brightness(copy,pos);
  at_display_show_image(window,copy);
}

int main(){
  AtTrackbar   * track;
  img    = at_arrayu8_read_png("teste.png",NULL);
  copy   = at_arrayu8_new(img->h.dim, img->h.shape);
  window = at_display_imagewindow("image");
  track  = at_display_add_trackbar(window,"Increment",&increment,-255,254);
  at_trackbar_on_change(track,trackbar_changed);
  at_display_show_image(window,img);
  at_display_wait_key();
  at_arrayu8_destroy(&img);
  at_arrayu8_destroy(&copy);
  return 0;
}
