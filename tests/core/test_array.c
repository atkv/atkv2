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

#include <at/core.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>
#include <unistd.h>
#include <time.h>

/*=============================================================================
 HELPERS
 ============================================================================*/

void
test_header(AtArrayHeader* header1, AtArrayHeader* header2){
  uint8_t i;
  assert_int_equal(header1->dim, header2->dim);
  for(i = 0; i < header1->dim; i++){
    assert_int_equal(header1->shape[i]  ,header2->shape[i]);
    assert_int_equal(header1->step[i]   ,header2->step[i]);
  }
  assert_int_equal(header1->nelem, header2->nelem);
  assert_int_equal(header1->owns_data   , header2->owns_data);
}
void
test_array_data(uint64_t nelem, uint8_t* data1, uint8_t* data2){
  uint64_t i;
  for(i = 0; i < nelem; i++)
    assert_int_equal(data1[i], data2[i]);
}
void
test_array_data_64(uint64_t nelem, uint64_t* data1, uint64_t* data2){
  uint64_t i;
  for(i = 0; i < nelem; i++)
    assert_int_equal(data1[i], data2[i]);
}

static inline _Bool is_aligned(const void *restrict pointer, size_t byte_count)
{ return (uintptr_t)pointer % byte_count == 0; }

/*=============================================================================
 TEST CASES
 ============================================================================*/

static void
test_array(void** state){
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
  test_array_data(header.nelem, array->data, data);
  assert_int_equal(at_array_max(array),1);

  memset(data,0,16);
  at_arrayu8_fill(array,0);
  test_array_data(header.nelem, array->data, data);

  at_arrayu8_add_scalar(array,4);
  uint8_t i;
  for(i = 0; i < array->h.nelem; i++) assert_int_equal(array->data[i],4);

  at_arrayu8_fill(array,3);
  for(i = 0; i < array->h.nelem; i++) assert_int_equal(array->data[i],3);

  at_arrayu8_destroy(&array);

  array = at_arrayu8_zeros(2,data_shape);
  for(i = 0; i < array->h.nelem; i++) assert_int_equal(array->data[i],0);
  at_arrayu8_destroy(&array);

  array = at_arrayu8_ones(2,data_shape);
  for(i = 0; i < array->h.nelem; i++) assert_int_equal(array->data[i],1);

  at_array_header_dispose(&header);
}

static void
test_array_save_load(){
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
    for(k = 0; k < arrays[i]->h.nelem; k++)
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
    assert_int_equal(arrayso[i].h.nelem, arrays[i]->h.nelem);
    assert_int_equal(arrayso[i].h.owns_data, arrays[i]->h.owns_data);
    for(k = 0; k < arrays[i]->h.dim; k++){
      assert_int_equal(arrayso[i].h.shape[k], arrays[i]->h.shape[k]);
      assert_int_equal(arrayso[i].h.step[k], arrays[i]->h.step[k]);
    }
    for(k = 0; k < arrays[i]->h.nelem; k++){
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

static void
test_array_64(void** state){
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
  test_array_data_64(header.nelem, array->data, data);

  memset(data,0,16*sizeof(uint64_t));
  at_arrayu64_fill(array,0);
  test_array_data_64(header.nelem, array->data, data);

  at_arrayu64_destroy(&array);
  at_array_header_dispose(&header);
}

static void
test_array_sub(void** state){
  srand ( time(NULL) );
  AtArrayU16* array = NULL, *subar16 = NULL;
  AtArrayU8*  subar = NULL;
  uint64_t    shape[3]  = {3,3,3};
  uint16_t    data[27];
  AtRange     ranges[3] = {AT_RANGE_ALL, AT_RANGE_ALL, AT_RANGE_ALL};
  uint8_t     i;
  uint8_t     val8;
  for(i = 0; i < 27; i++) data[i] = rand();
  array = at_arrayu16_new_with_data(3,shape,data,false);

  // Whole array (u16 => u8)
  at_arrayu16_sub_u8(array,ranges,&subar);
  assert_int_equal(subar->h.dim, array->h.dim);
  assert_int_equal(subar->h.elemsize, 1);
  assert_int_equal(subar->h.owns_data,true);
  assert_int_equal(subar->h.shape[0],array->h.shape[0]);
  assert_int_equal(subar->h.shape[1],array->h.shape[1]);
  assert_int_equal(subar->h.shape[2],array->h.shape[2]);
  at_arrayu8_destroy(&subar);

  // First slice (u16 => u8)
  ranges[0] = at_range_at(0);
  at_arrayu16_sub_u8(array,ranges,&subar);
  assert_int_equal(subar->h.shape[0],1);
  for(i = 0; i < 9; i++){
    val8 = array->data[i];
    assert_int_equal(subar->data[i], val8);
  }
  at_arrayu8_destroy(&subar);

  // 2nd axis (u16 => u8)
  ranges[0] = AT_RANGE_ALL;
  ranges[1] = at_range_at(1);
  at_arrayu16_sub_u8(array,ranges,&subar);
  assert_int_equal(subar->h.shape[0],3);
  assert_int_equal(subar->h.shape[1],1);
  uint64_t k;
  for(i = 0; i < 9; i++){
    k = (i%3)+3 +(i/3)*9;
    val8 = array->data[k];
    assert_int_equal(subar->data[i], val8);
  }
  at_arrayu8_squeeze(subar);
  assert_int_equal(subar->h.dim, 2);
  assert_int_equal(subar->h.shape[0], 3);
  assert_int_equal(subar->h.shape[1], 3);
  for(i = 0; i < 9; i++){
    k = (i%3)+3 +(i/3)*9;
    val8 = array->data[k];
    assert_int_equal(subar->data[i], val8);
    assert_int_equal(at_arrayu8_get_1d(subar,i), val8);
  }
  at_arrayu8_destroy(&subar);

  // Not copy
  at_arrayu16_sub(array,ranges,&subar16,false);
  assert_int_equal(subar16->h.shape[0],3);
  assert_int_equal(subar16->h.shape[1],1);
  uint16_t  val16;
  at_arrayu16_set_1d(subar16,4,22);
  assert_int_equal(array->data[13],22);
  uint64_t nd[3] = {2,0,1};
  at_arrayu16_set_nd(subar16,nd,21);
  assert_int_equal(array->data[22],21);
  for(i = 0; i < 9; i++){
    k = (i%3)+3 +(i/3)*9;
    val16 = array->data[k];
    assert_int_equal(at_arrayu16_get_1d(subar16,i), val16);
  }
  at_arrayu16_squeeze(subar16);
  assert_int_equal(subar16->h.dim, 2);
  assert_int_equal(subar16->h.shape[0],3);
  assert_int_equal(subar16->h.shape[1],3);
  assert_int_equal(at_arrayu16_get_1d(subar16,4),22);
  nd[1] = 1;
  assert_int_equal(at_arrayu16_get_nd(subar16,nd),21);
  at_arrayu16_destroy(&subar16);


  // With copy
  at_arrayu16_sub(array, ranges,&subar16,true);
  at_arrayu16_set_1d(subar16,4,15);
  assert_int_equal(array->data[13],22);
  at_arrayu16_destroy(&subar16);
  at_arrayu16_destroy(&array);
}

int main(void){
  const struct CMUnitTest tests[4] = {
    cmocka_unit_test(test_array),
    cmocka_unit_test(test_array_64),
    cmocka_unit_test(test_array_save_load),
    cmocka_unit_test(test_array_sub),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
