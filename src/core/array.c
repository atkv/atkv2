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
#include <at/core/array.h>
#include <at/core/macro.h>
#include <at/core/znzfile.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PUBLIC API
 ============================================================================*/

void
at_array_header_init(AtArrayHeader* header){
  memset(header,0,sizeof(AtArrayHeader));
  header->owns_data = 1;
}

void
at_array_header_set(AtArrayHeader* header, uint8_t dim, uint64_t* shape){
  size_t   num_bytes = dim*sizeof(uint64_t);
  uint64_t tmp = 1;
  uint8_t  i;
  header->dim      = dim;
  header->shape    = (uint64_t*) realloc(header->shape,num_bytes << 1);
  header->step     = &header->shape[dim];
  memcpy(header->shape, shape, dim*sizeof(uint64_t));
  for(i = dim-1; i < dim; i--){
    header->step[i] = tmp;
    tmp *= header->shape[i];
  }
  header->num_elements = header->step[0] * header->shape[0];
}

AtArrayU8*
at_arrayu8_create(){
  AtArrayU8* array = malloc(sizeof(AtArrayU8));
  at_array_header_init(&array->h);
  array->data = NULL;
  array->h.elemsize = sizeof(uint8_t);
  return array;
}
AtArrayU32*
at_arrayu32_create(){
  AtArrayU32* array = malloc(sizeof(AtArrayU32));
  at_array_header_init(&array->h);
  array->data = NULL;
  array->h.elemsize = sizeof(uint32_t);
  return array;
}
AtArrayU64*
at_arrayu64_create(){
  AtArrayU64* array = malloc(sizeof(AtArrayU64));
  at_array_header_init(&array->h);
  array->data = NULL;
  array->h.elemsize = sizeof(uint64_t);
  return array;
}

AtArrayU8*
at_arrayu8_new(uint8_t dim, uint64_t* shape){
  AtArrayU8* array = at_arrayu8_create();
  at_array_header_set(&array->h, dim, shape);
  array->data = malloc(array->h.num_elements * sizeof(uint8_t));
  return array;
}

AtArrayU8*
at_arrayu8_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data, bool copy){
  AtArrayU8* array = at_arrayu8_create();
  uint64_t num_bytes;
  at_array_header_set(&array->h, dim, shape);

  if(!copy){
    array->data = data;
    array->h.owns_data = false;
  }
  else{
    num_bytes   = array->h.num_elements * sizeof(uint8_t);
    array->data = malloc(num_bytes);
    memcpy(array->data, data, num_bytes);
  }
  return array;
}

AtArrayU64*
at_arrayu64_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data, bool copy){
  AtArrayU64* array = at_arrayu64_create();
  uint64_t num_bytes;
  at_array_header_set(&array->h, dim, shape);

  if(!copy)
    array->data = data;
  else{
    num_bytes   = array->h.num_elements * sizeof(uint64_t);
    array->data = malloc(num_bytes);
    memcpy(array->data, data, num_bytes);
  }
  return array;
}
AtArrayU32*
at_arrayu32_new_with_data(uint8_t dim, uint64_t* shape, uint32_t* data, bool copy){
  AtArrayU32* array = at_arrayu32_create();
  uint64_t num_bytes;
  at_array_header_set(&array->h, dim, shape);

  if(!copy)
    array->data = data;
  else{
    num_bytes   = array->h.num_elements * array->h.elemsize;
    array->data = malloc(num_bytes);
    memcpy(array->data, data, num_bytes);
  }
  return array;
}

void
at_arrayu8_fill(AtArrayU8* array, uint8_t value){
  memset(array->data,value,array->h.num_elements * sizeof(uint8_t));
}

uint8_t
at_arrayu8_max(AtArrayU8* array){
  uint64_t i;
  uint8_t  value = 0;
  for(i = 0; i < array->h.num_elements; i++)
    value = max(value, array->data[i]);
  return value;
}

void
at_arrayu64_fill(AtArrayU64* array, uint64_t value){
  uint64_t i;
  if(value == 0) memset(array->data, 0, array->h.num_elements * sizeof(uint64_t));
  else
    for(i = 0; i < array->h.num_elements; i++)
      array->data[i] = value;
}

AtArrayU8*
at_arrayu8_zeros(uint8_t dim, uint64_t *shape){
  AtArrayU8* array = at_arrayu8_new(dim, shape);
  at_arrayu8_fill(array,0);
  return array;
}
AtArrayU8*
at_arrayu8_ones(uint8_t dim, uint64_t *shape){
  AtArrayU8* array = at_arrayu8_new(dim, shape);
  at_arrayu8_fill(array,1);
  return array;
}


void
at_arrayu8_destroy(AtArrayU8** array_ptr){
  if(array_ptr){
    AtArrayU8* array = *array_ptr;
    if(array){
      if(array->h.owns_data)
        free(array->data);
      at_array_header_dispose(&array->h);
      free(array);
    }
    *array_ptr = NULL;
  }
}

void
at_index_to_nd(uint8_t dim, uint64_t* step, uint64_t s, uint64_t* s_nd){
  uint8_t i;
  for(i = 0; i < dim; i++){
    s_nd[i]  = s/step[i];
    s       %= step[i];
  }
}

void
at_index_to_1d(uint8_t dim, uint64_t* step, int64_t* s_nd, uint64_t* s){
  uint8_t i;
  *s = 0;
  for(i = 0; i < dim; i++)
    *s += s_nd[i] * step[i];
}

#define at_array_index_to_nd(array, s, s_nd) at_index_to_nd(array->h.dim, array->h.step,s,s_nd)
#define at_array_index_to_1d(array, s_nd, s) at_index_to_1d(array->h.dim, array->h.step,s_nd,s)

