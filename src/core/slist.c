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
#include <at/core.h>

AtSList*    at_slist_new(){
  AtSList* item = malloc(sizeof(AtSList));
  item->next = NULL;
  at_slist_set_value(item,NULL);
  return item;
}
AtSList*    at_slist_new_with_value(void* value){
  AtSList* item = at_slist_new();
  at_slist_set_value(item,value);
  return item;
}

AtSList*    at_slist_prepend(AtSList* list, void* value){
  return at_slist_prepend_item(list, at_slist_new_with_value(value));
}

AtSList*    at_slist_append(AtSList* list, void* value){
  return at_slist_append_item(list, at_slist_new_with_value(value));
}

AtSList*    at_slist_prepend_at_item(AtSList* list, AtSList* item, void* value){
  if(list == item) return at_slist_prepend(list, value);
  AtSList* current = list;
  while(current && current->next != item) current=current->next;
  AtSList* new_item = at_slist_new_with_value(value);
  new_item->next  = item;
  current->next   = new_item;
  return list;
}

AtSList*    at_slist_append_at_item(AtSList* list, AtSList* item, void* value){
  return at_slist_append_item_at_item(list, item, at_slist_new_with_value(value));
}

AtSList*    at_slist_prepend_at(AtSList* list, uint32_t index, void* value){
  if(index) return at_slist_append_at_item(list, at_slist_item_at(list, index-1), value);
  return at_slist_append(NULL, value);
}

AtSList*    at_slist_append_at(AtSList* list, uint32_t index, void* value){
  return at_slist_append_at_item(list, at_slist_item_at(list, index), value);
}
AtSList*    at_slist_append_item(AtSList *list, AtSList *new_item){
  if(list == NULL) return new_item;
  else{
    AtSList* last = at_slist_item_end(list);
    last->next = new_item;
    new_item->next = NULL;
  }
  return list;
}
AtSList*    at_slist_append_item_at_item(AtSList *list, AtSList *item, AtSList *new_item){
  new_item->next  = item->next;
  item->next      = new_item;
  return list;
}
AtSList*    at_slist_append_item_at(AtSList *list, uint32_t index, AtSList *new_item){
  return at_slist_append_item_at_item(list, at_slist_item_at(list,index), new_item);
}
AtSList*    at_slist_prepend_item(AtSList *list, AtSList *new_item){
  if(list == NULL) return new_item;
  new_item->next = list;
  return new_item;
}
AtSList*    at_slist_prepend_item_at(AtSList *list, uint32_t index, AtSList *new_item){
  if(!index || !list) return at_slist_prepend_item(list, new_item);
  return at_slist_append_item_at(list, index-1, new_item);
}
AtSList*    at_slist_prepend_item_at_item(AtSList *list, AtSList *item, AtSList *new_item){
  if(!item || !list) return at_slist_prepend_item(list, new_item);
  AtSList* before;
  for(before = list; before->next && before->next != item; before=before->next);
  return at_slist_append_item_at_item(list, before, new_item);
}


AtSList*    at_slist_remove_item(AtSList* list, AtSList* item){
  if(!list || !item) return list;
  if(item  ==  list) return at_slist_remove_begin(list);
  AtSList* prev = at_slist_prev(list, item);
  prev->next  = item->next;
  free(item);
  return list;
}

AtSList*    at_slist_remove(AtSList* list, void* value){
  AtSList* current = list;
  while(current->next && current->next->value != value) current = current->next;
  if(current->next) return at_slist_remove_after(list, current);
  return list;
}

AtSList*    at_slist_remove_at(AtSList* list, uint32_t index){
  if(!index) return at_slist_remove_begin(list);
  return at_slist_remove_after(list, at_slist_item_at(list, index-1));
}

AtSList*    at_slist_remove_after(AtSList* list, AtSList* before){
  if(!list || !before) return list;
  if(before == list) return at_slist_remove_begin(list);
  AtSList* item  = before->next;
  before->next = item->next;
  free(item);
  return list;
}

AtSList*    at_slist_remove_begin(AtSList* list){
  if(list == NULL) return NULL;
  AtSList* item       = list;
  AtSList* new_begin  = item->next;
  free(item);
  return new_begin;
}

AtSList*    at_slist_remove_end(AtSList* list){
  if(!list) return NULL;
  if(!list->next) {free(list); return NULL;}
  AtSList* before = list;
  while(before->next->next) before=before->next;
  free(before->next);
  before->next = NULL;
  return list;
}

void      at_slist_free(AtSList* list){
  if(list){
    AtSList* item = list;
    AtSList* next = item->next;
    while(item->next){free(item); item = next; next = item->next;}
    free(item);
  }
}

int32_t   at_slist_index_of(AtSList* list, void* value){
  int32_t i;
  for(i=0;list;list=list->next,i++) if(list->value == value) return i;
  return -1;
}

AtSList*    at_slist_item_end(AtSList* list){
  if(!list) return NULL;
  while(list->next) list = list->next;
  return list;
}
void*     at_slist_end(AtSList* list){
  return at_slist_item_end(list)->value;
}

uint32_t  at_slist_length(AtSList* list){
  uint32_t i;
  for(i=0;list;list=list->next, i++);
  return i;
}

uint8_t   at_slist_is_empty(AtSList* list){
  return list == NULL;
}

void*    at_slist_at(AtSList* list, uint32_t index){
  return at_slist_item_at(list, index)->value;
}

AtSList*    at_slist_item_at(AtSList* list, uint32_t index){
  uint32_t i;
  for(i = 0; i < index; i++) list = list->next;
  return list;
}

AtSList*    at_slist_next(AtSList* list){
  return list->next;
}

