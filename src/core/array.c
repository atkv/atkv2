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
#include <at/core.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/

static uint64_t*
at_arrayheader_get_child_step(AtArrayHeader* h){
  uint64_t* cstep, i;
  cstep = malloc(h->dim << 3);
  cstep[h->dim-1] = 1;
  for(i = h->dim-2; i < h->dim; i--)
    cstep[i] = cstep[i+1]*h->shape[i+1];
  return cstep;
}
static uint64_t
at_arrayheader_1d_to_p1d(AtArrayHeader*h, uint64_t idx){
  uint64_t p1d = 0;
  uint8_t  k,  f = h->cstep == NULL;
  if(f) h->cstep = at_arrayheader_get_child_step(h);
  for(k = 0; k < h->dim; k++){
    p1d += (idx/h->cstep[k])*h->step[k];
    idx %= h->cstep[k];
  }
  if(f) {free(h->cstep);h->cstep = NULL;}
  return p1d;
}

static uint64_t
at_arrayheader_nd_to_p1d(AtArrayHeader* h, uint64_t* idx){
  uint64_t p1d = 0;
  uint8_t k;
  for(k = 0; k < h->dim; k++)
    p1d += idx[k]*h->step[k];
  return p1d;
}

#define at_array_1d_to_p1d(ar,idx) at_arrayheader_1d_to_p1d(&ar->h,idx)
#define at_array_nd_to_p1d(ar,idx) at_arrayheader_nd_to_p1d(&ar->h,idx)

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
  header->nelem = header->step[0] * header->shape[0];
}

#define AT_DEFINE_ARRAY_OP(op)             \
  op(u8 ,U8 )                               \
  op(u16,U16)                               \
  op(u32,U32)                               \
  op(u64,U64)                               \
  op(i8 ,I8 )                               \
  op(i16,I16)                               \
  op(i32,I32)                               \
  op(i64,I64)                               \
  op(f32,F32)                               \
  op(d64,D64)

#define AT_DEFINE_ARRAY_OP3(op)             \
  op(u8 ,U8 ,uint8_t )                      \
  op(u16,U16,uint16_t)                      \
  op(u32,U32,uint32_t)                      \
  op(u64,U64,uint64_t)                      \
  op(i8 ,I8 , int8_t )                      \
  op(i16,I16, int16_t)                      \
  op(i32,I32, int32_t)                      \
  op(i64,I64, int64_t)                      \
  op(f32,F32,float)                         \
  op(d64,D64,double)

#define AT_ARRAY_CREATE(lower,UPPER, type)                  \
AtArray##UPPER*                                             \
at_array##lower##_create(){                                 \
  AtArray##UPPER* array = malloc(sizeof(AtArray##UPPER));   \
  at_array_header_init(&array->h);                          \
  array->data = NULL;                                       \
  array->h.elemsize = sizeof(type);                         \
  return array;                                             \
}
AT_DEFINE_ARRAY_OP3(AT_ARRAY_CREATE)
#undef AT_ARRAY_CREATE

#define AT_ARRAY_NEW(lower,UPPER, type)                           \
AtArray##UPPER*                                                   \
at_array##lower##_new(uint8_t dim, uint64_t* shape){              \
  AtArray##UPPER* array = at_array##lower##_create();             \
  at_array_header_set(&array->h, dim, shape);                     \
  array->data = malloc(array->h.nelem * array->h.elemsize);\
  return array;                                                   \
}
AT_DEFINE_ARRAY_OP3(AT_ARRAY_NEW)
#undef AT_ARRAY_NEW

