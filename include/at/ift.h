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
#ifndef AT_IFT_H
#define AT_IFT_H
#include <at/array.h>
#include <at/grapharray.h>
#include <at/optimization.h>

/**
 * Necessary info for IFT
 */
typedef struct AtIFT{
  // Output
  uint64_t* p;// 00+08
  uint64_t* r;// 08+08
  double  * c;// 16+08
  uint8_t * l;// 24+08 = 32
}AtIFT;

typedef void
(*AtConnInit_uint8_t)(AtIFT* ift, AtArray_uint8_t* array);
typedef void
(*AtConnInitSeeds)(AtIFT* ift, AtArray_uint64_t* seeds);
typedef double
(*AtConnFunc_uint8_t) (AtIFT* ift, AtGraphArray* graph,
                       uint64_t s, uint64_t t, uint64_t i);

typedef struct AtConnectivity{
  AtConnInit_uint8_t init;
  AtConnInitSeeds    seeds;
  AtConnFunc_uint8_t func;
}AtConnectivity;

AtConnectivity at_conn_max;
AtConnectivity at_conn_min;
AtConnectivity at_conn_sum;
AtConnectivity at_conn_euc;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
#define at_ift_apply(input) _Generic((input), Array: at_ift_apply_array)

AtArray_uint64_t*
at_seeds_new(uint64_t n, uint64_t* data);


AtIFT*
at_ift_apply_array_uint8_t(AtArray_uint8_t*           array,
                           AtAdjacency                adj,
                           AtOptimization             o,
                           AtConnectivity             connectivity,
                           AtWeightingFunc_uint8_t    w,
                           AtArray_uint64_t*          seeds,
                           AtPolicy                   po);

#endif
