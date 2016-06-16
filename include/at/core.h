/*Copyright (C) 2016  Anderson <acmt@outlook.com>

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>

typedef enum {
  AT_ADJACENCY_4      = 4,
  AT_ADJACENCY_8      = 8,
  AT_ADJACENCY_6      = 6,
  AT_ADJACENCY_18     = 18,
  AT_ADJACENCY_26     = 26,
  AT_ADJACENCY_CUSTOM = 1
}AtAdjacency;

typedef enum {
  AT_MINIMIZATION,
  AT_MAXIMIZATION
}AtOptimization;

/*
Multidimensional Array


*/




/**
 * Graph in a Grid-Style
 */
typedef struct AtGraphArray{
  uint64_t* neighbors;   // indices of the neighbors for each node 
                         // (8 bytes - offset 0)
  double  * weights;     // weights of edge between the neighbor and the node
                         // (8 bytes - offset 8)
  uint8_t * active;      // active edge?
                         // (8 bytes - offset 16)
                         // Total: 24 bytes
}AtGraphArray;

typedef struct AtQueue{

}AtQueue;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/




/* QUEUE FUNCTIONS
 -----------------------------*/
AtQueue*
at_queue_new();

void
at_queue_init(AtQueue* queue, AtArray_uint64_t* seeds);

uint8_t
at_queue_is_empty(AtQueue* queue);

void
at_queue_add(AtQueue* queue, uint8_t bucket, uint64_t item);

uint64_t
at_queue_remove(AtQueue* queue);

void
at_queue_destroy(AtQueue* queue);

/* GRAPH ARRAY FUNCTIONS
 -----------------------------*/
typedef double
(*AtWeightingFunc_uint8_t) (AtGraphArray* graph, uint64_t s, uint64_t t);


AtGraphArray*
at_grapharray_new();

void
at_grapharray_init(AtGraphArray* graph,
                   AtArray* array,
                   AtAdjacency adjacency,
                   AtWeightingFunc weighting);
void
at_grapharray_destroy(AtGraphArray* graph);
