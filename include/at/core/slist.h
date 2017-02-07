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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif
#ifndef AT_SLIST_H
#define AT_SLIST_H
#include <at/core.h>
AT_BEGIN_DECLS

/**
 * @brief Single-Linked AtList
 */
typedef struct _AtSList{
  void* value;        /**< Value of an item of the list */
  struct _AtSList* next;/**< Link to the next list item */
}AtSList;
/**
 * @brief at_slist_new
 * @return
 */
AtSList*    at_slist_new();
/**
 * @brief at_slist_new_with_value
 * @param value
 * @return
 */
AtSList*    at_slist_new_with_value(void* value);
/**
 * @brief at_slist_prepend
 * @param list
 * @param value
 * @return
 */
AtSList*    at_slist_prepend(AtSList* list, void* value);
/**
 * @brief at_slist_append
 * @param list
 * @param value
 * @return
 */
AtSList*    at_slist_append(AtSList* list, void* value);
/**
 * @brief at_slist_prepend_at
 * @param list
 * @param item
 * @param value
 * @return
 */
AtSList*    at_slist_prepend_at_item(AtSList* list, AtSList* item, void* value);
/**
 * @brief at_slist_append_at
 * @param list
 * @param item
 * @param value
 * @return
 */
AtSList*    at_slist_append_at_item(AtSList* list, AtSList* item, void* value);
/**
 * @brief at_slist_prepend_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
AtSList*    at_slist_prepend_at(AtSList* list, uint32_t index, void* value);
/**
 * @brief at_slist_append_at_index
 * @param list
 * @param index
 * @param value
 * @return
 */
AtSList*    at_slist_append_at(AtSList* list, uint32_t index, void* value);
/**
 * @brief at_slist_append_item
 * @param list
 * @param new_item
 * @return
 */
AtSList*    at_slist_append_item(AtSList* list, AtSList* new_item);
/**
 * @brief at_slist_append_item_at
 * @param list
 * @param index
 * @param new_item
 * @return
 */
AtSList*    at_slist_append_item_at(AtSList* list, uint32_t index, AtSList* new_item);
/**
 * @brief at_slist_append_item_at_item
 * @param list
 * @param item
 * @param new_item
 * @return
 */
AtSList*    at_slist_append_item_at_item(AtSList* list, AtSList* item, AtSList* new_item);
/**
 * @brief at_slist_prepend_item
 * @param list
 * @param new_item
 * @return
 */
AtSList*    at_slist_prepend_item(AtSList* list, AtSList* new_item);
/**
 * @brief at_slist_prepend_item_at
 * @param list
 * @param index
 * @param new_item
 * @return
 */
AtSList*    at_slist_prepend_item_at(AtSList* list, uint32_t index, AtSList* new_item);
/**
 * @brief at_slist_prepend_item_at_item
 * @param list
 * @param item
 * @param new_item
 * @return
 */
AtSList*    at_slist_prepend_item_at_item(AtSList* list, AtSList* item, AtSList* new_item);
/**
 * @brief at_slist_remove
 * @param list
 * @param item
 * @return
 */
AtSList*    at_slist_remove_item(AtSList* list, AtSList* item);
/**
 * @brief at_slist_remove_from_value
 * @param list
 * @param value
 * @return
 */
AtSList*    at_slist_remove(AtSList* list, void* value);
/**
 * @brief at_slist_remove_after
 * @param list
 * @param before
 * @return
 */
AtSList*    at_slist_remove_after(AtSList* list, AtSList* before);
/**
 * @brief at_slist_remove_at_index
 * @param list
 * @param index
 * @return
 */
AtSList*    at_slist_remove_at(AtSList* list, uint32_t index);
/**
 * @brief at_slist_remove_begin
 * @param list
 * @return
 */
AtSList*    at_slist_remove_begin(AtSList* list);
/**
 * @brief at_slist_remove_end
 * @param list
 * @return
 */
AtSList*    at_slist_remove_end(AtSList* list);
/**
 * @brief at_slist_free
 * @param list
 */
void      at_slist_free(AtSList* list);
/**
 * @brief at_slist_index_of
 * @param list
 * @param value
 * @return
 */
int32_t   at_slist_index_of(AtSList* list, void* value);
/**
 * @brief at_slist_item_of
 * @param list
 * @param value
 * @return
 */
