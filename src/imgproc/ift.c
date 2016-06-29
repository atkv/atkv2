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
#include <at/imgproc/ift.h>
#include <at/core/pqueue.h>
#include <math.h>
#include <at/core/macro.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
void
at_conn_max_array_uint8_t(AtIFT* ift, AtArray_uint8_t* array);
void
at_conn_min_array_uint8_t(AtIFT* ift, AtArray_uint8_t* array);
void
at_ift_add_seeds_conn_max(AtIFT* ift, AtArray_uint64_t* seeds);
void
at_ift_add_seeds_conn_min(AtIFT* ift, AtArray_uint64_t* seeds);
double
at_conn_max_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_min_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_sum_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_euc_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);

#define at_conn_sum_array_uint8_t at_conn_max_array_uint8_t
#define at_conn_euc_array_uint8_t at_conn_max_array_uint8_t
#define at_ift_add_seeds_conn_sum at_ift_add_seeds_conn_max
#define at_ift_add_seeds_conn_euc at_ift_add_seeds_conn_max

/**
 * @brief Create and allocate whole IFT structure, as well as its members
 * @param array
 * @param adjacency
 * @param connectivity
 * @return
 */
static AtIFT*
at_ift_new_init_uint8_t(AtArray_uint8_t* array){

  // Allocating data
  size_t   num_bytes = sizeof(AtIFT) + array->h.num_elements*(
                      (sizeof(uint64_t)<<1) + sizeof(double) + sizeof(uint8_t));
  void*    buffer    = malloc(num_bytes);
  AtIFT*   ift       = buffer;
  size_t   size      = (array->h.num_elements << 3);
  uint64_t i;
  size_t   sizeIFT   = sizeof(AtIFT);
  ift->p = (uint64_t*)(ift    + 1);
  ift->r =             ift->p + array->h.num_elements;
  ift->c = (double*)  (ift->r + array->h.num_elements);
  ift->l = (uint8_t*) (ift->c + array->h.num_elements);

  // Initializing arrays
  for(i = 0; i < array->h.num_elements;i++){
    ift->p[i] = i;
    ift->r[i] = i;
  }
  memset(ift->l, 0, (size >> 3));

  // Returning
  return ift;
}

static AtIFT*
at_ift_new(){
  return malloc(sizeof(AtIFT));
}
static void
at_ift_init_uint8_t(AtIFT* ift, AtArray_uint8_t array, AtAdjacency adjacency,
                    AtConnFunc_uint8_t connectivity){
  ift->p = malloc(array.h.num_elements * sizeof(uint64_t));
  ift->r = malloc(array.h.num_elements * sizeof(uint64_t));
  ift->c = malloc(array.h.num_elements * sizeof(double));
  ift->l = malloc(array.h.num_elements * sizeof(uint8_t));
}

__attribute__((constructor))
static void
at_conn_constructor(){
  at_conn_max.init  = at_conn_max_array_uint8_t;
  at_conn_max.seeds = at_ift_add_seeds_conn_max;
  at_conn_max.func       = at_conn_max_func;

  at_conn_min.init  = at_conn_min_array_uint8_t;
  at_conn_min.seeds = at_ift_add_seeds_conn_min;
  at_conn_min.func       = at_conn_min_func;

  at_conn_sum.init  = at_conn_sum_array_uint8_t;
  at_conn_sum.seeds = at_ift_add_seeds_conn_sum;
  at_conn_sum.func       = at_conn_sum_func;

  at_conn_euc.init  = at_conn_euc_array_uint8_t;
  at_conn_euc.seeds = at_ift_add_seeds_conn_euc;
  at_conn_euc.func       = at_conn_euc_func;
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
void
at_ift_add_seeds_to_pqueue(AtIFT* ift, AtPQueue_uint64_t* queue, AtArray_uint64_t* seeds){
  uint64_t i;
  for(i = 0; i < seeds->h.num_elements; i+=2){
    double conn = ift->c[seeds->data[i]];
    if(conn == -INFINITY)
      at_pqueue_uint64_t_add(queue,0,seeds->data[i]);
    else if(conn == INFINITY)
      at_pqueue_uint64_t_add(queue,queue->np-1,seeds->data[i]);
    else
      at_pqueue_uint64_t_add(queue,(uint8_t)conn,seeds->data[i]);
  }
}

// Connectivity Init functions (without seeds)
//--------------------------------------------
void
at_conn_max_array_uint8_t(AtIFT* ift, AtArray_uint8_t* array){
  uint64_t i;
  for(i = 0; i < array->h.num_elements; i++)
    ift->c[i] = INFINITY;
}
void
at_conn_min_array_uint8_t(AtIFT* ift,AtArray_uint8_t* array){
  uint64_t i;
  for(i = 0; i < array->h.num_elements; i++)
    ift->c[i] = -INFINITY;
}

// Adding seeds initially (changing label)
//--------------------------------------------
void
at_ift_add_seeds_labels(AtIFT* ift, AtArray_uint64_t* seeds){
  uint8_t *lb = ift->l;
  register uint64_t nelem  = seeds->h.num_elements;
  register uint64_t i, v, l;
  for(i = 0; i < nelem;){
    v = seeds->data[i++];
    l = seeds->data[i++];
    lb[v] = l;
  }
}
// Adding seeds initially (changing connectivity)
//--------------------------------------------
void
at_ift_add_seeds_conn_max(AtIFT* ift, AtArray_uint64_t* seeds){
  double   *conn = ift->c;
  uint64_t *data = seeds->data;
  register uint64_t nelem  = seeds->h.num_elements;
  register uint64_t i;
  for(i = 0; i < nelem; i+=2){
    conn[data[i]] = -INFINITY;
  }
}
void
at_ift_add_seeds_conn_min(AtIFT* ift, AtArray_uint64_t* seeds){
  // find max value
  double   *conn = ift->c;
  uint64_t *data = seeds->data;
  register uint64_t nelem  = seeds->h.num_elements;
  register uint64_t i;
  for(i = 0; i < nelem; i+=2){
    conn[data[i]] = INFINITY;
  }
}


// Connectivity functions (when calculating IFT)
//--------------------------------------------

double at_conn_max_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  return max(ift->c[s],graph->weights[i]);
}

