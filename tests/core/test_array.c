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

#include <at/core/array.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>
#include <unistd.h>

void
test_header(AtArrayHeader* header1, AtArrayHeader* header2){
  uint8_t i;
  assert_int_equal(header1->dim, header2->dim);
  for(i = 0; i < header1->dim; i++){
    assert_int_equal(header1->shape[i]  ,header2->shape[i]);
    assert_int_equal(header1->step[i]   ,header2->step[i]);
  }
  assert_int_equal(header1->num_elements, header2->num_elements);
  assert_int_equal(header1->owns_data   , header2->owns_data);
}
void
test_array_data(uint64_t num_elements, uint8_t* data1, uint8_t* data2){
  uint64_t i;
  for(i = 0; i < num_elements; i++)
    assert_int_equal(data1[i], data2[i]);
}
void
test_array_data_64(uint64_t num_elements, uint64_t* data1, uint64_t* data2){
  uint64_t i;
  for(i = 0; i < num_elements; i++)
    assert_int_equal(data1[i], data2[i]);
}

static inline _Bool is_aligned(const void *restrict pointer, size_t byte_count)
{ return (uintptr_t)pointer % byte_count == 0; }

void test_array(void** state){
  // Variables
  AtArrayHeader     header;                   // 32 bytes - 32B
  uint8_t           data[16]      = {0,0,0,0, // 16 bytes - 48B
                                     0,0,0,1,
                                     0,1,1,1,
                                     1,1,1,1};
  uint64_t          data_shape[2] = {4,4};    // 16 bytes - 64B
  AtArrayU8* array;                    //  8 bytes - 88B

//  print_message("%p: %d, %zu\n",&header   , is_aligned(    &header,alignof(AtArrayHeader)),alignof(AtArrayHeader));
//  print_message("%p: %d, %zu\n",data      , is_aligned(       data,alignof(data))      ,alignof(data));
//  print_message("%p: %d, %zu\n",data_shape, is_aligned( data_shape,alignof(data_shape)),alignof(data_shape));

  // Array
  array = at_arrayu8_new_with_data(2,data_shape,data,true);

//  print_message("%p: %d, %zu\n",array     , is_aligned(      array,alignof(array))     ,alignof(array));

  // Testing
  at_array_header_init(&header);
  at_array_header_set (&header, 2, data_shape);
  test_header(&(array->h), &header);
  test_array_data(header.num_elements, array->data, data);
  assert_int_equal(at_array_max(array),1);

  memset(data,0,16);
  at_arrayu8_fill(array,0);
  test_array_data(header.num_elements, array->data, data);
  at_arrayu8_destroy(&array);
  at_array_header_dispose(&header);
}

void test_array_save_load(){
  char             * names[4]     = {"Anderson", "Carlos","Moreira","Tavares"};
  char            ** nameso;
  AtArrayU16 * arrays[4];
  AtArrayU16 * arrayso;
  uint64_t           shapes[4][3] = {{100,150,0},{100,123,140},{30,42,130},{45,12,0}};
  uint64_t           k;
  uint16_t           dims[4]      = {2,3,3,2};
  char             * filename     = "arrayu16.atz";
  uint8_t            num;
  uint8_t            i;

  // Writing an array to the file
  for(i = 0; i < 4; i++){
    arrays[i] = at_arrayu16_new(dims[i],shapes[i]);
    for(k = 0; k < arrays[i]->h.num_elements; k++)
      arrays[i]->data[k] = rand();
  }
  unlink(filename);
  at_array_save(arrays,names,4,filename);
  assert_int_not_equal(access(filename, F_OK),-1);

  // Reading the file
  arrayso = (AtArrayU16*) at_array_load(&nameso,&num,filename);
  for(i = 0; i < 4; i++){
    assert_string_equal(names[i],nameso[i]);
    assert_int_equal(arrayso[i].h.dim, arrays[i]->h.dim);
    assert_int_equal(arrayso[i].h.num_elements, arrays[i]->h.num_elements);
    assert_int_equal(arrayso[i].h.owns_data, arrays[i]->h.owns_data);
    for(k = 0; k < arrays[i]->h.dim; k++){
      assert_int_equal(arrayso[i].h.shape[k], arrays[i]->h.shape[k]);
      assert_int_equal(arrayso[i].h.step[k], arrays[i]->h.step[k]);
    }
    for(k = 0; k < arrays[i]->h.num_elements; k++){
      assert_int_equal(arrayso[i].data[k], arrays[i]->data[k]);
    }
    at_arrayu16_destroy(&arrays[i]);
    free(arrayso[i].h.shape);
    free(arrayso[i].h.step);
    free(arrayso[i].data);
    free(nameso[i]);
  }
  free(arrayso);
  free(nameso);
}


void test_array_64(void** state){
  // Variables
  AtArrayHeader     header;                   // 32 bytes - 32B
  uint64_t          data[16]      = {0,0,0,0, // 16 bytes - 48B
                                     0,0,0,1,
                                     0,1,1,1,
                                     1,1,1,1};
  uint64_t          data_shape[2] = {4,4};    // 16 bytes - 64B
  AtArrayU64* array;                   //  8 bytes - 88B

  // Array
  array = at_arrayu64_new_with_data(2,data_shape,data,true);

  // Testing
  at_array_header_init(&header);
  at_array_header_set (&header, 2, data_shape);
  test_header(&array->h, &header);
  test_array_data_64(header.num_elements, array->data, data);

  memset(data,0,16*sizeof(uint64_t));
  at_arrayu64_fill(array,0);
  test_array_data_64(header.num_elements, array->data, data);

  at_arrayu64_destroy(&array);
  at_array_header_dispose(&header);
}

int main(void){
  const struct CMUnitTest tests[3] = {
    cmocka_unit_test(test_array),
    cmocka_unit_test(test_array_64),
    cmocka_unit_test(test_array_save_load)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
