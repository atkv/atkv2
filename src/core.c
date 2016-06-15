#include <core.h>

size_t
at_get_num_elements(uint8_t dim, uint64_t* shape){
  uint8_t i;
  size_t num_elements = 1;
  for(i = 0; i < dim; i++)
    num_elements *= shape;
  return
}

AtArray*
at_array_new(uint8_t dim, uint64_t* shape){
  size_t data_bytes = 1;


  AtArray* array = (AtArray*)malloc(sizeof(AtArray) + sizeof(uint64_t) * dim);
  array->dim     = dim;
  array->shape   = (uint64_t*)&array[1];

}
