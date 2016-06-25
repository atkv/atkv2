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

#include <at/pqueue.h>
#include <stdlib.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
static void
at_pqueue_find_non_null_priority(AtPQueue_uint64_t* queue){
  while(queue->priorities[queue->cur_priority].first == NULL){
    switch(queue->optimization){
      case AT_MINIMIZATION: queue->cur_priority++;break;
      case AT_MAXIMIZATION: queue->cur_priority--;break;
    }
  }
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtPQueue_uint64_t*
at_pqueue_uint64_t_new(AtOptimization optimization, AtPolicy policy){
  AtPQueue_uint64_t* queue     = malloc(sizeof(AtPQueue_uint64_t));
  queue->optimization = optimization;
  queue->policy       = policy;
  return queue;
}

void
at_pqueue_uint64_t_new_ptr(AtPQueue_uint64_t** queue_ptr, AtOptimization optimization, AtPolicy policy){
  *queue_ptr = at_pqueue_uint64_t_new(optimization,policy);
}

AtPQueue_uint64_t*
at_pqueue_uint64_t_new_prealloc(AtOptimization optimization, AtPolicy policy, uint64_t n_priorities, uint64_t n_values){
  AtPQueue_uint64_t* queue     = at_pqueue_uint64_t_new(optimization, policy);
  uint64_t  i;
  queue->priorities   = at_queue_uint64_t_new_array(n_priorities);
  queue->values       = at_list_uint64_t_new_array(n_values);
  for(i = 0; i < n_values; i++) queue->values[i].value = i;
  switch(optimization){
    case AT_MINIMIZATION: queue->cur_priority = 0;break;
    case AT_MAXIMIZATION: queue->cur_priority = n_priorities-1;break;
  }
}

void
at_pqueue_uint64_t_add(AtPQueue_uint64_t* queue, uint64_t priority, uint64_t value){
  switch(queue->policy){
    case AT_FIFO:
      at_queue_uint64_t_append_link(&queue->priorities[priority],&queue->values[value]);
    break;
    case AT_LIFO:
      at_queue_uint64_t_prepend_link(&queue->priorities[priority],&queue->values[value]);
    break;
  }
}

uint64_t
at_pqueue_uint64_t_remove(AtPQueue_uint64_t* queue){
  at_pqueue_find_non_null_priority(queue);
  return at_queue_uint64_t_remove_first_link(&queue->priorities[queue->cur_priority])->value;
}

void
at_pqueue_uint64_t_destroy(AtPQueue_uint64_t** queue_ptr){
  if(*queue_ptr){
    AtPQueue_uint64_t* queue = *queue_ptr;
    if(queue->priorities) at_queue_uint64_t_destroy_array(&queue->priorities);
    if(queue->values) at_list_uint64_t_destroy_array(&queue->values);
    free(*queue_ptr);
  }
}
