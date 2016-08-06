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
#include <string.h>
#include <stdlib.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PRIVATE API
 ============================================================================*/

AtListU64*
at_listu64_new_array(uint64_t n_items){
  AtListU64* list = calloc(n_items, sizeof(AtListU64));
  return list;
}

void
at_listu64_destroy(AtListU64** list_ptr){
  if(*list_ptr){
    AtListU64* list = *list_ptr;
    AtListU64* next;
    do{
      next = list->next;
      free(list);
      list = next;
    }while(list != NULL);
    *list_ptr = NULL;
  }
}

void
at_listu64_destroy_array(AtListU64** list){
  if(*list) free(*list);
  *list = NULL;
}

AtList* at_list_new(){
  AtList* item = malloc(sizeof(AtList));
  at_list_set_next(item,NULL);
  at_list_set_prev(item,NULL);
  at_list_set_value(item,NULL);

  return item;
}
AtList* at_list_new_with_value(void* value){
  AtList* list = at_list_new();
  at_list_set_value(list, value);
  return list;
}

AtList* at_list_append(AtList* list, void* value){
  AtList* item = at_list_new_with_value(value);

  if(list == NULL) return item;
  else{
    AtList* last = at_list_end(list);
    at_list_set_next(last,item);
    at_list_set_prev(item,last);
    at_list_set_next(item,NULL);
  }
  return list;
}

void at_list_set_next(AtList* item, AtList* next){
  item->base.next = (AtSList*)next;
}
void at_list_set_prev(AtList* item, AtList* prev){
  item->prev = prev;
}
void at_list_set_value(AtList* item, void* value){
  item->base.value = value;
}

AtList* at_list_append_at(AtList *list, AtList *item, void *value){
  AtList* begin = at_list_begin(list);
  AtList* new_item = at_list_new_with_value(value);
  return at_list_append_item_at(begin, item, new_item);
}

AtList* at_list_append_at_index(AtList *list, uint32_t index, void *value){
  return at_list_append_at(list, at_list_at(list, index), value);
}

void at_list_free(AtList *list){
  if(list){
    AtList* item = at_list_begin(list);
    AtList* next = at_list_next(item);
    while(at_list_next(item)){free(item); item = next; next = at_list_next(item);}
    free(item);
  }
}

AtList* at_list_prepend(AtList *list, void *value){
  AtList* item  = at_list_new_with_value(value);
  AtList* begin = at_list_begin(list);
  if(begin == NULL) return item;
  at_list_set_next(item , begin);
  at_list_set_prev(begin, item );
  return item;
}

AtList* at_list_prepend_at(AtList *list, AtList *item, void *value){
  AtList* begin = at_list_begin(list);
  AtList* new_item = at_list_new_with_value(value);
  return at_list_prepend_item_at(begin,item,new_item);
}

AtList* at_list_prepend_item(AtList *list, AtList *new_item){
  if(list == NULL) return new_item;
  at_list_set_next(new_item, list);
  at_list_set_prev(list, new_item);
  return new_item;
}

AtList* at_list_prepend_at_index(AtList *list, uint32_t index, void *value){
  return at_list_prepend_at(list, at_list_at(list, index), value);
}

AtList* at_list_remove(AtList *list, AtList *item){
  if(item == NULL) return list;
  AtList* begin  = at_list_begin(list);
  if(at_list_next(item)) at_list_set_prev(at_list_next(item),at_list_prev(item));
  if(at_list_prev(item)) at_list_set_next(at_list_prev(item),at_list_next(item));
  else begin   = at_list_next(item);
  free(item);
  return begin;
}

AtList* at_list_prepend_item_at(AtList* list, AtList* item, AtList* item_new){
  at_list_set_next(item_new,item);
  at_list_set_prev(item_new,at_list_prev(item));
  at_list_set_prev(item, item_new);
  if(at_list_prev(item_new)) at_list_set_next(at_list_prev(item_new),item_new);
  else list = item_new;
  return list;
}

AtList* at_list_append_item_at(AtList* list, AtList* item, AtList* item_new){
  at_list_set_prev(item_new,item);
  at_list_set_next(item_new,at_list_next(item));
  at_list_set_next(item, item_new);
  if(at_list_next(item_new)) at_list_set_prev(at_list_next(item_new),item_new);

  return list;
}

AtList* at_list_prepend_item_at_index(AtList* list, uint32_t index, AtList* item_new){
  return at_list_prepend_item_at(list, at_list_at(list, index), item_new);
}

