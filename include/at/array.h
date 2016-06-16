#ifndef AT_ARRAY_H
#define AT_ARRAY_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
/**
 * Multidimensional Array
 */
typedef struct AtArrayHeader{
  uint64_t* shape;       // 00-07: size of each dimension
  uint64_t* step;        // 08-15: offset between consecutive elements
                         //              of each dimension
  uint64_t  num_elements;// 16-23: total number of elements
  uint8_t   dim;         // 24-24: dimension
  uint8_t   owns_data;   // 25-25: true if they can delete its own data
  uint8_t   alignment[6];// 26-31: explicit padding
}AtArrayHeader;          // Total: 32B

#define AtArray(type) AtArray_##type

typedef struct AtArray_uint8_t{
  AtArrayHeader header;  // 00-39
  uint8_t      *data;    // 40-47
}AtArray_uint8_t;        // Total: 48B
typedef struct AtArray_uint16_t{
  AtArrayHeader header;
  uint16_t     *data;
}AtArray_uint16_t;
typedef struct AtArray_uint32_t{
  AtArrayHeader header;
  uint32_t     *data;
}AtArray_uint32_t;
typedef struct AtArray_uint64_t{
  AtArrayHeader header;
  uint64_t     *data;
}AtArray_uint64_t;
typedef struct AtArray_int8_t{
  AtArrayHeader header;
  int8_t       *data;
}AtArray_int8_t;
typedef struct AtArray_int16_t{
  AtArrayHeader header;
  int16_t      *data;
}AtArray_int16_t;
typedef struct AtArray_int32_t{
  AtArrayHeader header;
  int32_t      *data;
}AtArray_int32_t;
typedef struct AtArray_int64_t{
  AtArrayHeader header;
  int64_t      *data;
}AtArray_int64_t;
typedef struct AtArray_float{
  AtArrayHeader header;
  float        *data;
}AtArray_float;
typedef struct AtArray_double{
  AtArrayHeader header;
  double       *data;
}AtArray_double;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/

void
at_array_header_init(AtArrayHeader* header);

void
at_array_header_set(AtArrayHeader* header, uint8_t dim, uint64_t* shape);

// uint8_t
// --------
AtArray_uint8_t*
at_array_uint8_t_create();

AtArray_uint8_t*
at_array_uint8_t_new(uint8_t dim, uint64_t* shape);

AtArray_uint8_t*
at_array_uint8_t_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data, uint8_t copy);

AtArray_uint8_t*
at_array_uint8_t_zeros(uint8_t dim, uint64_t* shape);

AtArray_uint8_t*
at_array_uint8_t_ones(uint8_t dim, uint64_t* shape);

void
at_array_uint8_t_fill(AtArray_uint8_t* array, uint8_t value);

void
at_array_uint8_t_destroy(AtArray_uint8_t* array);

// uint64_t
// --------

AtArray_uint64_t*
at_array_uint64_t_create();

AtArray_uint64_t*
at_array_uint64_t_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data, uint8_t copy);

void
at_array_uint64_t_fill(AtArray_uint64_t* array, uint64_t value);

void
at_array_uint64_t_destroy(AtArray_uint64_t* array);

#endif