#define AT_ARRAY_NEW_WITH_DATA(lower, UPPER, type)                                    \
AtArray##UPPER*                                                                       \
at_array##lower##_new_with_data(uint8_t dim, uint64_t* shape, type* data, bool copy){ \
  AtArray##UPPER* array = at_array##lower##_create();                                 \
  uint64_t num_bytes;                                                                 \
  at_array_header_set(&array->h, dim, shape);                                         \
                                                                                      \
  if(!copy){                                                                          \
    array->data = data;                                                               \
    array->h.owns_data = false;                                                       \
  }else{                                                                              \
    num_bytes   = array->h.nelem * array->h.elemsize;                          \
    array->data = malloc(num_bytes);                                                  \
    memcpy(array->data, data, num_bytes);                                             \
  }                                                                                   \
  return array;                                                                       \
}
AT_DEFINE_ARRAY_OP3(AT_ARRAY_NEW_WITH_DATA)
#undef AT_ARRAY_NEW_WITH_DATA

#define AT_ARRAY_DESTROY(lower, UPPER)                                          \
void                                                                                  \
at_array##lower##_destroy(AtArray##UPPER** array_ptr){                                            \
  if(array_ptr){                                                                      \
    AtArray##UPPER* array = *array_ptr;                                                    \
    if(array){                                                                        \
      if(array->h.owns_data)                                                          \
        free(array->data);                                                            \
      at_array_header_dispose(&array->h);                                             \
      free(array);                                                                    \
    }                                                                                 \
    *array_ptr = NULL;                                                                \
  }                                                                                   \
}
AT_DEFINE_ARRAY_OP(AT_ARRAY_DESTROY)
#undef AT_ARRAY_NEW_WITH_DATA

#define AT_ARRAY_ZEROS(lower, UPPER)                                                  \
AtArray##UPPER*                                                                       \
at_array##lower##_zeros(uint8_t dim, uint64_t* shape){                               \
  AtArray##UPPER* array = at_array##lower##_new(dim, shape);                          \
  at_array##lower##_fill(array,0);                                                    \
  return array;                                                                       \
}
AT_DEFINE_ARRAY_OP(AT_ARRAY_ZEROS)
#undef AT_ARRAY_ZEROS


#define AT_ARRAY_ONES(lower, UPPER)                                                   \
AtArray##UPPER*                                                                       \
at_array##lower##_ones(uint8_t dim, uint64_t* shape){                                \
  AtArray##UPPER* array = at_array##lower##_new(dim, shape);                          \
  at_array##lower##_fill(array,1);                                                    \
  return array;                                                                       \
}
AT_DEFINE_ARRAY_OP(AT_ARRAY_ONES)
#undef AT_ARRAY_ONES

#define AT_ARRAY_FILL(lower, UPPER, type)                                             \
void                                                                                  \
at_array##lower##_fill(AtArray##UPPER* array, type value){                            \
  uint64_t i;                                                                         \
  if(value == 0) memset(array->data, 0, array->h.nelem * array->h.elemsize);          \
  else                                                                                \
    for(i = 0; i < array->h.nelem; i++)                                               \
      array->data[i] = value;                                                         \
}
AT_DEFINE_ARRAY_OP3(AT_ARRAY_FILL)
#undef AT_ARRAY_FILL


void
at_arrayu8_add_scalar(AtArrayU8* array, uint8_t value){
  uint64_t i;
  for(i = 0; i < array->h.nelem; i++)
    array->data[i] += value;
}
void
at_arrayu8_add_scalar_clamped(AtArrayU8* array, uint8_t value){
  uint64_t i;
  uint8_t  c;
  for(i = 0; i < array->h.nelem; i++){
    c = array->data[i] + value;
    if(c < array->data[i]) c = UINT8_MAX;
    array->data[i] = c;
  }
}

