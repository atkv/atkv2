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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif
#ifndef AT_QUEUE_H
#define AT_QUEUE_H

#include <at/core.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/

/**
 * Queue for 64bit unsigned integers
 */
typedef struct AtQueueu64{
  AtListU64* first;/*!< First */
  AtListU64* last;/*!< Last */
}AtQueueu64;

/**
  * @brief AtQueue structure
  */
typedef struct _AtQueue{
  uint32_t   length; /**< Number of elements */
  AtList  * begin;  /**< Initial element */
  AtList  * end;    /**< Final element */
}AtQueue;
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

/**
 * @brief Alocar uma fila
 * @return
 */
AtQueue* at_queue_new();
/**
 * @brief Liberar a fila
 * @return
 */
void at_queue_free(AtQueue* queue);
/**
 * @brief Adicionar no início
 * @return
 */
void at_queue_prepend(AtQueue* queue, void* value);
/**
 * @brief Adicionar no final
 * @return
 */
void at_queue_append(AtQueue* queue, void* value);
/**
 * @brief Adicionar antes de um item
 * @return
 */
void at_queue_prepend_at(AtQueue* queue, AtList* item, void* value);
/**
 * @brief Adicionar após um item
 * @return
 */
void at_queue_append_at(AtQueue* queue, AtList* item, void* value);
/**
 * @brief Adicionar antes de item especificado pela sua posição
 * @return
 */
void at_queue_prepend_at_index(AtQueue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar após um item especificado pela sua posição
 * @return
 */
void at_queue_append_at_index(AtQueue* queue, uint32_t index, void* value);
/**
 * @brief Adicionar um item AtList no início da fila
 * @return
 */
void at_queue_prepend_item(AtQueue* queue, AtList* new_item);
/**
 * @brief Adicionar um item AtList no fim da fila
 * @return
 */
void at_queue_append_item(AtQueue* queue, AtList* new_item);
/**
 * @brief Adicionar um item AtList antes de um item
 * @return
 */
void at_queue_prepend_item_at(AtQueue* queue, AtList* item, AtList* new_item);
/**
 * @brief Adicionar um item AtList depois de um item
 * @return
 */
void at_queue_append_item_at(AtQueue* queue, AtList* item, AtList* new_item);
/**
 * @brief Adicionar um item AtList antes de um item especificado pela sua posição
 * @return
 */
void at_queue_prepend_item_at_index(AtQueue* queue, uint32_t index, AtList* new_item);
/**
 * @brief Adicionar um item AtList depois de um item especificado pela sua posição
 * @return
 */
void at_queue_append_item_at_index(AtQueue* queue, uint32_t index, AtList* new_item);
/**
 * @brief Get first item
 * @return
 */
AtList* at_queue_begin(AtQueue* queue);
/**
 * @brief Get last item
 * @return
 */
AtList* at_queue_end(AtQueue* queue);
/**
 * @brief True if length is zero
 * @return
 */
uint8_t at_queue_is_empty(AtQueue* queue);
/**
 * @brief Inverte a fila
 * @return
 */
void at_queue_reverse(AtQueue* queue);
/**
 * @brief ordena a fila
 * @return
 */
void at_queue_sort(AtQueue* queue, AtCompareDataFunc compare_function, void* data);
/**
 * @brief copia a fila
 * @return
 */
AtQueue* at_queue_copy(AtQueue* queue);
/**
 * @brief vectorize a function within a queue
 * @return
 */
void at_queue_foreach(AtQueue* queue, AtDataFunc func, void* data);
/**
 * @brief posição de um item especificado pelo seu valor
 * @return
 */
int32_t at_queue_index_of(AtQueue* queue, void* value);
/**
 * @brief obter um item a partir de sua posição
 * @return
 */
AtList* at_queue_at(AtQueue* queue, uint32_t index);
/**
 * @brief obter o valor do item a partir de sua posição
 * @return
 */
void* at_queue_value_at(AtQueue* queue, uint32_t index);
/**
 * @brief obter o valor do item inicial
 * @return
 */
void* at_queue_begin_value(AtQueue* queue);
/**
 * @brief obter o valor do item final
 * @return
 */
void* at_queue_end_value(AtQueue* queue);
/**
 * @brief Remover um item em uma fila
 * @return
 */
void at_queue_remove(AtQueue* queue, void* value);
/**
 * @brief Remover um item em uma fila baseado em sua posição
 * @return
 */
void* at_queue_remove_at(AtQueue* queue, uint32_t index);
/**
 * @brief at_queue_remove_begin
 * @param queue
 * @return
 */
void* at_queue_remove_begin(AtQueue* queue);
/**
 * @brief at_queue_remove_end
 * @param queue
 * @return
 */
void* at_queue_remove_end(AtQueue* queue);
/**
 * @brief Obter o número de elementos da fila
 * @return
 */
uint32_t at_queue_length(AtQueue* queue);
/**
 * @brief at_queue_append_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void at_queue_append_sorted(AtQueue* queue, AtCompareFunc compare_function, void* value);
/**
 * @brief at_queue_append_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 */
void at_queue_append_sorted_with_data(AtQueue* queue, AtCompareDataFunc compare_function, void* value, void* user_data);
/**
 * @brief at_queue_prepend_sorted
 * @param queue
 * @param compare_function
 * @param value
 */
void at_queue_prepend_sorted(AtQueue* queue, AtCompareFunc compare_function, void* value);
/**
 * @brief at_queue_prepend_sorted_with_data
 * @param queue
 * @param compare_function
 * @param value
 * @param user_data
 */
void at_queue_prepend_sorted_with_data(AtQueue* queue, AtCompareDataFunc compare_function, void* value, void* user_data);

AT_END_DECLS

#endif