AtList* at_list_append_item_at_index(AtList* list, uint32_t index, AtList* item_new){
  return at_list_append_item_at(list, at_list_at(list, index), item_new);
}

AtList* at_list_remove_at_index(AtList *list, uint32_t index){
  return at_list_remove(list,at_list_at(list, index));
}

AtList* at_list_remove_begin(AtList *list){
  if(list == NULL) return NULL;
  AtList* item       = at_list_begin(list);
  AtList* new_begin  = at_list_next(item);
  if(new_begin) at_list_set_prev(new_begin,NULL);
  free(item);
  return new_begin;
}

AtList* at_list_remove_end(AtList *list){
  if(list == NULL) return NULL;
  AtList* begin = at_list_begin(list);
  AtList* end   = at_list_end(list);
  if(begin == end) begin = NULL;
  else at_list_set_next(at_list_prev(end),NULL);
  free(end);
  return begin;
}

AtList* at_list_remove_from_value(AtList *list, void *value){
  return at_list_remove(list, at_list_find(list, value));
}
int32_t at_list_index_of(AtList* list, void* value){
  if(!list) return -1;
  int32_t  i        = 0;
  AtList     *current = list;
  while(current && at_list_value(current) != value) {current = at_list_next(current);i++;}
  if(current) return i;
  return -1;
}

AtList*    at_list_begin(AtList* list){
  if(list){
    AtList* current = list;
    while(current->prev != NULL) current = current->prev;
    return current;
  }
  return NULL;
}

AtList*    at_list_end(AtList* list){
  if(list){
    AtList* current = list;
    while(at_list_next(current) != NULL) current = at_list_next(current);
    return current;
  }
  return NULL;
}

uint32_t at_list_length(AtList* list){
  uint32_t i = 0;
  AtList* current = list;
  while(current){current = at_list_next(current); i++;}
  return i;
}

uint8_t  at_list_is_empty(AtList* list){
  return list == NULL;
}

AtList*    at_list_at(AtList* list, uint32_t index){
  uint32_t i = 0;
  AtList* current = list;
  while(i++ < index && at_list_next(current)) current = at_list_next(current);
  if(i < index) return NULL;
  return current;
}

void*    at_list_value_at(AtList* list, uint32_t index){
  AtList* item = at_list_at(list, index);
  if(item) return at_list_value(item);
  return NULL;
}

AtList*    at_list_next(AtList* list){
  return (AtList*)list->base.next;
}

AtList*    at_list_prev(AtList* list){
  return list->prev;
}

AtList*    at_list_join(AtList *list1, AtList *list2){
  AtList* end1 = at_list_end(list1);
  at_list_set_next(end1,list2);
  at_list_set_prev(list2,end1);
  return at_list_begin(list1);
}

AtList*    at_list_split_at(AtList *list, uint32_t index){
  AtList* list2 = at_list_at(list, index);
  at_list_set_next(at_list_prev(list2),NULL);
  at_list_set_prev(list2,NULL);
  return list2;
}

AtList*    at_list_swap_items(AtList* list, AtList* item1, AtList* item2){
  AtList* item1_prev = at_list_prev(item1);
  AtList* item1_next = at_list_next(item1);
  AtList* item2_prev = at_list_prev(item2);
  AtList* item2_next = at_list_next(item2);

  // Change specified items
  if(item1_prev != item2)  at_list_set_next(item1,item2_next);
  else                     at_list_set_next(item1,item2);
  if(item1_next != item2)  at_list_set_prev(item1,item2_prev);
  else                     at_list_set_prev(item1,item2);
  if(item2_prev != item1)  at_list_set_next(item2,item1_next);
  else                     at_list_set_next(item2,item1);
  if(item2_next != item1)  at_list_set_prev(item2,item1_prev);
  else                     at_list_set_prev(item2,item1);

  // Change neighbor items
  if(item1_prev && item1_prev != item2) at_list_set_next(item1_prev,item2);
  if(item1_next && item1_next != item2) at_list_set_prev(item1_next,item2);
  if(item2_prev && item2_prev != item1) at_list_set_next(item2_prev,item1);
  if(item2_next && item2_next != item1) at_list_set_prev(item2_next,item1);
  return at_list_begin(list);
}

AtList*    at_list_swap_items_at(AtList* list, uint32_t index1, uint32_t index2){
  AtList* item1 = at_list_at(list, index1);
  AtList* item2 = at_list_at(list, index2);
  return at_list_swap_items(list, item1, item2);
}

