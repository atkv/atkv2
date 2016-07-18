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

#include <at/imgproc/color.h>
#include <at/imgproc/img_io.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

void
test_lut_array(void** state){
  AtArrayU8* array, *arraylut;
  uint64_t   shape[2]  = {4,4};
  uint8_t    data[16]  = {0,1,2,0, 0,1,1,2, 0,1,2,2, 2,1,0,2};
  uint8_t    colors[3] = {0,0,0};
  uint8_t    i;
  array    = at_arrayu8_new_with_data(2,shape,data,false);
  arraylut = at_arrayu8_lut_random(array);

  for(i = 0; i < 16; i++){
    if(colors[array->data[i]] == 0) colors[array->data[i]] = arraylut->data[i];
    assert_int_equal(arraylut->data[i], colors[array->data[i]]);
  }

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&arraylut);
}
void
test_lut_array_rgb(void** state){
  AtArrayU8* array, *arraylut;
  uint64_t   shape[2]  = {4,4};
  uint8_t    data[16]  = {0,1,2,0, 0,1,1,2, 0,1,2,2, 2,1,0,2};
  uint8_t    colors[9] = {0,0,0};
  uint8_t    i,j;
  array    = at_arrayu8_new_with_data(2,shape,data,false);
  arraylut = at_arrayu8_lut_random_multi(array,3);

  for(i = 0; i < 16; i++){
    if(colors[array->data[i]*3] == 0) memcpy(&colors[array->data[i]*3], &arraylut->data[i*3],3);
    for(j = 0; j < 3; j++)
      assert_int_equal(arraylut->data[i*3+j], colors[array->data[i]*3+j]);
  }
  at_arrayu8_write_png(arraylut,"test_lut_array_rgb.png",NULL);

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&arraylut);
}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_lut_array),
    cmocka_unit_test(test_lut_array_rgb),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
