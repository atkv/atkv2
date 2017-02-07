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
#if !defined(AT_IMGPROC_H_INSIDE)
#error "Only <at/imgproc.h> can be included directly."
#endif
#ifndef AT_IFT_H
#define AT_IFT_H
#include <at/core.h>
#include <at/imgproc.h>
AT_BEGIN_DECLS
/**
 * Necessary info for IFT
 */
typedef struct AtIFT{
  // Output
  uint64_t* p;/*!< Predecessors */  /*00+08*/
  uint64_t* r;/*!< Roots */         /*08+08*/
  double  * c;/*!< Connectivities *//*16+08*/
  uint8_t * l;/*!< Labels */        /*24+08*/
              // Total: 32 bytes
}AtIFT;

typedef void
(*AtConnInitu8)(AtIFT* ift, AtArrayU8* array);
typedef void
(*AtConnInitSeeds)(AtIFT* ift, AtSeeds* seeds);
typedef double
(*AtConnFuncu8) (AtIFT* ift, AtGraphArray* graph,
                       uint64_t s, uint64_t t, uint64_t i);

/**
 * @brief information for connectivity function
 */
typedef struct AtConnectivity{
  AtConnInitu8    init; /*!< Initialize non-seeds */
  AtConnInitSeeds seeds;/*!< Initialize seeds */
  AtConnFuncu8    func; /*!< Connectivity value */
  AtOptimization  o;    /*!< Optimization Class */
}AtConnectivity;

AtConnectivity at_conn_max;
AtConnectivity at_conn_maxr;
AtConnectivity at_conn_min;
AtConnectivity at_conn_minr;
AtConnectivity at_conn_sum;
AtConnectivity at_conn_euc;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
#define at_ift_apply(input) _Generic((input), Array: at_ift_apply_array)

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
                     AtGraphArray*        g,
                     AtConnectivity       connectivity,
                     AtSeeds*             seeds,
                     AtPolicy             po);

AtSCC*
at_orfc_core_arrayu8(AtArrayU8     * array,
                     AtGraphArray  * g,
                     AtConnectivity  conn,
                     AtSeeds       * seeds,
                     uint64_t        lblback,
                     AtPolicy        po,
                     AtSCCAlgorithm  sccalgo);
AtIFT*
at_orfc_arrayu8(AtArrayU8*        array,
                AtGraphArray*     g,
                AtConnectivity    conn,
                AtSeeds*          seeds,
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
AtSeeds *at_seeds_from_mask(AtArrayU8* mask);

AT_END_DECLS
#endif
