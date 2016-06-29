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
#ifndef AT_PQUEUE_H
#define AT_PQUEUE_H
#include <at/core/macro.h>
AT_BEGIN_DECLS
#include <stdint.h>
#include <at/core/optimization.h>
#include <at/core/queue.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
#define AtPQueue(type) AtPQueue##type
#define at_pqueue_new(queue,optimization,policy) queue = _Generic((queue), \
  AtPQueue_uint64_t*: at_pqueue_uint64_t_new)(optimization, policy)

/**
 * @brief Priority Queue Structure
 */
typedef struct AtPQueue_uint64_t{
  AtQueue_uint64_t* pr;
  AtList_uint64_t*  v;
  uint64_t*         vp;
  uint64_t          cur_p;
  uint32_t          ne;
  uint32_t          np;
  AtOptimization    o;
  AtPolicy          po;
}AtPQueue_uint64_t;
/*=============================================================================
 PUBLIC API
 ============================================================================*/

/**
 * @brief at_pqueue_new
 * @param optimization
 * @return
 */
AtPQueue_uint64_t*
at_pqueue_uint64_t_new(AtOptimization o, AtPolicy po);

/**
 * @brief at_pqueue_new_prealloc
 * @param optimization
 * @param n_buckets
 * @param n_values
 * @return
 */
AtPQueue_uint64_t*
at_pqueue_uint64_t_new_prealloc(AtOptimization o, AtPolicy po, uint64_t np, uint64_t nv);

/**
 * @brief at_pqueue_add
 * @param queue
 * @param bucket
 * @param value
 */
void
at_pqueue_uint64_t_add(AtPQueue_uint64_t* q, uint64_t pr, uint64_t v);

/**
 * @brief at_pqueue_remove
 * @param queue
 * @return
 */
uint64_t
at_pqueue_uint64_t_remove(AtPQueue_uint64_t* q);

/**
 * @brief at_pqueue_uint64_t_remove_from
 * @param q
 * @param v
 */
void
at_pqueue_uint64_t_remove_from(AtPQueue_uint64_t* q, uint64_t v);

/**
 * @brief at_pqueue_uint64_t_is_empty
 * @param q
 * @return
 */
uint8_t
at_pqueue_uint64_t_is_empty(AtPQueue_uint64_t* q);

/**
 * @brief at_pqueue_uint64_t_has
 * @param q
 * @param pr
 * @param v
 * @return
 */
uint8_t
at_pqueue_uint64_t_has(AtPQueue_uint64_t* q, uint64_t v);

/**
 * @brief at_pqueue_destroy
 * @param queue_ptr
 */
void
at_pqueue_uint64_t_destroy(AtPQueue_uint64_t** qp);
AT_END_DECLS
#endif
