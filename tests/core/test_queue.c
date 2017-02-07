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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <at/core.h>

static void helper_test_values(AtQueue* queue, uint8_t* values, uint8_t qty){
  AtList*   current;
  uint8_t i;

  assert_int_equal(queue->length, qty);
  for(i = 0, current=queue->begin;i < qty; i++, current=at_list_next(current)){
    assert_non_null(current);
    assert_int_equal(at_list_value(current), values[i]);
  }
  for(i = 0, current=queue->end  ;i < qty; i++, current=at_list_prev(current)){
    assert_non_null(current);
    assert_int_equal(at_list_value(current), values[qty-1-i]);
  }
}

int8_t int_compare(void* value1, void* value2){
  if(value1 < value2) return -1;
  else if(value1 == value2) return 0;
  return 1;
}
int8_t int_sum_compare(void* value1, void* value2, void* data){
  int value1_int = POINTER_TO_INT32(value1);
  int value2_int = POINTER_TO_INT32(value2);
  int data_int = POINTER_TO_INT32(data);
  if(value1_int+data_int < value2_int+data_int) return -1;
  else if(value1_int+data_int == value2_int+data_int) return 0;
  return 1;
}

static void test_at_queue_adding_removing(void** state){
  (void) state;
  AtList* item, *item2;
  void* value;
  AtQueue* queue = at_queue_new();
  assert_non_null(queue);
  assert_int_equal(queue->length,0);
  assert_null(queue->begin);
  assert_null(queue->end);
  at_queue_free(queue);

  queue = at_queue_new();
  at_queue_prepend(queue, INT8_TO_POINTER(5)); // 5
  assert_int_equal(queue->length, 1);
  assert_non_null(queue->begin);
  assert_non_null(queue->end);
  assert_int_equal(queue->begin, queue->end);
  assert_int_equal(at_list_value(queue->begin), 5);

  at_queue_append(queue, INT8_TO_POINTER(3)); // 5 3
  assert_int_equal(queue->length, 2);
  assert_non_null(queue->begin);
  assert_non_null(queue->end);
  assert_int_not_equal(queue->begin, queue->end);
  assert_int_equal(at_list_value(queue->begin), 5);
  assert_int_equal(at_list_value(queue->end), 3);

  print_message("\nat_queue_prepend\n");
  at_queue_prepend(queue, INT8_TO_POINTER(2)); // 2 5 3
  uint8_t values1[3] = {2,5,3};
  helper_test_values(queue, values1, 3);

  print_message("\nat_queue_prepend_at\n");
  at_queue_prepend_at(queue, at_list_next(queue->begin), INT8_TO_POINTER(4)); // 2 4 5 3
  uint8_t values[4] = {2,4,5,3};
  helper_test_values(queue, values, 4);

  print_message("\nat_queue_append_at\n");
  at_queue_append_at(queue, queue->end->prev, INT8_TO_POINTER(7)); // 2 4 5 7 3
  uint8_t values2[5] = {2,4,5,7,3};
  helper_test_values(queue, values2, 5);

  print_message("\nat_queue_prepend_at_index\n");
  at_queue_prepend_at_index(queue, 2, INT8_TO_POINTER(8)); // 2 4 8 5 7 3
  uint8_t values3[6] = {2,4,8,5,7,3};
  helper_test_values(queue, values3, 6);

  print_message("\nat_queue_append_at_index\n");
  at_queue_append_at_index(queue, 2, INT8_TO_POINTER(6)); // 2 4 8 6 5 7 3
  uint8_t values4[7] = {2,4,8,6,5,7,3};
  helper_test_values(queue, values4, 7);

  print_message("\nat_queue_prepend_item\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(9));
  at_queue_prepend_item(queue, item); // 9 2 4 8 6 5 7 3
  uint8_t values5[8] = {9,2,4,8,6,5,7,3};
  helper_test_values(queue, values5, 8);

  print_message("\nat_queue_append_item\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(10));
  at_queue_append_item(queue, item); // 9 2 4 8 6 5 7 3 10
  uint8_t values6[9] = {9,2,4,8,6,5,7,3,10};
  helper_test_values(queue, values6, 9);

  print_message("\nat_queue_prepend_item_at\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(11));
  item2 = at_list_at(queue->begin,5);
  at_queue_prepend_item_at(queue, item2, item); // 9 2 4 8 6 11 5 7 3 10
  uint8_t values7[10] = {9,2,4,8,6,11,5,7,3,10};
  helper_test_values(queue, values7, 10);

  print_message("\nat_queue_append_item_at\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(12));
  item2 = at_list_at(queue->begin,3);
  at_queue_append_item_at(queue, item2, item); // 9 2 4 8 12 6 11 5 7 3 10
  uint8_t values8[11] = {9,2,4,8,12,6,11,5,7,3,10};
  helper_test_values(queue, values8, 11);

  print_message("\nat_queue_prepend_item_at_index\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(13));
  at_queue_prepend_item_at_index(queue, 7, item); // 9 2 4 8 12 6 11 13 5 7 3 10
  uint8_t values9[12] = {9,2,4,8,12,6,11,13,5,7,3,10};
  helper_test_values(queue, values9, 12);

  print_message("\nat_queue_append_item_at_index\n");
  item = at_list_prepend(NULL, INT8_TO_POINTER(14));
  at_queue_append_item_at_index(queue, 2, item); // 9 2 4 14 8 12 6 11 13 5 7 3 10
  uint8_t values10[13] = {9,2,4,14,8,12,6,11,13,5,7,3,10};
  helper_test_values(queue, values10, 13);

  print_message("\nat_queue_remove\n");
  at_queue_remove(queue, INT8_TO_POINTER(8));
  uint8_t values11[12] = {9,2,4,14,12,6,11,13,5,7,3,10};
  helper_test_values(queue, values11, 12);

  print_message("\nat_queue_remove_at\n");
  value = at_queue_remove_at(queue, 3);
  assert_int_equal(value, 14);
  uint8_t values12[11] = {9,2,4,12,6,11,13,5,7,3,10};
  helper_test_values(queue, values12, 11);

  print_message("\nat_queue_remove_begin\n");
  value = at_queue_remove_begin(queue);
  assert_int_equal(value, 9);
  uint8_t values13[10] = {2,4,12,6,11,13,5,7,3,10};
  helper_test_values(queue, values13, 10);

  print_message("\nat_queue_remove_end\n");
  value = at_queue_remove_end(queue);
  assert_int_equal(value, 10);
  uint8_t values14[9] = {2,4,12,6,11,13,5,7,3};
  helper_test_values(queue, values14, 9);
  at_queue_free(queue);

  print_message("\nat_queue_prepend_sorted\n");
  queue = at_queue_new();
  uint8_t values15[4] = {12,20,32,50};
  uint8_t values16[4] = {32,12,50,20};
  uint8_t i;
  for(i = 0; i < 4; i++) at_queue_prepend_sorted(queue, int_compare, INT8_TO_POINTER(values16[i]));
  helper_test_values(queue, values15, 4);
  at_queue_free(queue);

  print_message("\nat_queue_append_sorted\n");
  queue = at_queue_new();
  for(i = 0; i < 4; i++) at_queue_append_sorted(queue, int_compare, INT8_TO_POINTER(values16[i]));
  helper_test_values(queue, values15, 4);
  at_queue_free(queue);

  print_message("\nat_queue_append_sorted_with_data\n");
  queue = at_queue_new();
  for(i = 0; i < 4; i++) at_queue_append_sorted_with_data(queue, int_sum_compare, INT8_TO_POINTER(values16[i]), INT8_TO_POINTER(10));
  helper_test_values(queue, values15, 4);
  at_queue_free(queue);

  print_message("\nat_queue_prepend_sorted_with_data\n");
  queue = at_queue_new();
  for(i = 0; i < 4; i++) at_queue_prepend_sorted_with_data(queue, int_sum_compare, INT8_TO_POINTER(values16[i]), INT8_TO_POINTER(10));
  helper_test_values(queue, values15, 4);
  at_queue_free(queue);
}

static void test_at_queue_accessors(void** state){
  (void) state;
  uint8_t i;
  AtList* item;
  void* value;
#define NUM_NAMES 4
  char* values[NUM_NAMES] = {"Anderson","Carlos","Moreira","Tavares"};

  AtQueue* queue = at_queue_new();
  assert_int_equal(at_queue_is_empty(queue),1);
  for(i = 0; i < NUM_NAMES; i++) at_queue_prepend(queue,values[i]);
  assert_int_equal(at_queue_is_empty(queue),0);

  item = at_queue_begin(queue);
  assert_non_null(item);
  assert_string_equal(at_list_value(item), "Tavares");

  item = at_queue_end(queue);
  assert_non_null(item);
  assert_string_equal(at_list_value(item), "Anderson");

  assert_int_equal(at_queue_index_of(queue,"Moreira"), 1);
  assert_int_equal(at_queue_index_of(queue,"Carlos"), 2);

  item = at_queue_at(queue,2);
  assert_non_null(item);
  assert_string_equal(at_list_value(item),"Carlos");

  item = at_queue_at(queue,0);
  assert_non_null(item);
  assert_string_equal(at_list_value(item),"Tavares");

  value = at_queue_value_at(queue, 3);
  assert_string_equal(value, "Anderson");

  value = at_queue_begin_value(queue);
  assert_string_equal(value, "Tavares");

  value = at_queue_end_value(queue);
  assert_string_equal(value, "Anderson");

  assert_int_equal(at_queue_length(queue), 4);

  at_queue_free(queue);
#undef NUM_NAMES
}

void add_prefix(void* item, void* prefix){
  AtList* at_list_item = (AtList*) item;
  char* text       = (char*) at_list_value(at_list_item);
  char* prefix_str = (char*) prefix;

  size_t text_length   = strlen(text);
  size_t prefix_length = strlen(prefix_str);

  char* str = malloc(prefix_length + text_length + 1);
  strcpy(str, prefix);
  strcpy(str+prefix_length, text);
  str[prefix_length+text_length] = '\0';

  at_list_set_value(at_list_item,str);
}

static void test_at_queue_operations(void** state){
  (void) state;

  uint8_t i;
#define NUM_NAMES 4
  char* values[NUM_NAMES] = {"Anderson","Carlos","Moreira","Tavares"};
  char* values2[NUM_NAMES] = {"prevAnderson","prevCarlos","prevMoreira","prevTavares"};

  AtQueue* queue = at_queue_new();
  for(i = 0; i < NUM_NAMES; i++) at_queue_prepend(queue,values[i]);

  at_queue_reverse(queue);
  AtList* current;
  for(i=0,current = queue->begin; current;current = at_list_next(current),i++)
    assert_string_equal(at_list_value(current), values[i]);

  AtQueue* queuecopy = at_queue_copy(queue);
  AtList* current2;
  for(current = queue->begin,current2 = queuecopy->begin, i=0;
      current;
      current = at_list_next(current),current2 = at_list_next(current2),i++) {
    assert_int_not_equal(current, current2);
    assert_string_equal(at_list_value(current), at_list_value(current2));
  }

  at_queue_foreach(queue, add_prefix, "prev");
  for(current = queue->begin, i=0;current;current = at_list_next(current),i++) {
    assert_string_equal(at_list_value(current), values2[i]);
  }

}
static void test_at_queue_comparisons(void** state){
  (void) state;
}


int main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[4]={
    cmocka_unit_test(test_at_queue_adding_removing),
    cmocka_unit_test(test_at_queue_accessors),
    cmocka_unit_test(test_at_queue_operations),
    cmocka_unit_test(test_at_queue_comparisons),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