AtArrayU16*
at_arrayu16_create(){
  AtArrayU16* array = malloc(sizeof(AtArrayU16));
  at_array_header_init(&array->h);
  array->h.elemsize = sizeof(uint16_t);
  array->data = NULL;
  return array;
}
AtArrayU16*
at_arrayu16_new(uint8_t dim, uint64_t* shape){
  AtArrayU16* array = at_arrayu16_create();
  at_array_header_set(&array->h, dim, shape);
  array->data = malloc(array->h.num_elements * sizeof(uint16_t));
  return array;
}

void
at_arrayu8_save(AtArrayU8** arrays, char** names, uint8_t num, const char* filename){
  AtZnzFile* fp = at_znzfile_open(filename, "wb", true);
  uint8_t i,len;

  // write number of arrays
  at_znzfile_write(fp,&num,sizeof(uint8_t),1);

  // write names
  for(i = 0; i < num; i++){
    len = strlen(names[i]);
    at_znzfile_write(fp,&len,sizeof(uint8_t),1);
    at_znzfile_write(fp,names[i],sizeof(char),len);
  }

  // write proper arrays
  for(i = 0; i < num; i++){
    // write num_elements, dim, owns_data and elemsize
    at_znzfile_write(fp,&arrays[i]->h.num_elements, sizeof(uint64_t)+(sizeof(uint8_t)<<1)+sizeof(uint8_t),1);

    // write shape and step
    at_znzfile_write(fp,arrays[i]->h.shape, sizeof(uint64_t),arrays[i]->h.dim);
    at_znzfile_write(fp,arrays[i]->h.step, sizeof(uint64_t),arrays[i]->h.dim);

    // write data
    at_znzfile_write(fp,arrays[i]->data, arrays[i]->h.elemsize,arrays[i]->h.num_elements);
  }

  // close the file
  at_znzfile_close(fp);
}

AtArrayU8*
at_array_load(char*** namesp, uint8_t *nump, const char* filename){
  char            ** names;
  AtArrayU8  * ar;
  AtZnzFile        * fp;
  uint8_t            i;
  uint8_t            len;
  uint8_t            num;

  // open the file
  fp = at_znzfile_open(filename, "rb", true);

  // read number of arrays
  at_znzfile_read(fp,nump,sizeof(uint8_t),1);
  num = *nump;

  // read names
  *namesp =(char**) malloc(num*sizeof(char*));
  names = *namesp;
  for(i = 0; i < num; i++){
    at_znzfile_read(fp,&len,sizeof(uint8_t),1);
    names[i] = (char*)malloc(sizeof(char)*(len+1));
    at_znzfile_read(fp,names[i],sizeof(char),len);
    names[i][len] = '\0';
  }

  // read proper arrays
  ar = malloc(sizeof(AtArrayU8)*num);
  for(i = 0; i < num; i++){
    // read num_elements, dim, owns_data and elemsize
    at_znzfile_read(fp,&ar[i].h.num_elements,sizeof(uint64_t)+(sizeof(uint8_t)<<1)+sizeof(uint8_t),1);

    // read shape and step
    ar[i].h.shape = malloc(sizeof(uint64_t)*ar[i].h.dim);
    ar[i].h.step  = malloc(sizeof(uint64_t)*ar[i].h.dim);
    at_znzfile_read(fp,ar[i].h.shape,sizeof(uint64_t),ar[i].h.dim);
    at_znzfile_read(fp,ar[i].h.step ,sizeof(uint64_t),ar[i].h.dim);

    // read data
    ar[i].data = malloc(ar[i].h.elemsize*ar[i].h.num_elements);
    at_znzfile_read(fp,ar[i].data,ar[i].h.elemsize,ar[i].h.num_elements);
  }

  // close the file
  at_znzfile_close(fp);
  return ar;
}

void
at_arrayu8_set_nd(AtArrayU8* ar, uint64_t *coords, uint8_t value){
  uint64_t coord = 0;
  at_array_index_to_1d(ar,coords,&coord);
  ar->data[coord] = value;
}

void
at_arrayu8_set_nd_many(AtArrayU8* ar, uint64_t *coords, uint8_t* value){
  uint64_t coord = 0;
  uint8_t lim = ar->h.dim-1;
  uint8_t i;
  for(i = 0; i < lim; i++) coord += coords[i] * ar->h.step[i];
  memcpy(&ar->data[coord],value,sizeof(uint8_t)*ar->h.shape[lim]);
}

void
at_arrayu16_destroy(AtArrayU16** arp){
  if(arp){
    AtArrayU16* ar = *arp;
    if(ar){
      if(ar->h.owns_data)
        free(ar->data);
      at_array_header_dispose(&ar->h);
      free(ar);
    }
    *arp = NULL;
  }
}


void
at_arrayu64_destroy(AtArrayU64** array_ptr){
  if(array_ptr){
    AtArrayU64* array = *array_ptr;
    if(array){
      if(array->h.owns_data)
        free(array->data);
      at_array_header_dispose(&array->h);
      free(array);
    }
    *array_ptr = NULL;
  }
}

void
at_array_header_dispose(AtArrayHeader* header){
  free(header->shape);
}

AtArrayU32*
at_arrayu32_new(uint8_t dim, uint64_t* shape){
  AtArrayU32* array = at_arrayu32_create();
  at_array_header_set(&array->h, dim, shape);
  array->data = malloc(array->h.num_elements * array->h.elemsize);
  return array;
}
void
at_arrayu32_destroy(AtArrayU32** array_ptr){
  if(array_ptr){
    AtArrayU32* array = *array_ptr;
    if(array){
      if(array->h.owns_data)
        free(array->data);
      at_array_header_dispose(&array->h);
      free(array);
    }
    *array_ptr = NULL;
  }
}
