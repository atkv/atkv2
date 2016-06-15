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

#ifndef AT_ARRAY_H
#define AT_ARRAY_H

/**
 * Multidimensional Array 
 */
typedef struct AtArrayHeader{
  uint64_t* shape;       // 0x00 + 0x08: size of each dimension
  uint64_t* step;        // 0x08 + 0x08: offset between consecutive elements
                         //              of each dimension
  uint8_t   dim;         // 0x10 + 0x01: dimension
  uint8_t   alignment[7];// 0x11 + 0x07: Explicit padding
}AtArrayHeader;          // Total: 0x18 = 24B

#define AtArray(type) AtArray_##type

typedef struct AtArray_uint8_t{
  AtArrayHeader header;  // 0x00 + 0x18
  uint8_t      *data;    // 0x18 + 0x08
}AtArray_uint8_t;        // Total: 0x20 = 32 bytes
typedef struct AtArray_uint16_t{
  AtArrayHeader header;
  uint16_t     *data;
}AtArray_uint16_t;
typedef struct AtArray_uint32_t{
  AtArrayHeader header;
  uint32_t     *data;
}AtArray_uint32_t;
typedef struct AtArray_uint64_t{
  AtArrayHeader header;
  uint64_t     *data;
}AtArray_uint64_t;
typedef struct AtArray_int8_t{
  AtArrayHeader header;
  int8_t       *data;
}AtArray_int8_t;
typedef struct AtArray_int16_t{
  AtArrayHeader header;
  int16_t      *data;
}AtArray_int16_t;
typedef struct AtArray_int32_t{
  AtArrayHeader header;
  int32_t      *data;
}AtArray_int32_t;
typedef struct AtArray_int64_t{
  AtArrayHeader header;
  int64_t      *data;
}AtArray_int64_t;
typedef struct AtArray_float{
  AtArrayHeader header;
  float        *data;
}AtArray_float;
typedef struct AtArray_double{
  AtArrayHeader header;
  double       *data;
}AtArray_double;

#endif


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

/* ARRAY FUNCTIONS
 -----------------------------*/

// uint8_t
// --------
AtArray_uint8_t*
at_array_uint8_t_new(uint8_t dim, uint64_t* shape);

AtArray_uint8_t*
at_array_uint8_t_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data);

AtArray_uint8_t*
at_array_uint8_t_zeros(uint8_t dim, uint64_t* shape);

AtArray_uint8_t*
at_array_uint8_t_ones(uint8_t dim, uint64_t* shape);

AtArray_uint8_t*
at_array_uint8_t_fill(AtArray_uint8_t* array, uint8_t value);

void
at_array_uint8_t_destroy(AtArray_uint8_t* array);

// uint64_t
// --------


AtArray_uint64_t*
at_array_uint64_t_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data);


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
(*AtWeightingFunc) (AtArray* array, uint64_t s, uint64_t t);


AtGraphArray*
at_grapharray_new();

void
at_grapharray_init(AtGraphArray* graph,
                   AtArray* array,
                   AtAdjacency adjacency,
                   AtWeightingFunc weighting);
void
at_grapharray_destroy(AtGraphArray* graph);
