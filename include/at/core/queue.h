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
#ifndef AT_QUEUE_H
#define AT_QUEUE_H
#include <at/core/macro.h>
AT_BEGIN_DECLS
#include <at/core/list.h>

/*=============================================================================
 STRUCTURE
 ============================================================================*/

/**
 * Queue for 64bit unsigned integers
 */
typedef struct AtQueue_uint64_t{
  AtList_uint64_t* first;// First
  AtList_uint64_t* last;// Last
}AtQueue_uint64_t;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_queue_new
 * @return
 */
AtQueue_uint64_t*
at_queue_uint64_t_new();

/**
 * @brief at_queue_new_array
 * @param n_queues
 * @return
 */
AtQueue_uint64_t*
at_queue_uint64_t_new_array(uint64_t n_queues);

/**
 * @brief at_queue_append_link
 * @param queue
 * @param listitem
 */
void
at_queue_uint64_t_append_link(AtQueue_uint64_t* q, AtList_uint64_t* l);

/**
 * @brief at_queue_prepend_link
 * @param queue
 * @param listitem
 */
void
at_queue_uint64_t_prepend_link(AtQueue_uint64_t* q, AtList_uint64_t* l);

/**
 * @brief at_queue_remove_first_link
 * @param queue
 * @return
 */
AtList_uint64_t*
at_queue_uint64_t_remove_first_link(AtQueue_uint64_t* q);

/**
 * @brief at_queue_uint64_t_remove_link
 * @param q
 * @param l
 */
void
at_queue_uint64_t_remove_link(AtQueue_uint64_t* q, AtList_uint64_t* l);

/**
 * @brief at_queue_destroy
 * @param queue
 */
void
at_queue_uint64_t_destroy_array(AtQueue_uint64_t** qp);
AT_END_DECLS
#endif
