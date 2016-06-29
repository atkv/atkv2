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

#include <at/grapharray.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PUBLIC API
 ============================================================================*/
double
at_weighting_diff_abs(AtArray_uint8_t* array, uint64_t s, uint64_t t){
  return abs(array->data[s] - array->data[t]);
}
double
at_weighting_diff_absc(AtArray_uint8_t* array, uint64_t s, uint64_t t){
  return UINT8_MAX-abs(array->data[s] - array->data[t]);
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
at_grapharray_uint8_t_new(AtArray_uint8_t* array,
                          AtAdjacency adjacency,
                          AtWeightingFunc_uint8_t weighting){
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
        grapharray->weights[ss+i]   = weighting(array, s, t);
        grapharray->neighbors[ss+i] = t;
      }
    }
  }
  free(s_nd);
  free(t_nd);

  return grapharray;
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
at_grapharray_remove_arc(AtGraphArray* grapharray, uint64_t s, uint64_t t){
  grapharray->active[s*grapharray->adjacency + t] = 0;
}

void
at_grapharray_add_arc(AtGraphArray* grapharray, uint64_t s, uint64_t t){
  grapharray->active[s*grapharray->adjacency + t] = 1;
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
