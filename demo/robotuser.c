#include <stdio.h>
#include <at/imgproc.h>
#include <at/gui.h>
#include <string.h>

int main(int argc, char *argv[])
{
  // Get the image
  AtError* error = NULL;
  AtArrayU8* image = at_arrayu8_read_png("flower.png", &error);
  if(error){
    fprintf(stderr, "%s\n", error->message);
    return EXIT_FAILURE;
  }
  // Converto to grayscale
  AtArrayU8* gray = at_arrayu8_cvt_color(image, AT_RGB, AT_GRAY, NULL);

  // Display the image
  AtImageWindow* window;//   = at_display_show_image_by_name("image", image);
  AtKey          key   ;//   = at_display_wait_key();

  // Get the mask
  AtArrayU8    * mask_rgb = at_arrayu8_read_png("mask.png", &error);
  printf("%d %d %d\n", mask_rgb->h.shape[0],mask_rgb->h.shape[1],mask_rgb->h.shape[2]);

  // Original mask is rgb (convert it to grayscale)
  AtArrayU8    * mask     = at_arrayu8_cvt_color(mask_rgb,AT_RGBA,AT_GRAY, NULL);

  // Display it
//  at_display_show_image_by_name("image", mask);
//  at_display_wait_key();

  // Convert mask intensities to classes for robotuser processing
  uint64_t i, j;
  for(i =0 ; i < mask->h.nelem; i++){
    if(mask->data[i] == 255) mask->data[i] = 1;
    else mask->data[i] = 2;
  }

  // Create a robotuser for a primal fmax IFT
  AtRobotUserU8* robot = at_robotuseru8_new_from_array(gray, mask, 255, AT_ADJACENCY_4, at_wdiffabs, AT_FIFO, at_conn_max);
  AtArrayU8* labels = NULL;
  AtArrayU8* labels_rgb = NULL;

  uint8_t color_fp[4] = {255,0,0,255};
  uint8_t color_fn[4] = {255,255,0,255};
  uint8_t color_tp[4] = {0,255,255,255};
  uint8_t color_tn[4] = {255,255,255,255};

  // Iterate through robotuser and add seeds
  for(j = 0; !robot->finished; j++){
    // Process robouser
    at_robotuseru8_next(robot);

    // Get resulting labels of robotuser and display them
    if(!labels)labels = at_arrayu8_new_with_data(gray->h.dim, gray->h.shape, robot->ift->l, false);

    // Convert to rgb
    labels_rgb = at_arrayu8_cvt_color(labels,AT_GRAY, AT_RGBA, labels_rgb);



    uint64_t irgb;
    for(i =0 ; i < mask->h.nelem; i++){
      irgb = i << 2;
      if(labels->data[i] == 1){
        if(mask->data[i] == 2) at_arrayu8_set_1d_many(labels_rgb,irgb,color_fp);
        else                   at_arrayu8_set_1d_many(labels_rgb,irgb,color_tp);
      } else {
        if(mask->data[i] == 1) at_arrayu8_set_1d_many(labels_rgb,irgb,color_fn);
        else                   at_arrayu8_set_1d_many(labels_rgb,irgb,color_tn);
      }
    }

//    for(i =0 ; i < mask->h.nelem; i++){
//      if(labels->data[i] == 1){
//        if(mask->data[i] == 2) labels->data[i] = 0;
//        else                   labels->data[i] = 100;
//      } else {
//        if(mask->data[i] == 1) labels->data[i] = 200;
//        else                   labels->data[i] = 255;
//      }
//    }

    // Display wrong regions


    // Display seeds
    AtVec2I16 center;
    AtVec4U8 colors[2] = {{255,0,0,255},{0,0,255,255}};
    printf("Obtendo os centros das sementes\n");
    printf("%d %d\n", mask->h.shape[0], mask->h.shape[1]);
    for(i = 0; i < j+2; i++){
      printf("Etapa %d\n", i);
      center.y = robot->seeds->s[i]/mask->h.shape[1];
      center.x = robot->seeds->s[i]%mask->h.shape[1];
      printf("%d %d %d %d\n", robot->seeds->s[i], center.x, center.y, robot->seeds->n);
      fflush(stdout);
      at_arrayu8_draw_circle(labels_rgb, center, 8, colors[robot->seeds->l[i]-1], 0, AT_ADJACENCY_4, 0);
    }
    char name[16];
    sprintf(name,"robotuser%02d.png", j);
    at_arrayu8_write_png(labels_rgb,name,&error);
    at_display_show_image_by_name("image", labels_rgb);
    key = at_display_wait_key();
  }
  // Destroy rgb image (for not leaking memory)
  at_arrayu8_destroy(&labels_rgb);

//  at_robotuseru8_destroy(&robot);
  at_arrayu8_destroy(&image);
  at_arrayu8_destroy(&gray);
  at_arrayu8_destroy(&labels);
  at_arrayu8_destroy(&mask);
  at_arrayu8_destroy(&mask_rgb);
  return 0;
}
