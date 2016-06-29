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
at_queue_uint64_t_prepend_link(AtQueue_uint64_t* q, AtList_uint64_t* l){
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
at_queue_uint64_t_append_link(AtQueue_uint64_t* q, AtList_uint64_t* l){
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

AtList_uint64_t*
at_queue_uint64_t_remove_first_link(AtQueue_uint64_t* q){
  AtList_uint64_t* l = q->first;
  if(l) {
    q->first = l->next;
    l->next = NULL;
  }
  return l;
}

void
at_queue_uint64_t_remove_link(AtQueue_uint64_t* q, AtList_uint64_t* l){
  if(l != NULL){
    if(q->first == l) q->first = l->next;
    if(q->last == l)  q->last  = l->prev;

    // Previous node link to next one and vice-versa
    if(l->prev) l->prev->next = l->next;
    if(l->next) l->next->prev = l->prev;

    // Same as:
    // l->prev = NULL;
    // l->next = NULL;
    memset(l,0,(sizeof(AtList_uint64_t*)<<1));
  }
}

void
at_queue_uint64_t_destroy_array(AtQueue_uint64_t** qp){
  if(*qp != NULL) free(*qp);
  *qp = NULL;
}
