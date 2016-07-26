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

#include <at/core/grapharray.h>
#include <math.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PUBLIC API
 ============================================================================*/
double
at_weighting_diff_abs(AtArrayU8* array, uint64_t s, uint64_t t, void* params){
  return abs(array->data[s] - array->data[t]);
}
double
at_weighting_diff_absc(AtArrayU8* array, uint64_t s, uint64_t t, void* params){
  return UINT8_MAX-abs(array->data[s] - array->data[t]);
}
double
at_weighting_diff_absc_alpha(AtArrayU8* array, uint64_t s, uint64_t t, void* params){
  double  sv    = (double)array->data[s];
  double  st    = (double)array->data[t];
  double  alpha = *((double*) params);
  double  diff  = sv-st;
  if(diff >= 0) alpha = -alpha;
  return (UINT8_MAX-fabs(diff))*(1+alpha);
}

void
at_grapharray_init(AtGraphArray* grapharray){
  memset(grapharray,0,sizeof(AtGraphArray));
}

AtGraphArray*
at_grapharray_create(){
  AtGraphArray* grapharray = malloc(sizeof(AtGraphArray));
  at_grapharray_init(grapharray);
  return grapharray;
}

static int8_t neighboring_2D[16] = { 0,-1,  0, 1, -1, 0,  1, 0,
                                    -1,-1, -1, 1,  1,-1,  1, 1};
static int8_t neighboring_3D[78] = { 0, 0,-1,  0, 0, 1,  0,-1, 0,            // N-6
                                     0, 1, 0, -1, 0, 0,  1, 0, 0,

                                     0,-1,-1,  0,-1, 1,  0, 1,-1,  0, 1, 1,  // N-18
                                    -1, 0,-1, -1, 0, 1,  1, 0,-1,  1, 0, 1,
                                    -1,-1, 0, -1, 1, 0,  1,-1, 0,  1, 1, 0,

                                    -1,-1,-1, -1,-1, 1, -1, 1,-1, -1, 1, 1,  // N-26
                                     1,-1,-1,  1,-1, 1,  1, 1,-1,  1, 1, 1};

AtGraphArray*
at_grapharrayu8_new(AtArrayU8*  array,
                    AtAdjacency adjacency,
                    AtWeighting w){
  AtGraphArray* grapharray   = at_grapharray_create();
  uint64_t      num_elements = array->h.num_elements * adjacency;
  uint64_t    * s_nd         = malloc(array->h.dim * sizeof(uint64_t));
  int64_t     * t_nd         = malloc(array->h.dim * sizeof(int64_t));
  int8_t      * neighboring;
  uint64_t    s, t, i, ss;
  uint8_t     k;
  bool        out;

  grapharray->neighbors      = malloc(num_elements * sizeof(uint64_t));
  grapharray->active         = malloc(num_elements * sizeof(uint8_t));
  grapharray->weights        = malloc(num_elements * sizeof(double));
  grapharray->h              = &array->h;
  grapharray->adjacency      = adjacency;
  memset(grapharray->neighbors,0,num_elements*sizeof(uint64_t));
  memset(grapharray->weights  ,0,num_elements*sizeof(double));
  memset(grapharray->active   ,0,num_elements*sizeof(uint8_t));

  if(array->h.dim == 2) neighboring = neighboring_2D;
  else                  neighboring = neighboring_3D;

  // Fill the neighbors and weights by using the weighting function
  // for each pixel
  for(s = 0; s < array->h.num_elements; s++){
    at_array_index_to_nd(array,s, s_nd);
    ss = s*adjacency;

    // for each neighbor
    for(i = 0; i < adjacency; i++){
      out = false;
      for(k = 0; k < array->h.dim; k++){
        // Is the neighbor inside?
        t_nd[k] = s_nd[k] + neighboring[i*array->h.dim + k];
        if(t_nd[k] < 0 || t_nd[k] >= array->h.shape[k]){
          out = true;
          break;
        }
      }
      if(!out){
        at_array_index_to_1d(array,t_nd,&t);
        grapharray->active[ss+i]    = 1;
        grapharray->weights[ss+i]   = w.f(array, s, t, w.params);
        grapharray->neighbors[ss+i] = t;
      }
    }
  }
  free(s_nd);
  free(t_nd);

  return grapharray;
}