AtSList*    at_slist_item_of(AtSList* list, void* value);
/**
 * @brief at_slist_end
 * @param list
 * @return
 */
AtSList*    at_slist_item_end(AtSList* list);
/**
 * @brief at_slist_end
 * @param list
 * @return
 */
void*     at_slist_end(AtSList* list);
/**
 * @brief at_slist_length
 * @param list
 * @return
 */
uint32_t  at_slist_length(AtSList* list);
/**
 * @brief at_slist_is_empty
 * @param list
 * @return
 */
uint8_t   at_slist_is_empty(AtSList* list);
/**
 * @brief at_slist_item_at
 * @param list
 * @param index
 * @return
 */
AtSList*    at_slist_item_at(AtSList* list, uint32_t index);
/**
 * @brief at_slist_at
 * @return
 */
void*     at_slist_at(AtSList* list, uint32_t index);
/**
 * @brief at_slist_next
 * @param list
 * @return
 */
AtSList*    at_slist_next(AtSList* list);
/**
 * @brief at_slist_prev
 * @param list
 * @param item
 * @return
 */
AtSList*    at_slist_prev(AtSList* list, AtSList* item);
/**
 * @brief at_slist_join
 * @param list1
 * @param list2
 * @return
 */
AtSList*    at_slist_join(AtSList* list1, AtSList* list2);
/**
 * @brief at_slist_split_at
 * @param list
 * @param index
 * @return
 */
AtSList*    at_slist_split_at(AtSList* list, uint32_t index);
/**
 * @brief at_slist_swap
 * @param list
 * @param item1
 * @param item2
 * @return
 */
AtSList*    at_slist_swap(AtSList* list, AtSList* item1, AtSList* item2);
/**
 * @brief at_slist_swap_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
AtSList*    at_slist_swap_at(AtSList* list, uint32_t index1, uint32_t index2);
/**
 * @brief at_slist_swap_values
 * @param list
 * @param item1
 * @param item2
 * @return
 */
AtSList* at_slist_swap_items(AtSList* list, AtSList* item1, AtSList* item2);
/**
 * @brief at_slist_swap_values_at
 * @param list
 * @param index1
 * @param index2
 * @return
 */
AtSList* at_slist_swap_items_at(AtSList* list, uint32_t index1, uint32_t index2);
/**
 * @brief at_slist_copy
 * @param list
 * @return
 */
AtSList* at_slist_copy(AtSList* list);
/**
 * @brief at_slist_replace
 * @param list
 * @param item
 * @param item2
 * @return
 */
AtSList* at_slist_replace(AtSList* list, AtSList* item, AtSList* item2);
/**
 * @brief at_slist_replace_at
 * @param list
 * @param index
 * @param item2
 * @return
 */
AtSList* at_slist_replace_at(AtSList* list, uint32_t index, AtSList* item2);
/**
 * @brief at_slist_reverse
 * @param list
 * @return
 */
AtSList* at_slist_reverse(AtSList* list);
/**
 * @brief at_slist_sort
 * @param list
 * @return
 */
AtSList* at_slist_sort(AtSList* list, AtCompareFunc compare_function);
/**
 * @brief at_slist_sort_with_data
 * @param list
 * @param compare_function
 * @param user_data
 * @return
 */
AtSList* at_slist_sort_with_data(AtSList* list, AtCompareDataFunc compare_function, void* user_data);
/**
 * @brief at_slist_is_different
 * @param list
 * @param list2
 * @return
 */
uint8_t   at_slist_is_different(AtSList* list, AtSList* list2);
/**
 * @brief at_slist_is_equal
 * @param list
 * @param list2
 * @return
 */
uint8_t   at_slist_is_equal(AtSList* list, AtSList* list2);
/**
 * @brief at_slist_value
 * @param list
 * @return
 */
AtSList* at_slist_value(AtSList* list);
/**
 * @brief at_slist_set_next
 * @param list
 * @param item
 * @return
 */
void      at_slist_set_next(AtSList* list, AtSList* item);
/**
 * @brief at_slist_set_value
 * @param list
 * @param value
 */
void      at_slist_set_value(AtSList* list, void* value);

AT_END_DECLS

#endif
