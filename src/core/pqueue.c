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

#include <at/core/pqueue.h>
#include <stdlib.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
static void
at_pqueue_find_non_null_priority(AtPQueueU64* queue){
  while(queue->pr[queue->cur_p].first == NULL){
    switch(queue->o){
      case AT_MINIMIZATION: queue->cur_p++;break;
      case AT_MAXIMIZATION: queue->cur_p--;break;
    }
  }
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtPQueueU64*
at_pqueueu64_new(AtOptimization o, AtPolicy po){
  AtPQueueU64* queue     = malloc(sizeof(AtPQueueU64));
  queue->o     = o;
  queue->po    = po;
  queue->ne = 0;
  return queue;
}

AtPQueueU64*
at_pqueueu64_new_prealloc(AtOptimization o, AtPolicy po, uint64_t np, uint64_t nv){
  AtPQueueU64* q     = at_pqueueu64_new(o, po);
  uint64_t  i;
  q->pr = at_queueu64_new_array(np);
  q->np = np;
  q->v  = at_listu64_new_array(nv);
  q->vp = malloc(nv * sizeof(uint64_t));
  for(i = 0; i < nv; i++){
    q->v[i].value = i;
    q->vp[i]      = UINT64_MAX;
  }
  switch(o){
    case AT_MINIMIZATION: q->cur_p = 0;break;
    case AT_MAXIMIZATION: q->cur_p = np-1;break;
  }
  return q;
}
uint8_t
at_pqueueu64_has(AtPQueueU64* q, uint64_t v){
  return q->vp[v] != UINT64_MAX;
}
void
at_pqueueu64_remove_from(AtPQueueU64* q, uint64_t v){
  at_queueu64_remove_link(&q->pr[q->vp[v]],&q->v[v]);
  q->ne--;
}

void
at_pqueueu64_add(AtPQueueU64* q, uint64_t pr, uint64_t v){
  switch(q->po){
    case AT_FIFO:
      at_queueu64_append_link(&q->pr[pr],&q->v[v]);
    break;
    case AT_LIFO:
      at_queueu64_prepend_link(&q->pr[pr],&q->v[v]);
    break;
  }
  // Set in which priority is the element
  q->vp[v] = pr;
  q->ne++;
}

uint64_t
at_pqueueu64_remove(AtPQueueU64* q){
  // Find filled queue
  at_pqueue_find_non_null_priority(q);
  // Update number of elements and erase element priority
  q->ne--;
  q->vp[q->pr[q->cur_p].first->value] = UINT64_MAX;
  // Remove the element from queue
  return at_queueu64_remove_first_link(&q->pr[q->cur_p])->value;
}
uint8_t
at_pqueueu64_is_empty(AtPQueueU64* q){
  return q->ne == 0;
}


void
at_pqueueu64_destroy(AtPQueueU64** qp){
  if(*qp){
    AtPQueueU64* q = *qp;
    if(q->pr) at_queueu64_destroy_array(&q->pr);
    if(q->v)  at_listu64_destroy_array(&q->v);
    if(q->vp) free(q->vp);
    free(*qp);
  }
}