void
at_grapharrayu8_renew_edges(AtGraphArray* g){
  int64_t   * t_nd         = malloc(g->h->dim * sizeof(int64_t));
  uint64_t  * s_nd         = malloc(g->h->dim * sizeof(uint64_t));
  int8_t    * neighboring;
  uint64_t    s, i, ss;
  uint8_t     k;

  if(g->h->dim == 2) neighboring = neighboring_2D;
  else               neighboring = neighboring_3D;

  // Fill the neighbors and weights by using the weighting function
  // for each pixel
  for(s = 0; s < g->h->num_elements; s++){
    at_index_to_nd(g->h->dim,g->h->step,s,s_nd);
    ss = s*g->adjacency;

    // for each neighbor
    for(i = 0; i < g->adjacency; i++){
      g->active[ss+i] = true;
      for(k = 0; k < g->h->dim; k++){
        // Is the neighbor inside?
        t_nd[k] = s_nd[k] + neighboring[i*g->h->dim + k];
        if(t_nd[k] < 0 || t_nd[k] >= g->h->shape[k]){
          g->active[ss+i] = false;
          break;
        }
      }
    }
  }
  free(t_nd);
  free(s_nd);
}

void
at_grapharray_destroy(AtGraphArray** grapharray_ptr){
  if(grapharray_ptr){
    AtGraphArray* grapharray = *grapharray_ptr;
    free(grapharray->active);
    free(grapharray->neighbors);
    free(grapharray->weights);
    free(grapharray);
    *grapharray_ptr = NULL;
  }
}

void
at_grapharray_remove_arc(AtGraphArray* g, uint64_t s, uint64_t t){
  uint64_t off = s*g->adjacency;
  uint64_t i;
  for(i = 0; i < g->adjacency; i++){
    if(g->neighbors[off+i] == t){
      g->active[off+i] = 0; break;
    }
  }
}

void
at_grapharray_add_arc(AtGraphArray* g, uint64_t s, uint64_t t){
  uint64_t off = s*g->adjacency;
  uint64_t i;
  for(i = 0; i < g->adjacency; i++){
    if(g->neighbors[off+i] == t){
      g->active[off+i] = 1; break;
    }
  }
}

void
at_grapharray_remove_edge(AtGraphArray* grapharray, uint64_t s, uint64_t t){
  at_grapharray_remove_arc(grapharray, s, t);
  at_grapharray_remove_arc(grapharray, t, s);
}

void
at_grapharray_add_edge(AtGraphArray* grapharray, uint64_t s, uint64_t t){
  at_grapharray_add_arc(grapharray, s, t);
  at_grapharray_add_arc(grapharray, t, s);
}

double
at_grapharray_get(AtGraphArray* graph, uint64_t s, uint64_t t){
  uint64_t off = s * graph->adjacency;
  uint64_t offn = off+graph->adjacency;
  uint64_t i;
  for(i = off; i < offn; i++)
    if(graph->neighbors[i] == t)
      return graph->weights[i];
  return 5;
}

void
at_grapharray_remove_arcs(AtGraphArray* g, uint64_t* pairs, uint64_t n){
  uint64_t i;
  for(i = 0; i < n; i += 2)
    at_grapharray_remove_arc(g, pairs[i], pairs[i+1]);
}

uint64_t
at_grapharray_get_indexr(AtGraphArray* g, uint64_t s, uint64_t t){
  uint64_t off = t*g->adjacency, i;
  for(i = 0; i < g->adjacency; i++)
    if(g->active[off+i] && g->neighbors[off+i] == s) return off+i;
}

__attribute__((constructor))
static void
at_conn_constructor(){
  at_wdiffabs.f       = at_weighting_diff_abs;
  at_wdiffabsc.f      = at_weighting_diff_absc;
  at_wdiffabscalpha.f = at_weighting_diff_absc_alpha;
}
