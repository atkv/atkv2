#include <at/imgproc.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
void
at_arrayu8_print(AtArrayU8* array){
  uint8_t i, j, k;
  for(i = 0, k = 0; i < array->h.shape[0]; i++){
    for(j = 0; j < array->h.shape[1]; j++, k++){
      print_message("%4u", array->data[k]);
      if(j != array->h.shape[1]-1) print_message(" ");
    }
    if(i != array->h.shape[0]-1) print_message("\n");
  }
  print_message("\n");
}

static void
test_drawing_line(void** state){
  uint64_t   shape[2] = {4,4};
  AtArrayU8* array    = at_arrayu8_new(2,shape);
  AtVec2I16  p0       = {0,0}, p1 = {3,3};
  AtVec4U8   color    = {1,1,1,1};
  uint8_t    data8[16]= {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
  uint8_t    data4[16]= {1,1,0,0, 0,1,1,0, 0,0,1,1, 0,0,0,1};
  uint8_t    i;

  at_arrayu8_fill(array,0);
  at_arrayu8_draw_line(array,p0,p1,color,1,AT_ADJACENCY_8,0);
  for(i = 0; i < 16; i++) assert_int_equal(data8[i],array->data[i]);
  at_arrayu8_fill(array,0);
  at_arrayu8_draw_line(array,p0,p1,color,1,AT_ADJACENCY_4,0);
  for(i = 0; i < 16; i++) assert_int_equal(data4[i],array->data[i]);
  at_arrayu8_destroy(&array);
}

static void
test_drawing_circle(void** state){
  uint64_t   shape[2]  = {5,5};
  AtArrayU8* array     = at_arrayu8_new(2,shape);
  AtVec2I16  c         = {2,2};
  AtVec4U8   color     = {255,255,0,255};
  uint8_t    data8[25] = {0,255,255,255,0, 255,0,0,0,255, 255,0,0,0,255, 255,0,0,0,255, 0,255,255,255,0};
  uint8_t    data8f[25]= {0,255,255,255,0, 255,255,255,255,255, 255,255,255,255,255, 255,255,255,255,255, 0,255,255,255,0};
  uint8_t    i;
  at_arrayu8_fill(array,0);
  at_arrayu8_draw_circle(array, c, 2, color, 1, AT_ADJACENCY_8,0);
  for(i = 0; i < 25; i++) assert_int_equal(data8[i], array->data[i]);

  at_arrayu8_fill(array,0);
  at_arrayu8_draw_circle(array, c, 2, color, 0, AT_ADJACENCY_8,0);
  for(i = 0; i < 25; i++) assert_int_equal(data8f[i], array->data[i]);

  at_arrayu8_destroy(&array);
}

static void
test_drawing_rectangle(void** state){
  uint64_t   shape[2] = {5,5}, i;
  AtArrayU8* array    = at_arrayu8_new(2, shape);
  AtVec4U8   color    = {255,255,  0,255};
  AtVec4U16  rect     = {1,1, 3,3};
  uint8_t    data8[25]= {0,0,0,0,0, 0,255,255,255,0, 0,255,0,255,0, 0,255,255,255,0, 0,0,0,0,0};
  at_arrayu8_fill(array,0);
  at_arrayu8_draw_rectangle(array,rect,color,1,AT_ADJACENCY_4,0);
  for(i = 0; i < 25; i++) assert_int_equal(data8[i], array->data[i]);

  at_arrayu8_destroy(&array);
}

int main(int argc, char** argv){
  const struct CMUnitTest tests[3]={
    cmocka_unit_test(test_drawing_line),
    cmocka_unit_test(test_drawing_circle),
    cmocka_unit_test(test_drawing_rectangle),
  };
  uint64_t shape[3] = {200,200, 3};
  AtArrayU8* array  = at_arrayu8_new(3,shape);
  AtVec4U8   color  = {255,255,0,255};
  AtVec2I16  p0     = {20,10}, p1 = {190,102};
  at_arrayu8_draw_line(array,p0, p1, color, 3, AT_ADJACENCY_8, 0);
  at_arrayu8_write_png(array, "line_thick.png", NULL);
  return cmocka_run_group_tests(tests,NULL,NULL);
}
