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
#ifndef AT_LIST_H
#define AT_LIST_H
#include <at/core/macro.h>
AT_BEGIN_DECLS
#include <stdint.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct AtListU64 AtListU64;
/**
 * @brief List 64 bits
 */
struct AtListU64{
  AtListU64* prev; /*!< Previous list item */
  AtListU64* next; /*!< Next list item */
  uint64_t value;  /*!< Value of current item */
};

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_listu64_new_array
 * @param n_items
 * @return
 */
AtListU64*
at_listu64_new_array(uint64_t n_items);
/**
 * @brief at_listu64_destroy
 * @param list_ptr
 */
void
at_listu64_destroy(AtListU64** list_ptr);
/**
 * @brief at_listu64_destroy_array
 * @param list
 */
void
at_listu64_destroy_array(AtListU64** list);

AT_END_DECLS
#endif