uint8_t
at_arrayu8_max(AtArrayU8* array){
  uint64_t i;
  uint8_t  value = 0;
  for(i = 0; i < array->h.nelem; i++)
    value = max(value, array->data[i]);
  return value;
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
    // write nelem, dim, owns_data and elemsize
    at_znzfile_write(fp,&arrays[i]->h.nelem, sizeof(uint64_t)+(sizeof(uint8_t)<<1)+sizeof(uint8_t),1);

    // write shape and step
    at_znzfile_write(fp,arrays[i]->h.shape, sizeof(uint64_t),arrays[i]->h.dim);
    at_znzfile_write(fp,arrays[i]->h.step, sizeof(uint64_t),arrays[i]->h.dim);

    // write data
    at_znzfile_write(fp,arrays[i]->data, arrays[i]->h.elemsize,arrays[i]->h.nelem);
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
    // read nelem, dim, owns_data and elemsize
    at_znzfile_read(fp,&ar[i].h.nelem,sizeof(uint64_t)+(sizeof(uint8_t)<<1)+sizeof(uint8_t),1);

    // read shape and step
    ar[i].h.shape = malloc(sizeof(uint64_t)*ar[i].h.dim);
    ar[i].h.step  = malloc(sizeof(uint64_t)*ar[i].h.dim);
    at_znzfile_read(fp,ar[i].h.shape,sizeof(uint64_t),ar[i].h.dim);
    at_znzfile_read(fp,ar[i].h.step ,sizeof(uint64_t),ar[i].h.dim);

    // read data
    ar[i].data = malloc(ar[i].h.elemsize*ar[i].h.nelem);
    at_znzfile_read(fp,ar[i].data,ar[i].h.elemsize,ar[i].h.nelem);
  }

  // close the file
  at_znzfile_close(fp);
  return ar;
}

//void
//at_arrayu8_set_nd(AtArrayU8* ar, uint64_t *coords, uint8_t value){
//  uint64_t coord = 0;
//  at_array_index_to_1d(ar,coords,&coord);
//  ar->data[coord] = value;
//}

void
at_arrayu8_set_nd_many(AtArrayU8* ar, uint64_t *coords, uint8_t* value){
  uint64_t coord = 0;
  uint8_t lim = ar->h.dim-1;
  uint8_t i;
  for(i = 0; i < lim; i++) coord += coords[i] * ar->h.step[i];
  memcpy(&ar->data[coord],value,sizeof(uint8_t)*ar->h.shape[lim]);
}

void
at_array_header_dispose(AtArrayHeader* header){
  if(header->shape) free(header->shape);
  if(header->cstep) free(header->cstep);
}
void
at_arrayheader_sub(AtArrayHeader* h, AtArrayHeader* parent_h, AtRange* ranges){
  AtRange range;
  uint8_t i;

  h->dim          = parent_h->dim;
  h->elemsize     = parent_h->elemsize;
  h->shape        = realloc(h->shape,h->dim << 4);
  h->step         = &h->shape[h->dim];
  h->cstep        = NULL;
  h->owns_data    = parent_h->owns_data;
  h->nelem = 1;

  for(i = parent_h->dim-1; i < parent_h->dim; i--){
    range = ranges[i];
    h->shape[i] = min(range.to-range.from,parent_h->shape[i]);
    if(h->shape[i] == 0) h->shape[i] == 1;
    h->nelem *= h->shape[i];
  }
  memcpy(h->step,parent_h->step,parent_h->dim<<3);
}

#define COPY(ar, output, i, k)                                   \
  uint64_t *cnd   = malloc(output->h.dim << 3);                  \
  uint64_t  p1d;                                                 \
  output->h.step[output->h.dim-1] = 1;                           \
  for(i = output->h.dim-2; i < output->h.dim; i--)               \
    output->h.step[i] = output->h.step[i+1]*output->h.shape[i+1];\
  output->data = realloc(output->data,output->h.nelem);   \
  output->h.owns_data = true;                                    \
  for(i = 0; i < output->h.nelem; i++){                   \
    at_index_to_nd(output->h.dim,output->h.step,i,cnd);          \
    p1d = 0;                                                     \
    for(k = 0; k < output->h.dim; k++)                           \
      p1d += (cnd[k] + ranges[k].from)*ar->h.step[k];            \
    output->data[i] = ar->data[p1d];                             \
  }                                                              \
  free(cnd);

