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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif
#ifndef AT_LIST_H
#define AT_LIST_H
#include <at/core/macro.h>
AT_BEGIN_DECLS
#include <stdint.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct AtListU64 AtListU64;
/**
 * @brief List 64 bits
 */
struct AtListU64{
  AtListU64* prev; /*!< Previous list item */
  AtListU64* next; /*!< Next list item */
  uint64_t value;  /*!< Value of current item */
};

typedef struct _AtList{
  AtSList base;/**< All resources of single linked list (link to next item and value) */
  struct _AtList* prev;/**< Link to previous item */
}AtList;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_listu64_new_array
 * @param n_items
 * @return
 */
AtListU64*
at_listu64_new_array(uint64_t n_items);
/**
 * @brief at_listu64_destroy
 * @param list_ptr
 */
void
at_listu64_destroy(AtListU64** list_ptr);
/**
 * @brief at_listu64_destroy_array
 * @param list
 */
void
at_listu64_destroy_array(AtListU64** list);


/**
 * @brief at_list_new
 * @return
 */
AtList*    at_list_new();
/**
 * @brief at_list_prepend
 * @param list
 * @param value
 * @return
 */
AtList*    at_list_prepend(AtList* list, void* value);
/**
 * @brief at_list_append
 * @param list
 * @param value
 * @return
 */
AtList*    at_list_append(AtList* list, void* value);
/**
 * @brief at_list_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
AtList*    at_list_prepend_at(AtList* list, AtList* item, void* value);
/**
 * @brief at_list_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
AtList*    at_list_append_at(AtList* list, AtList* item, void* value);
/**
 * @brief at_list_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
AtList*    at_list_prepend_at_index(AtList* list, uint32_t index, void* value);
/**
 * @brief at_list_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
AtList*    at_list_append_at_index(AtList* list, uint32_t index, void* value);
/**
 * @brief at_list_prepend_item
 * @param list
 * @param new_item
 * @return
 */
AtList*    at_list_prepend_item(AtList *list, AtList *new_item);
/**
 * @brief at_list_prepend_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
AtList*    at_list_prepend_item_at(AtList* list, AtList* item, AtList* item_new);
/**
 * @brief at_list_append_item_at
 * @param list
 * @param item
 * @param item_new
 * @return
 */
AtList*    at_list_append_item_at(AtList* list, AtList* item, AtList* item_new);
/**
 * @brief at_list_prepend_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
AtList*    at_list_prepend_item_at_index(AtList* list, uint32_t index, AtList* item_new);
/**
 * @brief at_list_append_item_at_index
 * @param list
 * @param index
 * @param item_new
 * @return
 */
AtList*    at_list_append_item_at_index(AtList* list, uint32_t index, AtList* item_new);
/**
 * @brief at_list_remove
 * @param list
 * @param item
 * @return
 */
AtList*    at_list_remove(AtList* list, AtList* item);
/**
 * @brief at_list_remove_from_value
 * @param list
 * @param value
 * @return
 */
AtList*    at_list_remove_from_value(AtList* list, void* value);
/**
 * @brief at_list_remove_at_index
 * @param list
 * @param index
 * @return
 */
AtList*    at_list_remove_at_index(AtList* list, uint32_t index);
/**
 * @brief at_list_remove_begin
 * @param list
 * @return
 */
AtList*    at_list_remove_begin(AtList* list);
/**
 * @brief at_list_remove_end
 * @param list
 * @return
 */
AtList*    at_list_remove_end(AtList* list);
/**
 * @brief at_list_free
 * @param list
 */
void        at_list_free(AtList* list);
/**
 * @brief at_list_index_of
 * @param list
 * @param value
 * @return
 */
int32_t     at_list_index_of(AtList* list, void* value);
/**
 * @brief at_list_begin
 * @param list
 * @return
 */
AtList*    at_list_begin(AtList* list);
/**
 * @brief at_list_end
 * @param list
 * @return
 */
AtList*    at_list_end(AtList* list);
/**
 * @brief at_list_length
 * @param list
 * @return
 */
