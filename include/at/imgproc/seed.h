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
#ifndef AT_SEED_H
#define AT_SEED_H
#include<at/core.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/
/**
 * @brief Seeds (being a compact list or mask)
 */
typedef struct AtSeeds{
  uint64_t*   s; //00+8: seeds
  uint8_t*    l; //08+8: labels
  uint64_t    n; //16+8: number of seeds
  AtArrayU8 * m; //24+8: mask
}AtSeeds;        //32

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
/**
 * @brief at_seeds_from_list
 * @param num_seeds
 * @param slist
 * @param labels
 * @return
 */
AtSeeds*
at_seeds_from_list(uint64_t num_seeds, uint64_t* slist, uint8_t* labels);
/**
 * @brief at_seeds_from_mask
 * @param mask
 * @return
 */
AtSeeds*
at_seeds_from_mask(AtArrayU8* mask);
/**
 * @brief at_seeds_mask_generate
 * @param seeds
 * @param dim
 * @param shape
 */
void
at_seeds_mask_generate(AtSeeds* seeds, uint8_t dim, uint64_t* shape);

/**
 * @brief at_seeds_split
 * @param seeds
 * @param sbackp
 * @param sobjp
 * @param lblback
 */
void
at_seeds_split(AtSeeds* seeds, AtSeeds** sbackp, AtSeeds** sobjp, uint64_t lblback);
/**
 * @brief at_seeds_new
 * @param n
 * @param seeds
 * @param labels
 * @return
 */
AtSeeds*
at_seeds_new(uint64_t n, uint64_t* seeds, uint8_t* labels);

void
at_seeds_destroy(AtSeeds** seeds);

AT_END_DECLS
#endif
