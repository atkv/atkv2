/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
