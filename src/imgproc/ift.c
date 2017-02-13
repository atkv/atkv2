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
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <at/imgproc.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
void
at_conn_max_arrayu8(AtIFT* ift, AtArrayU8* array);
void
at_conn_min_arrayu8(AtIFT* ift, AtArrayU8* array);
void
at_ift_add_seeds_conn_max(AtIFT* ift, AtSeeds *seeds);
void
at_ift_add_seeds_conn_min(AtIFT* ift, AtSeeds* seeds);
double
at_conn_max_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_maxr_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_min_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_minr_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_sum_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);
double
at_conn_euc_func(AtIFT* ift, AtGraphArray* graph,uint64_t s, uint64_t t, uint64_t i);

#define at_conn_sum_arrayu8 at_conn_max_arrayu8
#define at_conn_euc_arrayu8 at_conn_max_arrayu8
#define at_ift_add_seeds_conn_sum at_ift_add_seeds_conn_max
#define at_ift_add_seeds_conn_euc at_ift_add_seeds_conn_max

static void
at_ift_initu8(AtArrayU8* array, AtIFT* ift){
  // Initializing arrays
  uint64_t i;
  for(i = 0; i < array->h.nelem;i++){
    ift->p[i] = i;
    ift->r[i] = i;
  }
  memset(ift->l, 0, array->h.nelem);
}

/**
 * @brief Create and allocate whole IFT structure, as well as its members
 * @param array
 * @param adjacency
 * @param connectivity
 * @return
 */
static AtIFT*
at_ift_new_initu8(AtArrayU8* array){

  // Allocating data
  size_t   num_bytes = sizeof(AtIFT) + array->h.nelem*(
                      (sizeof(uint64_t)<<1) + sizeof(double) + sizeof(uint8_t));
  void*    buffer    = malloc(num_bytes);
  AtIFT*   ift       = buffer;
  size_t   size      = (array->h.nelem << 3);
  uint64_t i;
  size_t   sizeIFT   = sizeof(AtIFT);
  ift->p = (uint64_t*)(ift    + 1);
  ift->r =             ift->p + array->h.nelem;
  ift->c = (double*)  (ift->r + array->h.nelem);
  ift->l = (uint8_t*) (ift->c + array->h.nelem);

  // Returning
  return ift;
}

static AtIFT*
at_ift_new(){
  return malloc(sizeof(AtIFT));
}


__attribute__((constructor))
static void
at_conn_constructor(){
  at_conn_max.init  = at_conn_max_arrayu8;
  at_conn_max.seeds = at_ift_add_seeds_conn_max;
  at_conn_max.func  = at_conn_max_func;
  at_conn_max.o     = AT_MINIMIZATION;

  at_conn_maxr.init = at_conn_max_arrayu8;
  at_conn_maxr.seeds= at_ift_add_seeds_conn_max;
  at_conn_maxr.func = at_conn_maxr_func;
  at_conn_maxr.o     = AT_MINIMIZATION;

  at_conn_min.init  = at_conn_min_arrayu8;
  at_conn_min.seeds = at_ift_add_seeds_conn_min;
  at_conn_min.func  = at_conn_min_func;
  at_conn_min.o     = AT_MAXIMIZATION;

  at_conn_minr.init = at_conn_min_arrayu8;
  at_conn_minr.seeds= at_ift_add_seeds_conn_min;
  at_conn_minr.func = at_conn_minr_func;
  at_conn_minr.o     = AT_MAXIMIZATION;

  at_conn_sum.init  = at_conn_sum_arrayu8;
  at_conn_sum.seeds = at_ift_add_seeds_conn_sum;
  at_conn_sum.func  = at_conn_sum_func;
  at_conn_sum.o     = AT_MINIMIZATION;

  at_conn_euc.init  = at_conn_euc_arrayu8;
  at_conn_euc.seeds = at_ift_add_seeds_conn_euc;
  at_conn_euc.func  = at_conn_euc_func;
  at_conn_euc.o     = AT_MINIMIZATION;
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
void
at_ift_add_seeds_to_pqueue(AtIFT* ift, AtPQueueU64* queue, AtSeeds* seeds){
  uint64_t i;
  uint64_t s;
  for(i = 0; i < seeds->n; i++){
    s = seeds->s[i];
    double conn = ift->c[s];
    if(conn == -INFINITY)
      at_pqueueu64_add(queue,0,s);
    else if(conn == INFINITY)
      at_pqueueu64_add(queue,queue->np-1,s);
    else
      at_pqueueu64_add(queue,(uint8_t)conn,s);
  }
}

// Connectivity Init functions (without seeds)
//--------------------------------------------
void
at_conn_max_arrayu8(AtIFT* ift, AtArrayU8* array){
  uint64_t i;
  for(i = 0; i < array->h.nelem; i++)
    ift->c[i] = INFINITY;
}
void
at_conn_min_arrayu8(AtIFT* ift,AtArrayU8* array){
  uint64_t i;
  for(i = 0; i < array->h.nelem; i++)
    ift->c[i] = -INFINITY;
}

// Adding seeds initially (changing label)
//--------------------------------------------
void
at_ift_add_seeds_labels(AtIFT* ift, AtSeeds* seeds){
  uint8_t *lb = ift->l;
  register uint64_t nelem  = seeds->n;
  register uint64_t i, v, l;
  for(i = 0; i < nelem;i++){
    v = seeds->s[i];
    l = seeds->l[i];
    lb[v] = l;
  }
}
// Adding seeds initially (changing connectivity)
//--------------------------------------------
void
at_ift_add_seeds_conn_max(AtIFT* ift, AtSeeds* seeds){
  double   *conn = ift->c;
  uint64_t *data = seeds->s;
  register uint64_t nelem  = seeds->n;
  register uint64_t i;
  for(i = 0; i < nelem; i++){
    conn[data[i]] = -INFINITY;
  }
}
void
at_ift_add_seeds_conn_min(AtIFT* ift, AtSeeds* seeds){
  // find max value
  double   *conn = ift->c;
  uint64_t *data = seeds->s;
  register uint64_t nelem  = seeds->n;
  register uint64_t i;
  for(i = 0; i < nelem; i++){
    conn[data[i]] = INFINITY;
  }
}


// Connectivity functions (when calculating IFT)
//--------------------------------------------

double at_conn_max_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  return max(ift->c[s],graph->weights[i]);
}

