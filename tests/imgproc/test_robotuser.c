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

#include <at/imgproc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <time.h>

#define MAXITER 6
#define NELEM 100


static void at_arrayu16_print(AtArrayU16* array){
  uint64_t k, y, x;
  for(y = 0, k=0; y < array->h.shape[0]; y++){
    for(x = 0; x < array->h.shape[1]; x++,k++){
      printf("%3d ", array->data[k]);
    }
    printf("\n");
  }
  fflush(stdout);
}

static void at_arrayu8_print(AtArrayU8* array){
  uint64_t k, y, x;
  for(y = 0, k=0; y < array->h.shape[0]; y++){
    for(x = 0; x < array->h.shape[1]; x++,k++){
      printf("%3d ", array->data[k]);
    }
    printf("\n");
  }
  fflush(stdout);
}

static void uint8_t_print(uint8_t* data, uint64_t width, uint64_t height){
  uint64_t k, y, x;
  for(y = 0, k=0; y < height; y++){
    for(x = 0; x < width; x++,k++){
      printf("%3d ", data[k]);
    }
    printf("\n");
  }
  fflush(stdout);
}

static void
test_robotuser(void** state){
  uint64_t    shape[2] = {10,10};
  AtArrayU8  *array, *maskar;

  uint8_t     data[NELEM]={  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                             0,  0,128,130, 30, 15,  0,  0,  0,  0,
                             0,  0,132, 80, 87, 43,  0,  0,  0,  0,
                             0,  0,110,108,  0,  0,  0,  0,  0,  0,
                             0,  0,90 , 32,  0,  0,  0,  0,  0,  0,
                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
  uint8_t     mask[NELEM]={  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  2,  2,  2,  2,  1,  1,  1,  1,
                             1,  1,  2,  2,  2,  2,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                             1,  1,  1,  1,  1,  1,  1,  1,  1,  1};


  array = at_arrayu8_new_with_data(2, shape, data, false);
  maskar = at_arrayu8_new_with_data(2, shape, mask, false);
  AtRobotUserU8* robot = at_robotuseru8_new_from_array(array, maskar, MAXITER<<1,AT_ADJACENCY_4, at_wdiffabs, AT_FIFO, at_conn_max);

  while(!robot->finished){
    at_robotuseru8_next(robot);
  }

  assert_true(robot->finished);
  assert_int_equal(robot->nclasses, 2);
  assert_int_equal(robot->step, 4);
  assert_non_null(robot->array);
  assert_non_null(robot->mask);
  assert_non_null(robot->g);
  assert_non_null(robot->ift);
  assert_non_null(robot->seeds);

  at_arrayu8_destroy(&array);
  at_arrayu8_destroy(&maskar);
}

int main(void){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_robotuser)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
