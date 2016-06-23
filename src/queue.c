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
#include <at/queue.h>
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtQueue_uint64_t*
at_queue_uint64_t_new_array(uint64_t n_queues){
  AtQueue_uint64_t* queues = malloc(n_queues*sizeof(AtQueue_uint64_t));
  memset(queues, 0, n_queues*sizeof(AtQueue_uint64_t));
  return queues;
}
void
at_queue_uint64_t_append_link(AtQueue_uint64_t* queue, AtList_uint64_t* listitem){
  listitem->prev   = NULL;
  if(queue->first != NULL){
    listitem->prev     = queue->first->prev;
    queue->first->prev = listitem;
  }
  if(queue->last == NULL)
    queue->last = listitem;
  listitem->next = queue->first;
  queue->first   = listitem;
}

void
at_queue_uint64_t_prepend_link(AtQueue_uint64_t* queue, AtList_uint64_t* listitem){
  listitem->next  = NULL;
  if(queue->last != NULL){
    listitem->next     = queue->last->next;
    queue->last->next = listitem;
  }
  if(queue->first == NULL)
    queue->first = listitem;
  listitem->prev = queue->last;
  queue->last   = listitem;
}

AtList_uint64_t*
at_queue_uint64_t_remove_first_link(AtQueue_uint64_t* queue){
  AtList_uint64_t* item = queue->first;
  if(item) {
    queue->first = item->next;
    item->next = NULL;
  }
  return item;
}