AtSList*    at_slist_prev(AtSList* list, AtSList* item){
  while(list->next != item) list = list->next;
  return list;
}

AtSList*    at_slist_join(AtSList* list1, AtSList* list2){
  AtSList* end1 = at_slist_item_end(list1);
  end1->next = list2;
  return list1;
}

AtSList*    at_slist_split_at(AtSList* list, uint32_t index){
  AtSList* end1 = at_slist_item_at(list,index-1);
  AtSList* list2 = end1->next;
  end1->next = NULL;
  return list2;
}

AtSList*    at_slist_swap(AtSList* list, AtSList* item1, AtSList* item2){
  void* tmp    = item1->value;
  at_slist_set_value(item1,item2->value);
  at_slist_set_value(item2,tmp);
  return list;
}

AtSList*    at_slist_swap_at(AtSList* list, uint32_t index1, uint32_t index2){
  return at_slist_swap(list, at_slist_item_at(list, index1),at_slist_item_at(list, index2));
}

AtSList*   at_slist_swap_items_after(AtSList* list, AtSList* before1, AtSList* before2, AtSList* item1, AtSList* item2){
  AtSList* tmp;

  // Sort items
  if(before1 == item2 || before1->next == item2){
    tmp = item1;item1=item2;item2=tmp;
  }

  // Exchange items
  tmp              = item1->next;
  item1->next      = item2->next;
  if(tmp != item2) item2->next = tmp;
  else item2->next = item1;

  // Fix previous items links to items
  if(before1 != item1) before1->next = item2;
  if(before2 != item2) before2->next = item1;

  if(list == item2) return item1;
  return list;
}


AtSList*    at_slist_swap_items(AtSList* list, AtSList* item1, AtSList* item2){
  AtSList* before1=NULL, *before2=NULL, *current, *tmp, *before;

  before = current = list;
  while(current){
    // Get previous items
    if(current == item1) before1 = before;
    if(current == item2) before2 = before;

    // If before2 was defined before before1,
    // then item2 is placed before item1
    // Let's sort it
    if(current == item1 && before2) {
      tmp = before1;before1 = before2;before2 = tmp;
      tmp = item1  ;item1   = item2  ;item2   = tmp;
    }

    // Fix before2 if item1 and item2 are neighbors
    if(item1->next == item2) before2 = item2;

    // Continue or break if items are defined
    if(before1 && before2) break;
    before  = current;
    current = current->next;
  }

  return at_slist_swap_items_after(list, before1, before2, item1, item2);
}

AtSList*    at_slist_swap_items_at(AtSList* list, uint32_t index1, uint32_t index2){
  AtSList* item1, *item2, *before1, *before2, *before, *current;
  uint32_t i = 0;
  // Same indices? Do nothing
  if(index1 == index2) return list;
  // Sort indices
  else if(index1 > index2) {
    uint32_t tmpi = index1;
    index1        = index2;
    index2        = tmpi;
  }
  // Iterate inside list
  before = current = list;
  while(current && i <= index2){

    // Get items and items before
    if(i == index1){ before1 = before; item1 = current;}
    if(i == index2){ before2 = before; item2 = current;}

    // Next iteration
    before  = current;
    current = current->next;
    i++;
  }
  if(index2 == index1+1) before2 = item2;

  // Swap items and return beginning
  return at_slist_swap_items_after(list, before1, before2, item1, item2);
}


AtSList*    at_slist_copy(AtSList* list){
  AtSList* list2 = NULL, *item2, *begin;
  while(list){
    item2 = at_slist_new_with_value(list->value);
    if(list2) list2->next = item2;
    else      begin = item2;
    list2 = item2;
    list  = list->next;
  }
  return begin;
}

AtSList*    at_slist_replace(AtSList* list, AtSList* item, AtSList* item2){
  AtSList* before;
  for(before = list; before->next != item; before = before->next);
  list          = at_slist_remove_after(list, before);
  return          at_slist_append_item_at_item(list,before,item2);
}

AtSList*    at_slist_replace_at(AtSList* list, uint32_t index, AtSList* item2){
  AtSList* before = at_slist_item_at(list, index-1);
  list          = at_slist_remove_after(list, before);
  return          at_slist_append_item_at_item(list,before,item2);
}

AtSList*    at_slist_reverse(AtSList* list){
  if(list && list->next){
    AtSList* current = list, *after = current->next, *old_after;
    current->next  = NULL;
    while(after){
      old_after   = after->next;
      after->next = current;
      current     = after;
      after       = old_after;
    }
    return current;
  }
  return list;
}

AtSList*    at_slist_sort(AtSList* list, AtCompareFunc compare_function){
  return NULL;
}
AtSList*    at_slist_sort_with_data(AtSList* list, AtCompareDataFunc compare_function, void* user_data){
  return NULL;
}

uint8_t   at_slist_is_different(AtSList* list, AtSList* list2){
  while(list){
    // If different values           or       lengths
    if((list->value != list2->value) || ((list->next==NULL)^(list->next==NULL)))
      return 1;
    list  = list->next;
    list2 = list2->next;
  }
  return 0;
}

uint8_t   at_slist_is_equal(AtSList* list, AtSList* list2){
  return !at_slist_is_different(list, list2);
}

AtSList*    at_slist_item_of(AtSList* list, void* value){
  while(list && list->value != value) list=list->next;
  return list;
}

void at_slist_set_value(AtSList* list, void* value){
  list->value = value;
}
AtSList* at_slist_value(AtSList* list){
  return list->value;
}
void at_slist_set_next(AtSList* list, AtSList* item){
  list->next = item;
}
