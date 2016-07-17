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
#include <at/core/scc.h>
AT_BEGIN_DECLS
/**
 * Necessary info for IFT
 */
typedef struct AtIFT{
  // Output
  uint64_t* p;/*00+08*//*!< Predecessors */
  uint64_t* r;/*08+08*//*!< Roots */
  double  * c;/*16+08*//*!< Connectivities */
  uint8_t * l;/*24+08*//*!< Labels */
              // Total: 32 bytes
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
/**
 * @brief at_seeds_new
 * @param n
 * @param data
 * @return
 */
AtArrayU64*
at_seeds_new(uint64_t n, uint64_t* data);

/**
 * @brief at_ift_apply_arrayu8
 * @param array
 * @param adj
 * @param o
 * @param connectivity
 * @param w
 * @param seeds
 * @param po
 * @return
 */
AtIFT*
at_ift_apply_arrayu8(AtArrayU8*           array,
                     AtAdjacency          adj,
                     AtOptimization       o,
                     AtConnectivity       connectivity,
                     AtWeightingFuncu8    w,
                     AtArrayU64*          seeds,
                     AtPolicy             po);

AtSCC*
at_ift_orfc_core_arrayu8(AtArrayU8*        array,
                         AtAdjacency       adj,
                         AtOptimization    o,
                         AtConnectivity    conn,
                         AtWeightingFuncu8 w,
                         AtArrayU64*       seeds,
                         uint64_t          lblback,
                         AtPolicy          po,
                         AtSCCAlgorithm    sccalgo);
AtIFT*
at_orfc_arrayu8(AtArrayU8*        array,
                AtAdjacency       adj,
                AtOptimization    o,
                AtConnectivity    conn,
                AtWeightingFuncu8 w,
                AtArrayU64*       seeds,
                uint64_t          lblback,
                AtPolicy          po);
/**
 * @brief Create the seeds from a mask (non-zero labels)
 *
 * If mask(v) != 0, for any node v, we add the pair (v, mask(v)) to the resulting array
 *
 * @param mask
 * @return
 */
AtArrayU64*
at_seeds_from_mask(AtArrayU8* mask);

AT_END_DECLS
#endif
