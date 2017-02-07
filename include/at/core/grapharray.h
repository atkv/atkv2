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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif

#ifndef AT_GRAPHARRAY_H
#define AT_GRAPHARRAY_H
#include <stdint.h>
#include <at/core.h>
AT_BEGIN_DECLS
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
  uint64_t* neighbors;   /*!< indices of neighbors for each node *//*00-8*/
  AtArrayHeader* h;      /*!< dim, shape, step... */               /*08-8*/
  double  * weights;     /*!< weights of edges <node,neighbor> */  /*16-8*/
  uint8_t * active;      /*!< <node,neighbor> is active? */        /*24-8*/
  uint8_t   adjacency;   /*!< Neighboring */                       /*32-1*/
  uint8_t   padding[6];  /*!< Memory alignment */                  /*33-7*/
}AtGraphArray;           /* Total: 40B

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
#define at_grapharray_new(array, adjacency, weighting) _Generic((array), \
  AtArrayU8*: at_grapharrayu8_new)(array, adjacency, weighting)
typedef double
(*AtWeightingFuncu8) (AtArrayU8* graph, uint64_t s, uint64_t t, void* params);

/**
 * @brief Weighting information
 */
typedef struct AtWeighting{
  AtWeightingFuncu8 f; /*!< weighting function for arc <s,t> */
  void* params;        /*!< pointer to params (number, structure...)*/
}AtWeighting;

AtWeighting at_wdiffabs;
AtWeighting at_wdiffabsc;
AtWeighting at_wdiffabscalpha;

// Weighting functions
//---------------------
/**
 * @brief at_weighting_diff_abs
 * @param array
 * @param s
 * @param t
 * @return
 */
double
at_weighting_diff_abs(AtArrayU8* array, uint64_t s, uint64_t t, void* params);
/**
 * @brief at_weighting_diff_absc
 * @param array
 * @param s
 * @param t
 * @return
 */
double
at_weighting_diff_absc(AtArrayU8* array, uint64_t s, uint64_t t, void* params);
/**
 * @brief at_weighting_diff_absc_alpha
 * @param array
 * @param s
 * @param t
 * @param params
 * @return
 */
double
at_weighting_diff_absc_alpha(AtArrayU8* array, uint64_t s, uint64_t t, void* params);

// GraphArray functions
//---------------------

/**
 * @brief at_grapharray_create
 * @return
 */
AtGraphArray*
at_grapharray_create();

/**
 * @brief at_grapharray_new_from_arrayu8
 * @param array
 * @return
 */
AtGraphArray*
at_grapharrayu8_new(AtArrayU8*  array,
                    AtAdjacency adjacency,
                    AtWeighting w);
/**
 * @brief at_grapharrayu8_renew
 * @param g
 */
void
at_grapharrayu8_renew_edges(AtGraphArray* g);
/**
 * @brief at_grapharray_remove_arc
 * @param g
 * @param s
 * @param t
 */
void
at_grapharray_remove_arc(AtGraphArray* g, uint64_t s, uint64_t t);
/**
 * @brief at_grapharray_remove_arcs
 * @param g
 * @param pairs
 * @param n
 */
void
at_grapharray_remove_arcs(AtGraphArray* g, uint64_t* pairs, uint64_t n);

/**
 * @brief at_grapharray_add_arc
 * @param g
 * @param s
 * @param t
 */
void
at_grapharray_add_arc(AtGraphArray* g, uint64_t s, uint64_t t);

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
 * @brief at_grapharray_get
 * @param graph
 * @param s
 * @param t
 * @return
 */
double
at_grapharray_get(AtGraphArray* graph, uint64_t s, uint64_t t);
/**
 * @brief Get index for reverse edge
 * @param g
 * @param s
 * @param t
 * @return
 */
uint64_t
at_grapharray_get_indexr(AtGraphArray* g, uint64_t s, uint64_t t);
/**
 * @brief at_grapharray_destroy
 * @param grapharray
 */
void
at_grapharray_destroy(AtGraphArray** grapharray);
AT_END_DECLS
#endif
