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
#ifndef AT_ARRAY_H
#define AT_ARRAY_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <at/core/macro.h>
AT_BEGIN_DECLS
/*=============================================================================
 STRUCTURE
 ============================================================================*/
/**
 * @brief @~Brazilian Cabeçalho do Vetor Multidimensional
 *
 *
 * Multidimensional Array (
 */
typedef struct AtArrayHeader{
  uint64_t* shape;       // 00+8: size of each dimension
  uint64_t* step;        // 08+8: offset between consecutive elements
                         //              of each dimension
  uint64_t  num_elements;// 16+8: total number of elements
  uint8_t   dim;         // 24+1: dimension
  uint8_t   owns_data;   // 25+1: true if they can delete its own data
  uint8_t   elemsize;    // 26+1: size of element
  uint8_t   alignment[5];// 27+5: explicit padding
}AtArrayHeader;          // Total: 32B

/**
 * @brief @~Brazilian Vetor Multidimensional
 *
 * Para criar um vetor, é necessário saber a dimensão (`dim`) e o tamanho
 * (`shape`) de cada uma. Por exemplo, podemos criar uma matriz bidimensional
 * 5x4: dim = 2 e `shape={5,4}`. Note-se que o tamanho é {Y,X} e não {X,Y}, e
 * dimensões adicionais devem ser colocadas primeiro ({Z,Y,X} por exemplo).
 * dimensão é uint8 e shape é um array de uint64.
 */
typedef struct AtArrayU8{
  AtArrayHeader h;  // 00-31
  uint8_t      *data;    // 32-39
}AtArrayU8;        // Total: 40B
typedef struct AtArrayU16{
  AtArrayHeader h;
  uint16_t     *data;
}AtArrayU16;
typedef struct AtArrayU32{
  AtArrayHeader h;
  uint32_t     *data;
}AtArrayU32;
typedef struct AtArrayU64{
  AtArrayHeader h;
  uint64_t     *data;
}AtArrayU64;
typedef struct AtArrayI8{
  AtArrayHeader h;
  int8_t       *data;
}AtArrayI8;
typedef struct AtArrayI16{
  AtArrayHeader h;
  int16_t      *data;
}AtArrayI16;
typedef struct AtArrayI32{
  AtArrayHeader h;
  int32_t      *data;
}AtArrayI32;
typedef struct AtArrayI64{
  AtArrayHeader h;
  int64_t      *data;
}AtArrayI64;
typedef struct AtArrayF32{
  AtArrayHeader h;
  float        *data;
}AtArrayF32;
typedef struct AtArrayD64{
  AtArrayHeader h;
  double       *data;
}AtArrayD64;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/

void
at_index_to_nd(uint8_t dim, uint64_t* step, uint64_t s, uint64_t* s_nd);

void
at_index_to_1d(uint8_t dim, uint64_t* step, int64_t* s_nd, uint64_t* s);
#define at_array_index_to_nd(array, s, s_nd) at_index_to_nd(array->h.dim, array->h.step,s,s_nd)
#define at_array_index_to_1d(array, s_nd, s) at_index_to_1d(array->h.dim, array->h.step,s_nd,s)

#define at_array_max(array) _Generic((array), \
  AtArrayU8*:at_arrayu8_max)(array)

void
at_array_header_init(AtArrayHeader* header);

void
at_array_header_set(AtArrayHeader* header, uint8_t dim, uint64_t* shape);

void
at_array_header_dispose(AtArrayHeader* header);


#define at_array_destroy(array_ptr) _Generic((array_ptr), \
  AtArrayU8**: at_arrayu8_destroy             \
)(array_ptr)

// uint8_t
// --------
AtArrayU8*
at_arrayu8_create();

AtArrayU8*
at_arrayu8_new(uint8_t dim, uint64_t* shape);

AtArrayU8*
at_arrayu8_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data, bool copy);

AtArrayU8*
at_arrayu8_zeros(uint8_t dim, uint64_t* shape);

AtArrayU8*
at_arrayu8_ones(uint8_t dim, uint64_t* shape);

void
at_arrayu8_fill(AtArrayU8* array, uint8_t value);

uint8_t
at_arrayu8_max(AtArrayU8* array);

void
at_arrayu8_destroy(AtArrayU8** array);

// uint16_t
// --------
AtArrayU16*
at_arrayu16_new(uint8_t dim, uint64_t* shape);

#define at_array_save(arrays,names,num,filename) at_arrayu8_save((AtArrayU8**)arrays,names,num,filename);

void
at_arrayu8_save(AtArrayU8** arrays, char** names, uint8_t num, const char* filename);

AtArrayU8*
at_array_load(char*** namesp, uint8_t *nump, const char* filename);

void
at_arrayu16_destroy(AtArrayU16** arp);

// uint64_t
// --------

AtArrayU64*
at_arrayu64_create();

AtArrayU64*
at_arrayu64_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data, bool copy);

void
at_arrayu64_fill(AtArrayU64* array, uint64_t value);

void
at_arrayu64_destroy(AtArrayU64** array);


AT_END_DECLS
#endif
