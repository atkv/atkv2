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

#include <at/core/grapharray.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>
#include <math.h>

void
test_grapharray_values(AtGraphArray* graph1, AtGraphArray* graph2, uint64_t num_elements){
  uint64_t i;
  double delta = 1e-5;
  for(i = 0; i < num_elements; i++){
    assert_int_equal(graph1->active[i]   , graph2->active[i]);
    assert_int_equal(graph1->neighbors[i], graph2->neighbors[i]);
    assert_true(fabs(graph1->weights[i]  - graph2->weights[i]) < delta);
  }
}
void
test_grapharray(void** state){
  uint64_t          neighbors[16] = {0,1,0,2,0,0,0,3,  // 000-128
                                     0,3,0,0,2,0,1,0};
  double            weights[16]   = {0,1,0,1,1,0,0,0,  // 128-128
                                     0,0,1,0,0,0,0,0};
  uint64_t          data_shape[2] = {2,2};             // 256-16
  AtArray(uint8_t) *array;                             // 272-8
  AtGraphArray     *grapharray;                        // 280-8
  uint8_t           active[16]    = {0,1,0,1,1,0,0,1,  // 288-16
                                     0,1,1,0,1,0,1,0};
  uint8_t           data[4]       = {0,1,1,1};         // 304-4

  array      = at_array_uint8_t_new_with_data(2,data_shape,data,true);
  grapharray = at_grapharray_uint8_t_new(array,AT_ADJACENCY_4,at_weighting_diff_abs);
  assert_non_null(grapharray);
  assert_non_null(grapharray->active);
  assert_non_null(grapharray->neighbors);
  assert_non_null(grapharray->weights);

  AtGraphArray grapharray_t;
  grapharray_t.active    = active;
  grapharray_t.weights   = weights;
  grapharray_t.neighbors = neighbors;

  test_grapharray_values(grapharray, &grapharray_t, array->h.num_elements * AT_ADJACENCY_4);

  at_grapharray_destroy(&grapharray);
  at_array_uint8_t_destroy(&array);
}

int main(void){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_grapharray)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}