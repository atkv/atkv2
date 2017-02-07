/**
 ** This file is part of the atkv project.
 ** Copyright 2016-2017 Anderson Tavares <nocturne.pe@gmail.com>.
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

/**
 * @file This is a demo of showing an image by using @Display API.
 *
 * You just need to load the image (PNG, JPG, PGM, PPM or based on raw data)
 * and call `at_display_show_image_by_name()` to create the window and
 * `at_display_wait_key()` to enter a loop waiting the user interaction
 */

#include <at/gui.h>     // for GUI and Image processing

#define BACKGROUND 0
#define OBJECT     255
AtImageWindow  * window;
uint8_t          pressed;
AtArrayU8      * seedsmask;
AtVec4U8         color[2] = {{127, 0, 255, 255},{254, 0, 0, 255}};
AtMouseEventType mbutton;
AtVec2I16        p0, p1;

static void
on_mouse(AtMouseEvent* ev, void* user_data){
  AtArrayU8* array = (AtArrayU8*) user_data;
  AtVec2I16  center= {ev->x, ev->y};
  switch(ev->type){
  case AT_MOUSE_LEFT_DOWN:
  case AT_MOUSE_RIGHT_DOWN:
    pressed = true;
    mbutton = ev->type;
    p1.x = ev->x;
    p1.y = ev->y;
    at_arrayu8_draw_circle(array    ,center, 3, color[mbutton == AT_MOUSE_LEFT_DOWN], 0, AT_ADJACENCY_4, 0);
    at_arrayu8_draw_circle(seedsmask,center, 3, color[mbutton == AT_MOUSE_LEFT_DOWN], 0, AT_ADJACENCY_4, 0);
    at_display_show_image (window, array);
    break;
  case AT_MOUSE_LEFT_UP:
  case AT_MOUSE_RIGHT_UP:
    pressed = false;
    break;
  case AT_MOUSE_MOVE:
    if(pressed){
      p0    = p1;
      p1    = center;
      at_arrayu8_draw_circle(array    ,center, 3, color[mbutton == AT_MOUSE_LEFT_DOWN], 0, AT_ADJACENCY_4, 0);
      at_arrayu8_draw_circle(seedsmask,center, 3, color[mbutton == AT_MOUSE_LEFT_DOWN], 0, AT_ADJACENCY_4, 0);
      at_arrayu8_draw_line  (array    ,p0,p1,color[mbutton == AT_MOUSE_LEFT_DOWN],7,AT_ADJACENCY_8,0);
      at_arrayu8_draw_line  (seedsmask,p0,p1,color[mbutton == AT_MOUSE_LEFT_DOWN],7,AT_ADJACENCY_8,0);
      at_display_show_image (window, array);

    }
    break;
  }
}

int main(){
  // Read PNG
  AtError     * error    = NULL;
  //AtArrayU8 * array    = at_arrayu8_read_jpg("teste.jpg",&error);
  //AtArrayU8 * arrayg   = at_arrayu8_cvt_color(array,AT_RGB, AT_GRAY);
  AtArrayU8   * array    = at_arrayu8_read_png("teste.png",&error);
  AtArrayU8   * arrayg   = at_arrayu8_cvt_color(array, AT_RGBA, AT_GRAY);
  AtSeeds     * seeds;
  AtArrayU8   * labels   = at_arrayu8_new(2, array->h.shape);
  AtIFT       * ift;
  AtGraphArray* g        = at_grapharrayu8_new(arrayg,AT_ADJACENCY_4,at_wdiffabs);
  seedsmask = at_arrayu8_new(2, array->h.shape);
  at_arrayu8_fill(seedsmask,UINT8_MAX);
  if(error){
    fprintf(stderr, error->message);
    return EXIT_FAILURE;
  }

  // Show Image
  window = at_display_show_image_by_name("window1",array);
  at_display_set_mouse_callback(window, on_mouse, array);
  at_display_wait_key();

  // Appyling IFT
  seeds  = at_seeds_from_mask(seedsmask);
  clock_t start = clock(), diff;
  ift    = at_ift_apply_arrayu8(arrayg,g,at_conn_max,seeds,AT_FIFO);
  diff = clock() - start;
  int msec = diff * 1000 / CLOCKS_PER_SEC;
  printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

  // Showing Results
  memcpy(labels->data, ift->l, labels->h.nelem*labels->h.elemsize);
  at_display_show_image(window,labels);
  at_display_wait_key();

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&arrayg);
  at_arrayu8_destroy(&seedsmask);
  at_arrayu8_destroy(&labels);
  free(ift);
  return EXIT_SUCCESS;
}
