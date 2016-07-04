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
typedef struct AtQueueu64{
  AtListU64* first;// First
  AtListU64* last;// Last
}AtQueueu64;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_queue_new
 * @return
 */
AtQueueu64*
at_queueu64_new();

/**
 * @brief at_queue_new_array
 * @param n_queues
 * @return
 */
AtQueueu64*
at_queueu64_new_array(uint64_t n_queues);

/**
 * @brief at_queue_append_link
 * @param queue
 * @param listitem
 */
void
at_queueu64_append_link(AtQueueu64* q, AtListU64* l);

/**
 * @brief at_queue_prepend_link
 * @param queue
 * @param listitem
 */
void
at_queueu64_prepend_link(AtQueueu64* q, AtListU64* l);

/**
 * @brief at_queue_remove_first_link
 * @param queue
 * @return
 */
AtListU64*
at_queueu64_remove_first_link(AtQueueu64* q);

/**
 * @brief at_queueu64_remove_link
 * @param q
 * @param l
 */
void
at_queueu64_remove_link(AtQueueu64* q, AtListU64* l);

/**
 * @brief at_queue_destroy
 * @param queue
 */
void
at_queueu64_destroy_array(AtQueueu64** qp);
AT_END_DECLS
#endif