uint32_t    at_list_length(AtList* list);
/**
 * @brief at_list_is_empty
 * @param list
 * @return
 */
uint8_t     at_list_is_empty(AtList* list);
/**
 * @brief at_list_at
 * @param list
 * @param index
 * @return
 */
AtList*    at_list_at(AtList* list, uint32_t index);
/**
 * @brief at_list_value_at
 * @param list
 * @param index
 * @return
 */
void*       at_list_value_at(AtList* list, uint32_t index);
/**
 * @brief at_list_next
 * @param list
 * @return
 */
AtList*    at_list_next(AtList* list);
/**
 * @brief at_list_prev
 * @param list
 * @return
 */
AtList*    at_list_prev(AtList* list);
/**
 * @brief at_list_join
 * @param list1
 * @param list2
 * @return
 */
AtList*    at_list_join(AtList* list1, AtList* list2);
/**
 * @brief at_list_split_at
 * @param list
 * @param index
 * @return
 */
AtList*    at_list_split_at(AtList* list, uint32_t index);
/**
 * @brief at_list_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
AtList*    at_list_swap_items(AtList* list, AtList* item1, AtList* item2);
/**
 * @brief at_list_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
AtList*    at_list_swap_items_at(AtList* list, uint32_t index1, uint32_t index2);
/**
 * @brief at_list_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
AtList*    at_list_swap(AtList* list, AtList* item1, AtList* item2);
/**
 * @brief at_list_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
AtList*    at_list_swap_at(AtList* list, uint32_t index1, uint32_t index2);
/**
 * @brief at_list_copy
 * @param list
 * @return
 */
AtList*    at_list_copy(AtList* list);
/**
 * @brief at_list_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
AtList*    at_list_replace(AtList* list, AtList* item, AtList* item2);
/**
 * @brief at_list_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
AtList*    at_list_replace_at(AtList* list, uint32_t index, AtList* item2);
/**
 * @brief at_list_reverse
 * @param list
 * @return
 */
AtList*    at_list_reverse(AtList* list);
/**
 * @brief at_list_sort
 * @param list
 * @return
 */
AtList*    at_list_sort(AtList* list, AtCompareFunc compare_function);
/**
 * @brief at_list_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t     at_list_is_different(AtList* list, AtList* list2);
/**
 * @brief at_list_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t     at_list_is_equal(AtList* list, AtList* list2);
/**
 * @brief at_list_find
 * @param list
 * @param value
 * @return
 */
AtList*    at_list_find(AtList* list, void* value);
/**
 * @brief at_list_foreach
 * @param list
 * @param data_function
 * @param value
 */
void        at_list_foreach(AtList* list, AtDataFunc data_function, void* value);
/**
 * @brief at_list_append_sorted
 * @param list
 * @param compare_function
 * @param value
 */
AtList*    at_list_append_sorted(AtList* list, AtCompareFunc compare_function, void* value);
/**
 * @brief at_list_append_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
AtList*    at_list_append_sorted_with_data(AtList* list, AtCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief at_list_prepend_sorted
 * @param list
 * @param compare_function
 * @param value
 */
AtList*    at_list_prepend_sorted(AtList* list, AtCompareFunc compare_function, void* value);
/**
 * @brief at_list_prepend_sorted_with_data
 * @param list
 * @param compare_function
 * @param value
 * @param user_data
 */
AtList*    at_list_prepend_sorted_with_data(AtList* list, AtCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief at_list_set_next
 * @param item
 * @param next
 */
void at_list_set_next(AtList* item, AtList* next);
/**
 * @brief at_list_set_prev
 * @param item
 * @param prev
 */
void at_list_set_prev(AtList* item, AtList* prev);
/**
 * @brief at_list_set_value
 * @param item
 * @param value
 */
void at_list_set_value(AtList* item, void* value);
/**
 * @brief at_list_value
 * @param item
 * @return
 */
void* at_list_value(AtList* item);
/**
 * @brief at_list_new_with_value
 * @param value
 * @return
 */
AtList* at_list_new_with_value(void* value);
AT_END_DECLS
#endif
