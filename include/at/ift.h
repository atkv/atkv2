/*Copyright (C) 2016  Anderson Tavares <acmt@outlook.com>

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
#include <at/core.h>

#define at_ift_apply(input) _Generic((input), Array: at_ift_apply_array)

/**
 * Necessary info for IFT
 */
typedef struct IFT{
  // Output
  uint64_t* predecessors;  // 8 bytes - offset 0
  uint64_t* roots;         // 8 bytes - offset 8
  uint8_t * labels;        // 8 bytes - offset 16
  double  * connectivities;// 8 bytes - offset 24
                           // 
                           // Total: 32 bytes
}IFT;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/

typedef double 
(*AtConnectivityFunc) (AtArray* array, IFT* ift, uint64_t s, uint64_t t);

IFT*
at_ift_new();

void
at_ift_init(IFT* ift,
            AtArray* array,
            AtConnectivityFunc connectivity);

IFT*
at_ift_apply_array(AtArray*             array,
                   AtAdjacency        adjacency,
                   AtOptimization     optimization, 
                   AtConnectivityFunc connectivity,
                   AtWeightingFunc    weighting,
                   AtArray*             seeds);
