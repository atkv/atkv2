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
#ifndef AT_GRAPHARRAY_H
#define AT_GRAPHARRAY_H
#include <stdint.h>
#include <at/array.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/

typedef enum {
  AT_ADJACENCY_4      = 4,
  AT_ADJACENCY_8      = 8,
  AT_ADJACENCY_6      = 6,
  AT_ADJACENCY_18     = 18,
  AT_ADJACENCY_26     = 26,
  AT_ADJACENCY_CUSTOM = 1
}AtAdjacency;

/**
 * @brief A directed weighted grid graph
 */
typedef struct AtGraphArray{
  uint64_t* neighbors; // 00-8: indices of neighbors for each node
  double  * weights;   // 08-8: weights of edges <node,neighbor>
  uint8_t * active;    // 16-8: <node,neighbor> is active?
  uint8_t   adjacency; // 24-1: Neighboring
  uint8_t   dim;       // 25-1: Dimension of array
  uint8_t   padding[6];// 26-6: Memory alignment
}AtGraphArray;         // Total: 32B

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
typedef double
(*AtWeightingFunc_uint8_t) (AtArray_uint8_t* graph, uint64_t s, uint64_t t);

/**
 * @brief at_grapharray_create
 * @return
 */
AtGraphArray*
at_grapharray_create();

/**
 * @brief at_grapharray_new_from_array_uint8_t
 * @param array
 * @return
 */
AtGraphArray*
at_grapharray_new_from_array_uint8_t(AtArray_uint8_t* array, AtAdjacency adjacency, AtWeightingFunc_uint8_t weighting);

/**
 * @brief at_grapharray_remove_arc
 * @param grapharray
 * @param s
 * @param t
 */
void
at_grapharray_remove_arc(AtGraphArray* grapharray, uint64_t s, uint64_t t);

/**
 * @brief at_grapharray_add_arc
 * @param grapharray
 * @param s
 * @param t
 */
void
at_grapharray_add_arc(AtGraphArray* grapharray, uint64_t s, uint64_t t);

/**
 * @brief at_grapharray_remove_edge
 * @param grapharray
 * @param s
 * @param t
 */
void
at_grapharray_remove_edge(AtGraphArray* grapharray, uint64_t s, uint64_t t);

/**
 * @brief at_grapharray_add_edge
 * @param grapharray
 * @param s
 * @param t
 */
void
at_grapharray_add_edge(AtGraphArray* grapharray, uint64_t s, uint64_t t);

/**
 * @brief at_grapharray_destroy
 * @param grapharray
 */
void
at_grapharray_destroy(AtGraphArray** grapharray);

#endif
