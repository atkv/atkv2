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

#include <at/pqueue.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>
#include <math.h>

void test_pqueue(void** state){
  AtPQueue_uint64_t* queue = at_pqueue_uint64_t_new_prealloc(AT_MINIMIZATION,AT_FIFO,10,6);
  uint64_t  priorities[5]  = {9,8,7,6,5};
  uint64_t  values[5]      = {1,2,3,4,5};
  uint64_t  values_r[5]    = {5,4,3,2,1};
  uint64_t  s;
  uint8_t   i;

  for(i = 0; i < 5; i++)
    at_pqueue_uint64_t_add(queue, priorities[i], values[i]);
  for(i = 0; i < 5; i++){
    s = at_pqueue_uint64_t_remove(queue);
    assert_int_equal(s, values_r[i]);
  }
}

int main(void){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_pqueue)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