void
at_arrayu16_sub_u8(AtArrayU16* ar, AtRange* ranges, AtArrayU8** outputp){
  AtArrayU8* output = *outputp;
  uint64_t i, k;
  if(output == NULL) output = at_arrayu8_create();
  at_arrayheader_sub(&output->h,&ar->h,ranges);
  output->h.elemsize     = 1;
  *outputp = output;
  COPY(ar,output,i,k)
}

void
at_arrayu16_sub(AtArrayU16* ar, AtRange* ranges, AtArrayU16** outputp, uint8_t copy){
  AtArrayU16* output = *outputp;
  uint64_t i, k;
  if(output == NULL) output = at_arrayu16_create();
  at_arrayheader_sub(&output->h,&ar->h,ranges);
  *outputp = output;
  // fill another (new) array or just offset it
  if(copy){
    COPY(ar,output,i,k)
  }else{
    output->h.cstep = at_arrayheader_get_child_step(&output->h);
    output->data = ar->data;
    for(i = 0; i < output->h.dim; i++)
      output->data += ranges[i].from * output->h.step[i];
  }
}

#define AT_DEFINE_ARRAY_SET_1D(lower, UPPER, type) void          \
at_array##lower##_set_1d(AtArray##UPPER* ar, uint64_t idx, type value){\
  ar->data[at_array_1d_to_p1d(ar,idx)] = value;                  \
}
#define AT_DEFINE_ARRAY_SET_ND(lower, UPPER, type) void          \
at_array##lower##_set_nd(AtArray##UPPER* ar, uint64_t* idx, type value){\
  ar->data[at_array_nd_to_p1d(ar,idx)] = value;                  \
}
#define AT_DEFINE_ARRAY_GET_1D(lower, UPPER, type) type          \
at_array##lower##_get_1d(AtArray##UPPER* ar, uint64_t idx){      \
  if(ar->h.cstep)                                                \
    return ar->data[at_array_1d_to_p1d(ar,idx)];                 \
  else                                                           \
    return ar->data[idx];                                        \
}
#define AT_DEFINE_ARRAY_GET_ND(lower, UPPER, type) type          \
at_array##lower##_get_nd(AtArray##UPPER* ar, uint64_t* idx){     \
  uint64_t i;                                                    \
  if(ar->h.cstep)                                                \
    return ar->data[at_array_nd_to_p1d(ar,idx)];                 \
  else{                                                          \
    at_index_to_1d(ar->h.dim, ar->h.step, idx, &i);              \
    return ar->data[i];                                          \
  }                                                              \
}

AT_DEFINE_ARRAY_OP3(AT_DEFINE_ARRAY_SET_1D)
AT_DEFINE_ARRAY_OP3(AT_DEFINE_ARRAY_SET_ND)
AT_DEFINE_ARRAY_OP3(AT_DEFINE_ARRAY_GET_1D)
AT_DEFINE_ARRAY_OP3(AT_DEFINE_ARRAY_GET_ND)

#undef AT_DEFINE_ARRAY_SET_1D
#undef AT_DEFINE_ARRAY_SET_ND
#undef AT_DEFINE_ARRAY_GET_1D
#undef AT_DEFINE_ARRAY_GET_ND

void
at_arrayheader_squeeze(AtArrayHeader* h){
  uint64_t* step = h->step;
  uint8_t cpos = 0,i;
  for(i = cpos; i < h->dim; i++){
    if(h->shape[i] != 1) {
      h->shape[cpos] = h->shape[i];
      step[cpos]= step[i];
      if(h->cstep) h->cstep[cpos]= h->cstep[i];
      cpos++;
    }
  }
  h->dim = cpos;
  h->step = &h->shape[h->dim];
  for(i = 0; i < h->dim; i++)
    h->step[i] = step[i];
  h->shape = realloc(h->shape, cpos << 4);
  if(h->cstep) h->cstep = realloc(h->cstep, cpos << 3);
}
