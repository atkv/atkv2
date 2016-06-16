#include <at/array.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>

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
  AtArray(uint8_t)* array;                    //  8 bytes - 88B

  print_message("%p: %d, %zu\n",&header   , is_aligned(    &header,alignof(AtArrayHeader)),alignof(AtArrayHeader));
  print_message("%p: %d, %zu\n",data      , is_aligned(       data,alignof(data))      ,alignof(data));
  print_message("%p: %d, %zu\n",data_shape, is_aligned( data_shape,alignof(data_shape)),alignof(data_shape));

  // Array
  array = at_array_uint8_t_new_with_data(2,data_shape,data,true);

  print_message("%p: %d, %zu\n",array     , is_aligned(      array,alignof(array))     ,alignof(array));

  // Testing
  at_array_header_init(&header);
  at_array_header_set (&header, 2, data_shape);
  test_header(&array->header, &header);
  test_array_data(header.num_elements, array->data, data);

  memset(data,0,16);
  at_array_uint8_t_fill(array,0);
  test_array_data(header.num_elements, array->data, data);
  at_array_uint8_t_destroy(&array);
  at_array_header_dispose(&header);
}


void test_array_64(void** state){
  // Variables
  AtArrayHeader     header;                   // 32 bytes - 32B
  uint64_t          data[16]      = {0,0,0,0, // 16 bytes - 48B
                                     0,0,0,1,
                                     0,1,1,1,
                                     1,1,1,1};
  uint64_t          data_shape[2] = {4,4};    // 16 bytes - 64B
  AtArray(uint64_t)* array;                   //  8 bytes - 88B

  // Array
  array = at_array_uint64_t_new_with_data(2,data_shape,data,true);

  // Testing
  at_array_header_init(&header);
  at_array_header_set (&header, 2, data_shape);
  test_header(&array->header, &header);
  test_array_data_64(header.num_elements, array->data, data);

  memset(data,0,16*sizeof(uint64_t));
  at_array_uint64_t_fill(array,0);
  test_array_data_64(header.num_elements, array->data, data);

  at_array_uint64_t_destroy(&array);
  at_array_header_dispose(&header);
}

int main(void){
  print_message("%zu %zu %zu\n", sizeof(AtArray_uint8_t),sizeof(AtArray_uint64_t),sizeof(AtArrayHeader));
  const struct CMUnitTest tests[2] = {
    cmocka_unit_test(test_array),
    cmocka_unit_test(test_array_64)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
