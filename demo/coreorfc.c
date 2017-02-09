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
#include <at/gui.h>
#include <at/imgproc.h>

//#define AT_SAVE

AtImageWindow*   window;

AtMouseEventType type;
uint8_t          pressed;
uint8_t          draw[2];
AtArrayU8*       smask;

// Draw just 1 seed for background or object
void
on_mouse(AtMouseEvent* event, void* user_data){
  AtVec2I16  center;
  AtVec4U8   color[2] = {{127,255,255,255},{254,0,0,255}};
  AtArrayU8* array    = (AtArrayU8*) user_data;

  switch(event->type){
  case AT_MOUSE_LEFT_DOWN:
  case AT_MOUSE_RIGHT_DOWN:
    pressed = true;
    type = event->type;
    center.x = event->x;
    center.y = event->y;
    at_arrayu8_draw_circle(array,center,2,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
    at_arrayu8_draw_circle(smask,center,2,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
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
      at_arrayu8_draw_circle(array,center,2,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
      at_arrayu8_draw_circle(smask,center,2,color[type==AT_MOUSE_LEFT_DOWN],0,AT_ADJACENCY_8,0);
      at_display_show_image(window,array);
    }
    break;
  }
  draw[type==AT_MOUSE_LEFT_DOWN] = 1;
}

// Load an image and find object core for ORFC
int main(int argc, char** argv){
  AtSeeds     * seeds;
  AtSCC       * scc  ;
  AtArrayU8   * smaskrand;
  AtError     * error = NULL;
  uint64_t      i;
  AtKey         k;

  // Load an image and duplicate it (to keep original)
  AtArrayU8   * array    = at_arrayu8_read_png("teste.png",&error);
  if(error){
    fprintf(stderr,error->message);
    fprintf(stderr,"\n");
    exit(EXIT_FAILURE);
  }
  AtArrayU8   * arrayg   = NULL;
  arrayg                 = at_arrayu8_cvt_color(array,AT_RGBA,AT_GRAY, arrayg);
  AtArrayU8   * arraycp  = at_arrayu8_new(array->h.dim, array->h.shape);
  AtArrayU8   * arraygcp = at_arrayu8_new(arrayg->h.dim, arrayg->h.shape);

  // Create our seed mask
  smask = at_arrayu8_new(2, array->h.shape);

  // Create our graph array
  double        alpha    = 0;
  at_wdiffabscalpha.params = &alpha;
  AtGraphArray* g        = at_grapharrayu8_new(arrayg,AT_ADJACENCY_4,at_wdiffabscalpha);

  // Prepare our window
  window = at_display_imagewindow("window");
  at_display_set_mouse_callback(window,on_mouse,arraycp);

#ifdef AT_SAVE
  char*  names[1] = {"mask"};
#else
  char** nameso;
  AtArrayU8* smaskload;
  uint8_t qtd;
#endif
  do{
    at_arrayu8_fill(smask,UINT8_MAX);
    memcpy( arraycp->data, array->data, array->h.nelem* array->h.elemsize);
    memcpy(arraygcp->data,arrayg->data,arrayg->h.nelem*arrayg->h.elemsize);

    // Getting user seeds
#ifdef AT_SAVE
    draw[0] = 0;draw[1] = 0;
    at_display_show_image(window, arraycp);
    at_display_wait_key();
    if(!draw[0] || !draw[1]){
      fprintf(stderr,"You need to mark the background and object seeds\n");
      exit(EXIT_FAILURE);
    }
    at_arrayu8_save(&smask,names,1,"mask.gz");
#else
    smaskload = at_array_load(&nameso,&qtd,"mask.gz");
    memcpy(smask->data, smaskload->data,smask->h.nelem*smask->h.elemsize);
#endif

    // Calculating Cores
    seeds  = at_seeds_from_mask(smask);
    scc    = at_orfc_core_arrayu8(arraygcp, g,at_conn_minr, seeds, 127, AT_FIFO, AT_SCC_TARJAN);

    // Showing number of components
    printf("%d regions\n",scc->n);
    fflush(stdout);

    // Showing components
    for(i = 0; i < smask->h.nelem; i++) smask->data[i] = scc->l[i];
    smaskrand = at_arrayu8_lut_random_multi(smask,3);
    at_display_show_image(window,smaskrand);
    k = at_display_wait_key();

    at_seeds_destroy(&seeds);
    at_scc_destroy(&scc);
    at_arrayu8_destroy(&smaskrand);
  }while(k != AT_KEY_ESCAPE);

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&arrayg);
  at_arrayu8_destroy(&smask);
  return 0;
}