double at_conn_min_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  return min(ift->c[s],graph->weights[i]);
}

double at_conn_sum_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  return ift->c[s] + graph->weights[i];
}

double at_conn_euc_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  uint64_t* step = graph->h->step;
  uint64_t  s_nd[4];
  uint64_t  t_nd[4];
  double    d, sum;
  uint8_t   dim   = graph->h->dim;
  uint8_t   k;
  at_index_to_nd(dim, step, s, s_nd);
  at_index_to_nd(dim, step, t, t_nd);
  for(k = 0; k < dim; k++){
    d = ((double)s_nd[k] - (double)t_nd[k]);
    sum += d*d;
  }
  return sqrt(sum);
}

AtArray_uint64_t*
at_seeds_new(uint64_t n, uint64_t* data){
  uint64_t shape[2] = {n,2};
  return at_array_uint64_t_new_with_data(2,shape,data,true);
}

AtIFT*
at_ift_apply_array_uint8_t(AtArray_uint8_t*           ar,
                           AtAdjacency                adj,
                           AtOptimization             o,
                           AtConnectivity             c,
                           AtWeightingFunc_uint8_t    w,
                           AtArray_uint64_t*          seeds,
                           AtPolicy                   po){
  AtIFT            * ift;         // 00+08: ift structure (the result)
  AtGraphArray     * g;           // 08+08: graph array structure
  AtPQueue_uint64_t* q;           // 16+08: priority queue structure
  uint8_t          * r;           // 24+08
  double             newc;        // 32+08: new neighbor connectivity (if better)
  uint64_t           s;           // 40+08: index of current node in array
  uint64_t           t;           // 48+08: index of current neighbor in array
  uint64_t           off;         // 56+08: index of current node in grapharray (for loop)
  uint64_t           offn;        // 64+08: index of next node in grapharray (for loop)
  uint16_t           pr;          // 72+02: priority
  uint8_t            i;           // 74+01: counter
  uint8_t            vmax;        // 75+01: max array value (for allocating the queue)
  uint8_t            pd[4];       // 76+04: padding for alignment
                                  // Total: 80 bytes
                           
  // Create the auxiliary structures
  r     = calloc(ar->h.num_elements,sizeof(uint8_t));
  ift   = at_ift_new_init_uint8_t(ar);
  g     = at_grapharray_new(ar,adj,w);
  vmax  = at_array_uint8_t_max(ar);

  // Initialize IFT structure values and add seeds
  c.init (ift,ar);
  c.seeds(ift,seeds);
  at_ift_add_seeds_labels(ift, seeds);

  // Create priority queue and add seeds
  q = at_pqueue_uint64_t_new_prealloc(o,po,(uint64_t)vmax+2,ar->h.num_elements);
  at_ift_add_seeds_to_pqueue(ift,q,seeds);

  // Main loop
  while(!at_pqueue_uint64_t_is_empty(q)){
    // get best node
    s    = at_pqueue_uint64_t_remove(q);
    r[s] = 1;
    // loop through neighbors
    off  = s   * adj;
    offn = off + adj;
    for(i = off; i < offn; i++){
      if(g->active[i]){
        t = g->neighbors[i];
        // Still not processed?
        if(!r[t]){
          newc = c.func(ift, g, s, t, i);
          if((o == AT_MAXIMIZATION && ift->c[t] < newc)||
             (o == AT_MINIMIZATION && ift->c[t] > newc)){
            if(at_pqueue_uint64_t_has(q,t))
              at_pqueue_uint64_t_remove_from(q,t);
            ift->c[t] = newc;
            ift->p[t] = s;
            ift->r[t] = ift->r[s];
            ift->l[t] = ift->l[s];
            pr = (uint16_t)ift->c[t];
            if(o == AT_MINIMIZATION) pr++;
            at_pqueue_uint64_t_add(q, pr, t);
          }
        }
      }
    }
  }

  // Clear memory
  at_pqueue_uint64_t_destroy(&q);
  at_grapharray_destroy(&g);
  free(r);
  return ift;
}
