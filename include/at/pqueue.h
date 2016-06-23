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
#include <stdint.h>
#include <at/optimization.h>
#include <at/queue.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
#define AtPQueue(type) AtPQueue##type
#define at_pqueue_new(queue_ptr,optimization,policy) _Generic((queue_ptr), \
AtPQueue_uint64_t**: at_pqueue_uint64_t_new_ptr)(queue_ptr, optimization, policy)


/**
 * @brief Priority Queue Structure
 */
typedef struct AtPQueue_uint64_t{
  AtQueue_uint64_t* priorities;
  AtList_uint64_t*  values;
  uint64_t          cur_priority;
  AtOptimization    optimization;
  AtPolicy          policy;
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
at_pqueue_uint64_t_new(AtOptimization optimization, AtPolicy policy);

/**
 * @brief at_pqueue_uint64_t_new_ptr
 * @param queue_ptr
 * @param optimization
 * @param policy
 */
void
at_pqueue_uint64_t_new_ptr(AtPQueue_uint64_t** queue_ptr, AtOptimization optimization, AtPolicy policy);

/**
 * @brief at_pqueue_new_prealloc
 * @param optimization
 * @param n_buckets
 * @param n_values
 * @return
 */
AtPQueue_uint64_t*
at_pqueue_uint64_t_new_prealloc(AtOptimization optimization, AtPolicy policy, uint64_t n_priorities, uint64_t n_values);

/**
 * @brief at_pqueue_add
 * @param queue
 * @param bucket
 * @param value
 */
void
at_pqueue_uint64_t_add(AtPQueue_uint64_t* queue, uint64_t priority, uint64_t value);

/**
 * @brief at_pqueue_remove
 * @param queue
 * @return
 */
uint64_t
at_pqueue_uint64_t_remove(AtPQueue_uint64_t* queue);

/**
 * @brief at_pqueue_destroy
 * @param queue_ptr
 */
void
at_pqueue_uint64_t_destroy(AtPQueue_uint64_t** queue_ptr);

#endif
