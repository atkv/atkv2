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

AtGraphArray*
at_grapharray_new_from_array_uint8_t(AtArray_uint8_t* array, AtAdjacency adjacency){
  AtGraphArray* grapharray = at_grapharray_create();
  uint64_t num_elements = array->header.num_elements * adjacency;
  grapharray->neighbors = malloc(num_elements * sizeof(uint64_t));
  grapharray->active    = malloc(num_elements * sizeof(uint8_t));
  grapharray->weights   = malloc(num_elements * sizeof(double));
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