AtList*    at_list_swap(AtList* list, AtList* item1, AtList* item2){
  void* tmp_value = at_list_value(item1);
  at_list_set_value(item1,at_list_value(item2));
  at_list_set_value(item2,tmp_value);
  return list;
}

AtList*    at_list_swap_at(AtList* list, uint32_t index1, uint32_t index2){
  AtList* item1 = at_list_at(list, index1);
  AtList* item2 = at_list_at(list, index2);
  return at_list_swap(list, item1, item2);
}

AtList*    at_list_copy(AtList* list){
  AtList* begin;
  AtList* list2 = NULL;
  AtList* item;
  while(list){
    item = at_list_new();
    at_list_set_value(item,at_list_value(list));
    if(list2){
      at_list_set_next(list2,item);
      at_list_set_prev(item,list2);
    }
    else begin = item;
    list2 = item;
    list  = at_list_next(list);
  }
  return begin;
}

AtList*    at_list_replace(AtList* list, AtList* item, AtList* item2){
  AtList* item_prev = at_list_prev(item);
  AtList* item_next = at_list_next(item);

  at_list_set_next(item_prev, item2);
  at_list_set_prev(item_next, item2);
  at_list_set_prev(item2, item_prev);
  at_list_set_next(item2, item_next);

  free(item);

  return at_list_begin(list);
}

AtList*    at_list_replace_at(AtList* list, uint32_t index, AtList* item2){
  AtList* item = at_list_at(list, index);
  return at_list_replace(list, item, item2);
}

uint8_t  at_list_is_different(AtList* list, AtList* list2){
  while(list) {
    if(at_list_value(list) != at_list_value(list2)) return 1;
    list = at_list_next(list);
    list2 = at_list_next(list2);
  }
  // They have different size
  if((list && at_list_next(list)) || (list2 && at_list_next(list2))) return 1;
  return 0;
}

uint8_t  at_list_is_equal(AtList* list, AtList* list2){
  return !at_list_is_different(list, list2);
}

AtList* at_list_find(AtList *list, void *value){
  AtList* current = at_list_begin(list);
  while(at_list_next(current) && at_list_value(current) != value) current = at_list_next(current);
  if(at_list_value(current) != value) return NULL;
  else return current;
}

AtList* at_list_reverse(AtList *list){
  AtList *after, *before;
  while(list){
    before = at_list_prev(list);
    after  = at_list_next(list);

    at_list_set_next(list, before);
    at_list_set_prev(list, after);

    before = list;
    list   = after;
  }
  return before;
}

void at_list_foreach(AtList* list, AtDataFunc data_function, void* value){
  AtList* current;
  for(current = list; current; current = at_list_next(current))
    data_function(current, value);
}

AtList* at_list_append_sorted(AtList* list, AtCompareFunc compare_function, void* value){
  AtList* current = list, *before = NULL;
  if(!list) return at_list_prepend(NULL, value);
  while(current && compare_function(at_list_value(current), value) <= 0){
    before  = current;
    current = at_list_next(current);
  }
  if(!current) return at_list_append_at(list,before,value);
  return at_list_prepend_at(list, current, value);
}
AtList* at_list_append_sorted_with_data(AtList* list, AtCompareDataFunc compare_function, void* value, void* user_data){
  AtList* current = list, *before = NULL;
  if(!list) return at_list_prepend(NULL, value);
  while(current && compare_function(at_list_value(current), value, user_data) <= 0){
    before  = current;
    current = at_list_next(current);
  }
  if(!current) return at_list_append_at(list,before,value);
  return at_list_prepend_at(list, current, value);
}
AtList* at_list_prepend_sorted(AtList* list, AtCompareFunc compare_function, void* value){
  AtList* current = list, *before = NULL;
  if(!list) return at_list_prepend(NULL, value);
  while(current && compare_function(at_list_value(current), value) < 0){
    before  = current;
    current = at_list_next(current);
  }
  if(!current) return at_list_append_at(list,before,value);
  return at_list_prepend_at(list, current, value);
}
AtList* at_list_prepend_sorted_with_data(AtList* list, AtCompareDataFunc compare_function, void* value, void* user_data){
  AtList* current = list, *before = NULL;
  if(!list) return at_list_prepend(NULL, value);
  while(current && compare_function(at_list_value(current), value, user_data) < 0){
    before  = current;
    current = at_list_next(current);
  }
  if(!current) return at_list_append_at(list,before,value);
  return at_list_prepend_at(list, current, value);
}
void* at_list_value(AtList* item){
  return item->base.value;
}

