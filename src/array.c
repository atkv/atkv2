#include <at/array.h>

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

AtArray_uint8_t*
at_array_uint8_t_create(){
  AtArray_uint8_t* array = malloc(sizeof(AtArray_uint8_t));
  at_array_header_init(&array->header);
  array->data = NULL;
  return array;
}
AtArray_uint64_t*
at_array_uint64_t_create(){
  AtArray_uint64_t* array = malloc(sizeof(AtArray_uint64_t));
  at_array_header_init(&array->header);
  array->data = NULL;
  return array;
}

AtArray_uint8_t*
at_array_uint8_t_new(uint8_t dim, uint64_t* shape){
  AtArray_uint8_t* array = at_array_uint8_t_create();
  at_array_header_set(&array->header, dim, shape);
  array->data = malloc(array->header.num_elements * sizeof(uint8_t));
  return array;
}

AtArray_uint8_t*
at_array_uint8_t_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data, bool copy){
  AtArray_uint8_t* array = at_array_uint8_t_create();
  uint64_t num_bytes;
  at_array_header_set(&array->header, dim, shape);

  if(!copy)
    array->data = data;
  else{
    num_bytes   = array->header.num_elements * sizeof(uint8_t);
    array->data = malloc(num_bytes);
    memcpy(array->data, data, num_bytes);
  }
  return array;
}

AtArray_uint64_t*
at_array_uint64_t_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data, bool copy){
  AtArray_uint64_t* array = at_array_uint64_t_create();
  uint64_t num_bytes;
  at_array_header_set(&array->header, dim, shape);

  if(!copy)
    array->data = data;
  else{
    num_bytes   = array->header.num_elements * sizeof(uint64_t);
    array->data = malloc(num_bytes);
    memcpy(array->data, data, num_bytes);
  }
  return array;
}

void
at_array_uint8_t_fill(AtArray_uint8_t* array, uint8_t value){
  memset(array->data,value,array->header.num_elements * sizeof(uint8_t));
}

void
at_array_uint64_t_fill(AtArray_uint64_t* array, uint64_t value){
  uint64_t i;
  if(value == 0) memset(array->data, 0, array->header.num_elements * sizeof(uint64_t));
  else
    for(i = 0; i < array->header.num_elements; i++)
      array->data[i] = value;
}

AtArray_uint8_t*
at_array_uint8_t_zeros(uint8_t dim, uint64_t *shape){
  AtArray_uint8_t* array = at_array_uint8_t_new(dim, shape);
  at_array_uint8_t_fill(array,0);
  return array;
}
AtArray_uint8_t*
at_array_uint8_t_ones(uint8_t dim, uint64_t *shape){
  AtArray_uint8_t* array = at_array_uint8_t_new(dim, shape);
  at_array_uint8_t_fill(array,1);
  return array;
}


void
at_array_uint8_t_destroy(AtArray_uint8_t** array_ptr){
  if(array_ptr){
    AtArray_uint8_t* array = *array_ptr;
    if(array){
      if(array->header.owns_data)
        free(array->data);
      at_array_header_dispose(&array->header);
      free(array);
    }
    *array_ptr = NULL;
  }
}
void
at_array_uint64_t_destroy(AtArray_uint64_t** array_ptr){
  if(array_ptr){
    AtArray_uint64_t* array = *array_ptr;
    if(array){
      if(array->header.owns_data)
        free(array->data);
      at_array_header_dispose(&array->header);
      free(array);
    }
    *array_ptr = NULL;
  }
}

void
at_array_header_dispose(AtArrayHeader* header){
  free(header->shape);
}
