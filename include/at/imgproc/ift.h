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
#include <at/core/array.h>
#include <at/core/grapharray.h>
#include <at/core/optimization.h>
#include <at/core/macro.h>
AT_BEGIN_DECLS
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
(*AtConnInitu8)(AtIFT* ift, AtArrayU8* array);
typedef void
(*AtConnInitSeeds)(AtIFT* ift, AtArrayU64* seeds);
typedef double
(*AtConnFuncu8) (AtIFT* ift, AtGraphArray* graph,
                       uint64_t s, uint64_t t, uint64_t i);

typedef struct AtConnectivity{
  AtConnInitu8 init;
  AtConnInitSeeds    seeds;
  AtConnFuncu8 func;
}AtConnectivity;

AtConnectivity at_conn_max;
AtConnectivity at_conn_min;
AtConnectivity at_conn_sum;
AtConnectivity at_conn_euc;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
#define at_ift_apply(input) _Generic((input), Array: at_ift_apply_array)

AtArrayU64*
at_seeds_new(uint64_t n, uint64_t* data);


AtIFT*
at_ift_apply_arrayu8(AtArrayU8*           array,
                           AtAdjacency                adj,
                           AtOptimization             o,
                           AtConnectivity             connectivity,
                           AtWeightingFuncu8    w,
                           AtArrayU64*          seeds,
                           AtPolicy                   po);
AT_END_DECLS
#endif