double at_conn_maxr_func(AtIFT* ift, AtGraphArray* graph,
                         uint64_t s, uint64_t t, uint64_t i){
  return max(ift->c[s],graph->weights[at_grapharray_get_indexr(graph,s,t)]);
}

double at_conn_min_func(AtIFT* ift, AtGraphArray* graph,
                        uint64_t s, uint64_t t, uint64_t i){
  return min(ift->c[s],graph->weights[i]);
}

double at_conn_minr_func(AtIFT* ift, AtGraphArray* graph,
                         uint64_t s, uint64_t t, uint64_t i){
  uint64_t off = at_grapharray_get_indexr(graph,s,t);
  return min(ift->c[s],graph->weights[off]);
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

AtIFT*
at_ift_apply_arrayu8(AtArrayU8     *ar,
                     AtGraphArray  *g,
                     AtConnectivity c,
                     AtSeeds       *seeds,
                     AtPolicy       po,
                     AtIFT         *ift){
  //AtIFT            * ift;         // 00+08: ift structure (the result)
  AtPQueueU64      * q;           // 08+08: priority queue structure
  uint8_t          * r;           // 16+08: node status (processed or not?)
  double             newc;        // 24+08: new neighbor connectivity (if better)
  double             vmax;        // 32+08: max array value (for allocating the queue)
  uint64_t           s;           // 40+08: index of current node in array
  uint64_t           t;           // 48+08: index of current neighbor in array
  uint64_t           off;         // 56+08: index of current node in grapharray (for loop)
  uint64_t           offn;        // 64+08: index of next node in grapharray (for loop)
  uint64_t           i;           // 72+08: counter
  uint16_t           pr;          // 80+02: priority
  uint8_t            pd[5];       // 82+06: padding for alignment
                                  // Total: 88 bytes
                           
  // Create the auxiliary structures
  r       = calloc(ar->h.nelem,sizeof(uint8_t));
  if(!ift)
    ift = at_ift_new_initu8(ar); // allocate maps (label, root, predec., and connectivity)
  at_ift_initu8(ar, ift);
  off     = g->h->nelem * g->adjacency;
  vmax    = -INFINITY;
  for(i   = 0; i < off; i++)       // get maximum value (for allocating queue)
    vmax  = max(vmax,g->weights[i]);

  // Initialize IFT structure values and add seeds
  c.init (ift,ar);   // trivial connectivity values (excluding seeds)
  c.seeds(ift,seeds);// trivial connectivity values (only seeds)
  at_ift_add_seeds_labels(ift, seeds);

  // Create priority queue and add seeds
  q = at_pqueueu64_new_prealloc(c.o,po,(uint64_t)vmax+2,ar->h.nelem);
  at_ift_add_seeds_to_pqueue(ift,q,seeds);

  // Main loop
  while(!at_pqueueu64_is_empty(q)){
    // get best node
    s    = at_pqueueu64_remove(q);
    r[s] = 1;
    // loop through neighbors
    off  = s   * g->adjacency;
    offn = off + g->adjacency;
    for(i = off; i < offn; i++){
      if(g->active[i]){
        t = g->neighbors[i];
        // Still not processed?
        if(!r[t]){
          newc = c.func(ift, g, s, t, i);
          if((c.o == AT_MAXIMIZATION && ift->c[t] < newc)||
             (c.o == AT_MINIMIZATION && ift->c[t] > newc)){
            if(at_pqueueu64_has(q,t))
              at_pqueueu64_remove_from(q,t);
            ift->c[t] = newc;
            ift->p[t] = s;
            ift->r[t] = ift->r[s];
            ift->l[t] = ift->l[s];
            pr = (uint16_t)ift->c[t];
            if(c.o == AT_MINIMIZATION) pr++;
            at_pqueueu64_add(q, pr, t);
          }
        }
      }
    }
  }

  // Clear memory
  at_pqueueu64_destroy(&q);
  free(r);
  return ift;
}

AtIFT*
at_arrayu8_dift(AtIFT* ift, AtSeeds* seeds, ){

}

AtSCC*
at_orfc_core_arrayu8(AtArrayU8     * array,
                     AtGraphArray  * g,
                     AtConnectivity  conn,
                     AtSeeds       * seeds,
                     uint64_t        lblback,
                     AtPolicy        po,
                     AtSCCAlgorithm  sccalgo){
  uint64_t off, i, k, a, b, size;
  AtSeeds* sback, *sobj;
  // get only background seeds by using lblback
  at_seeds_split(seeds, &sback, &sobj, lblback);

  // apply ift with background seeds
  AtIFT* ift = NULL;
  ift = at_ift_apply_arrayu8(array,g,conn,sback,po, ift);

  // regenerate graph
  at_grapharrayu8_renew_edges(g);

  size = g->h->nelem * g->adjacency;
  for(i = 0, a = 0; i < size; i += g->adjacency, a++){
    for(k = 0; k < g->adjacency; k++){
      off = i+k;
      if(g->active[off]){
        b = g->neighbors[off];
        if(ift->c[a] != ift->c[b] || (conn.o == AT_MAXIMIZATION && g->weights[off] <= ift->c[a] ||
                                      conn.o == AT_MINIMIZATION && g->weights[off] >= ift->c[a]))
          g->active[off] = 0;
      }
    }
  }

  // Find SCC
  AtSCC* scc = at_grapharrayu8_scc(g,sccalgo);

  // Remove background SCCs
  uint64_t *s = malloc(array->h.nelem << 3);
  memset(s,0,array->h.nelem << 3);
  //   Create LUT: s[label] = seed (for O(1) lookup)
  for(i = 0, k = 1; i < seeds->n; i++)
    if(s[scc->l[seeds->s[i]]] == 0 && seeds->l[i] != lblback)
      s[scc->l[seeds->s[i]]] = k++;
  scc->n = k;

  //   Use above array to LUT the labels
  AtArrayU32* scclbl = at_arrayu32_new_with_data(array->h.dim, array->h.shape, scc->l, false);
  scclbl->h.owns_data = false;
  AtArrayU32* scclbl2= at_arrayu32_lut(scclbl,s);
  memcpy(scc->l, scclbl2->data, scclbl2->h.nelem << 2);

  // Convert to bitmap
//  uint32_t lbl = scc->l[sobj->data[0]];
//  for(i = 0; i < array->h.nelem; i++){
//    if(scc->l[i] == lbl)      scc->l[i] = 255;
//    else                      scc->l[i] = 0;
//  }
//  scc->n = 2;

  free(s);
  at_seeds_destroy(&sback);
  at_seeds_destroy(&sobj);

  at_arrayu32_destroy(&scclbl);
  at_arrayu32_destroy(&scclbl2);
  free(ift);
  return scc;
}

/**
 * @brief Split vector in two arrays: lower then pivot and higher than pivot
 * @param a the vector
 * @param l the pivot (left end of range)
 * @param r the right end of range
 * @param c comparison values
 * @return
 */
static int64_t
partition( uint64_t a[], int64_t l, int64_t r, double* c) {
   int64_t i, j, t;
   uint64_t pivot;
   pivot = a[l];
   i = l; j = r+1;

   while( 1){
     do ++i; while( c[a[i]] <= c[pivot] && i <= r );
     do --j; while( c[a[j]] > c[pivot] );
     if( i >= j ) break;
     t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
/**
 * @brief quickSort
 * @param a the vector
 * @param l index of left range
 * @param r index of right range
 */
static void
quickSort( uint64_t a[], int64_t l, int64_t r, double* c)
{
  int64_t j;

  if( l < r ){
    // divide and conquer
    j = partition( a, l, r, c);
    quickSort( a, l, j-1, c);
    quickSort( a, j+1, r, c);
  }
}

static void
at_orfc_arrayu8_dfs(uint64_t seed, AtGraphArray* g, uint64_t* stack, uint64_t stackidx, AtIFT* ift, uint8_t* instack){
  uint64_t s, t, soff, soffn, i;
  if(ift->l[seed] == 0){
    stack[stackidx++] = seed;
    while(stackidx != 0){
      s = stack[--stackidx];
      instack[s] = 0;
      if(!ift->l[s]){
        ift->l[s] = 1;
        soff = s*g->adjacency;
        soffn= soff + g->adjacency;
        for(i = soff;i < soffn;i++){
          t = g->neighbors[i];
          if(g->active[i] && g->weights[i] > ift->c[seed] && !ift->l[t] && !instack[t]){
            stack[stackidx++] = t;
            instack[s] = 1;
          }
        }
      }
    }
  }
}


AtIFT*
at_orfc_arrayu8(AtArrayU8     * array,
                AtGraphArray  * g,
                AtConnectivity  conn,
                AtSeeds       * seeds,
                uint64_t        lblback,
                AtPolicy        po){
  uint64_t off, i,j, k, a, b, size;
  AtSeeds* sback, *sobj;
  // get only background seeds by using lblback
  at_seeds_split(seeds, &sback, &sobj, lblback);

  // find energy by applying ift with background seeds
  AtIFT* ift = NULL;
  ift = at_ift_apply_arrayu8(array,g,conn,sback,po, ift);
  memset(ift->l,0,array->h.nelem);

  // regenerate graph
  at_grapharrayu8_renew_edges(g);

  // create queue
  uint64_t *q = malloc(sizeof(uint64_t)*sobj->n);
  memcpy(q,sobj->s,sizeof(uint64_t)*sobj->n);

  // sort queue based on ift->c (Copt)
  quickSort(q,0,sobj->n-1,ift->c);

  // apply DFS
  uint64_t *s  = malloc(array->h.nelem << 3);
  uint8_t*  instack = malloc(array->h.nelem);
  memset(instack,0,array->h.nelem);
  uint64_t  si = 0;

  for(i = 0; i < sobj->n; i++)
    at_orfc_arrayu8_dfs(q[i],g,s,si,ift,instack);
  free(instack);
  free(s);
  free(q);
  at_seeds_destroy(&sback);
  at_seeds_destroy(&sobj);


//  // remove
//  size = g->h->nelem * g->adjacency;
//  for(i = 0, a = 0; i < size; i += g->adjacency, a++){
//    for(k = 0; k < g->adjacency; k++){
//      off = i+k;
//      if(g->active[off]){
//        b = g->neighbors[off];
//        if(conn.o == AT_MAXIMIZATION && g->weights[off] <= ift->c[sobj->s[0]] ||
//           conn.o == AT_MINIMIZATION && g->weights[off] >= ift->c[sobj->s[0]])
//          g->active[off] = 0;
//      }
//    }
//  }

//  // find Directed Rooted Tree (DRT)
//  uint64_t* tree = malloc(array->h.nelem << 3);
//  memset(ift->l,lblback,array->h.nelem);
//  uint64_t  top  = 1, offa;
//  uint8_t lblobj = sobj->l[0];

//  for(i = 0; i < sobj->n; i++){
//    tree[top++] = sobj->s[i];
//    ift->l[sobj->s[i]] = lblobj;
//  }
//  for(i = 0; i < top; i++){
//    a   = tree[i];
//    offa = g->adjacency * a;
//    for(k = 0; k < g->adjacency; k++){
//      b = g->neighbors[offa+k];
//      if(ift->l[b] != lblobj && g->active[offa+k]){
//        ift->l[b] = lblobj;
//        tree[top++] = b;
//      }
//    }
//  }
//  free(tree);
//  at_grapharray_destroy(&g);
  return ift;
}
