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
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtQueueu64*
at_queueu64_new_array(uint64_t n_queues){
  AtQueueu64* queues = malloc(n_queues*sizeof(AtQueueu64));
  memset(queues, 0, n_queues*sizeof(AtQueueu64));
  return queues;
}
void
at_queueu64_prepend_link(AtQueueu64* q, AtListU64* l){
  l->prev   = NULL;
  if(q->first != NULL){
    l->prev     = q->first->prev;
    q->first->prev = l;
  }
  if(q->last == NULL)
    q->last = l;
  l->next = q->first;
  q->first   = l;
}

void
at_queueu64_append_link(AtQueueu64* q, AtListU64* l){
  l->next  = NULL;
  if(q->last != NULL){
    l->next     = q->last->next;
    q->last->next = l;
  }
  if(q->first == NULL)
    q->first = l;
  l->prev = q->last;
  q->last   = l;
}

AtListU64*
at_queueu64_remove_first_link(AtQueueu64* q){
  AtListU64* l = q->first;
  if(l) {
    q->first = l->next;
    l->next = NULL;
  }
  return l;
}

void
at_queueu64_remove_link(AtQueueu64* q, AtListU64* l){
  if(l != NULL){
    if(q->first == l) q->first = l->next;
    if(q->last == l)  q->last  = l->prev;

    // Previous node link to next one and vice-versa
    if(l->prev) l->prev->next = l->next;
    if(l->next) l->next->prev = l->prev;

    // Same as:
    // l->prev = NULL;
    // l->next = NULL;
    memset(l,0,(sizeof(AtListU64*)<<1));
  }
}

void
at_queueu64_destroy_array(AtQueueu64** qp, uint64_t length){
  uint64_t i;
  for(i = 0; i < length; i++)
    at_listu64_destroy(&((*qp)->first));
  if(*qp != NULL) free(*qp);
  *qp = NULL;
}


// Allocate memory for the queue
AtQueue* at_queue_new(){
  AtQueue* queue  = malloc(sizeof(AtQueue));
  queue->length = 0;
  queue->begin  = NULL;
  queue->end    = NULL;
  return queue;
}
// Free the queue
void at_queue_free(AtQueue* queue){
  if(queue->begin) at_list_free(queue->begin);
  free(queue);
}
// Add in the beginning
void at_queue_prepend(AtQueue* queue, void* value){
  queue->length++;
  queue->begin = at_list_prepend(queue->begin, value);
  if(!queue->end) queue->end = queue->begin;
}
// Add in the end
void at_queue_append(AtQueue* queue, void* value){
  if(!queue->begin) at_queue_prepend(queue,value);
  else{
    queue->length++;
    at_list_append_at(queue->begin, queue->end, value);
    queue->end=at_list_next(queue->end);
  }
}
// Add before an item
void at_queue_prepend_at(AtQueue* queue, AtList* item, void* value){
  queue->length++;
  queue->begin = at_list_prepend_at(queue->begin, item, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Add after an item
void at_queue_append_at(AtQueue* queue, AtList* item, void* value){
  queue->length++;
  queue->begin = at_list_append_at(queue->begin, item, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}
// Add before item determined by its position
void at_queue_prepend_at_index(AtQueue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = at_list_prepend_at_index(queue->begin, index, value);
  if(queue->length == 1) queue->end = queue->begin;
}
// Add after specified item by its position
void at_queue_append_at_index(AtQueue* queue, uint32_t index, void* value){
  queue->length++;
  queue->begin = at_list_append_at_index(queue->begin, index, value);
  if(queue->length == 1)    queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}
// Add an item from beginning
void at_queue_prepend_item(AtQueue* queue, AtList* new_item){
  queue->length++;
  queue->begin->prev = new_item;
  at_list_set_next(new_item, queue->begin);
  new_item->prev     = NULL;
  queue->begin       = new_item;
}
// Add an item from the end
void at_queue_append_item(AtQueue* queue, AtList* new_item){
  queue->length++;
  at_list_set_next(queue->end,new_item);
  new_item->prev     = queue->end;
  at_list_set_next(new_item,NULL);
  queue->end         = new_item;
}
// Add an item before another one
void at_queue_prepend_item_at(AtQueue* queue, AtList* item, AtList* new_item){
  queue->length++;
  queue->begin = at_list_prepend_item_at(queue->begin, item, new_item);
}
// Add an item after another one
void at_queue_append_item_at(AtQueue* queue, AtList* item, AtList* new_item){
  queue->length++;
  queue->begin = at_list_append_item_at(queue->begin, item, new_item);
  if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}
// Add an item before another one specified by its position
void at_queue_prepend_item_at_index(AtQueue* queue, uint32_t index, AtList* new_item){
  queue->length++;
  queue->begin = at_list_prepend_item_at_index(queue->begin, index, new_item);
}
// Add an item after another one specified by its position
void at_queue_append_item_at_index(AtQueue* queue, uint32_t index, AtList* new_item){
  queue->length++;
  queue->begin = at_list_append_item_at_index(queue->begin, index, new_item);
  if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}
// Get first item
AtList* at_queue_begin(AtQueue* queue){
  return queue->begin;
}
// Get last item
AtList* at_queue_end(AtQueue* queue){
  return queue->end;
}
// True if length is zero
uint8_t at_queue_is_empty(AtQueue* queue){
  return queue->length == 0;
}
// Reverses the queue
void at_queue_reverse(AtQueue* queue){
  AtList* new_end = queue->begin;
  queue->begin = at_list_reverse(queue->begin);
  queue->end = new_end;
}
// Sort the queue
void at_queue_sort(AtQueue* queue, AtCompareDataFunc compare_function, void* data){

}
// Copy the queue
AtQueue* at_queue_copy(AtQueue* queue){
  AtQueue* queue2  = at_queue_new();
  queue2->length = queue->length;
  queue2->begin  = at_list_copy(queue->begin);
  queue2->end    = at_list_end(queue2->begin);
  return queue2;
}
// vectorize a function within a queue
void at_queue_foreach(AtQueue* queue, AtDataFunc data_function, void* data){
  at_list_foreach(queue->begin, data_function, data);
}
// position of an item specified by its value
int32_t at_queue_index_of(AtQueue* queue, void* value){
  return at_list_index_of(queue->begin, value);
}
// get an item from its position
AtList* at_queue_at(AtQueue* queue, uint32_t index){
  return at_list_at(queue->begin, index);
}
// get value from its position
void* at_queue_value_at(AtQueue* queue, uint32_t index){
  return at_list_value(at_queue_at(queue,index));
}
// Get the value of first item
void* at_queue_begin_value(AtQueue* queue){
  if(at_queue_is_empty(queue)) return NULL;
  return at_list_value(queue->begin);
}
// Get the value of last item
void* at_queue_end_value(AtQueue* queue){
  if(at_queue_is_empty(queue)) return NULL;
  return at_list_value(queue->end);
}
// Remove an item in the queue
void at_queue_remove(AtQueue* queue, void* value){
  AtList* item = at_list_find(queue->begin,value);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = at_list_remove(queue->begin, item);
  queue->length--;
}
// Remove an item in the queue based on its position
void* at_queue_remove_at(AtQueue* queue, uint32_t index){
  if(queue->length <= index) return NULL;
  AtList* item = at_list_at(queue->begin,index);
  void* value = at_list_value(item);
  if(queue->end == item) queue->end = queue->end->prev;
  queue->begin = at_list_remove(queue->begin, item);
  queue->length--;
  return value;
}
// Remove first element
void* at_queue_remove_begin(AtQueue* queue){
  if(at_queue_is_empty(queue)) return NULL;
  void* value  = at_list_value(queue->begin);
  queue->begin = at_list_remove_begin(queue->begin);
  if(!queue->begin) queue->end = NULL;
  queue->length--;
  return value;
}
// Remove last element
void* at_queue_remove_end(AtQueue* queue){
  if(at_queue_is_empty(queue)) return NULL;
  void* value = at_list_value(queue->end);
  queue->end   = queue->end->prev;
  queue->begin = at_list_remove_end(queue->begin);
  queue->length--;
  return value;
}
// Ger number of elements in queue
uint32_t at_queue_length(AtQueue* queue){
  return queue->length;
}

void at_queue_append_sorted(AtQueue* queue, AtCompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = at_list_append_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}

void at_queue_append_sorted_with_data(AtQueue* queue, AtCompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = at_list_append_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}

void at_queue_prepend_sorted(AtQueue* queue, AtCompareFunc compare_function, void* value){
  queue->length++;
  queue->begin = at_list_prepend_sorted(queue->begin, compare_function, value);
  if(!queue->end) queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}

void at_queue_prepend_sorted_with_data(AtQueue* queue, AtCompareDataFunc compare_function, void* value, void* user_data){
  queue->length++;
  queue->begin = at_list_prepend_sorted_with_data(queue->begin, compare_function, value, user_data);
  if(!queue->end) queue->end = queue->begin;
  else if(at_list_next(queue->end)) queue->end = at_list_next(queue->end);
}
