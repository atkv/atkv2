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
/*! \file */
typedef struct AtArrayHeader AtArrayHeader;

/**
 * @brief A directed weighted grid graph
 */
struct AtArrayHeader{
  uint64_t* shape;       /*!< size of each dimension */                               /*00+8*/
  uint64_t* step;        /*!< offset between consecutive elements of each dimension *//*08+8*/
  uint64_t  num_elements;/*!< total number of elements */                             /*16+8*/
  uint8_t   dim;         /*!< dimension */                                            /*24+1*/
  uint8_t   owns_data;   /*!< true if they can delete its own data */                 /*25+1*/
  uint8_t   elemsize;    /*!< size of element */                                      /*26+1*/
  uint8_t   alignment[5];/*!< explicit padding */                                     /*27+5*/
};          // Total: 32B

/**
 * @brief Vetor Multidimensional
 *
 * Para criar um vetor, é necessário saber a dimensão (`dim`) e o tamanho
 * (`shape`) de cada uma. Por exemplo, podemos criar uma matriz bidimensional
 * 5x4: dim = 2 e `shape={5,4}`. Note-se que o tamanho é {Y,X} e não {X,Y}, e
 * dimensões adicionais devem ser colocadas primeiro ({Z,Y,X} por exemplo).
 * dimensão é uint8 e shape é um array de uint64.
 */
typedef struct AtArrayU8{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  uint8_t      *data;    /*!< Data */  /*32-39*/
}AtArrayU8;        // Total: 40B

/**
 * @brief Array Uint16
 */
typedef struct AtArrayU16{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  uint16_t     *data;    /*!< Data */  /*32-39*/
}AtArrayU16;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayU32{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  uint32_t     *data;    /*!< Data */  /*32-39*/
}AtArrayU32;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayU64{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  uint64_t     *data;    /*!< Data */  /*32-39*/
}AtArrayU64;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayI8{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  int8_t       *data;    /*!< Data */  /*32-39*/
}AtArrayI8;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayI16{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  int16_t      *data;    /*!< Data */  /*32-39*/
}AtArrayI16;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayI32{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  int32_t      *data;    /*!< Data */  /*32-39*/
}AtArrayI32;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayI64{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  int64_t      *data;    /*!< Data */  /*32-39*/
}AtArrayI64;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayF32{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  float        *data;    /*!< Data */  /*32-39*/
}AtArrayF32;
/**
 * @brief Array Uint16
 */
typedef struct AtArrayD64{
  AtArrayHeader h;       /*!< Header *//*00-31*/
  double       *data;    /*!< Data */  /*32-39*/
}AtArrayD64;

/*=============================================================================
 FUNCTIONS
 ============================================================================*/
/**
 * @brief at_index_to_nd
 * @param dim
 * @param step
 * @param s
 * @param s_nd
 */
void
at_index_to_nd(uint8_t dim, uint64_t* step, uint64_t s, uint64_t* s_nd);
/**
 * @brief at_index_to_1d
 * @param dim
 * @param step
 * @param s_nd
 * @param s
 */
void
at_index_to_1d(uint8_t dim, uint64_t* step, int64_t* s_nd, uint64_t* s);
/**
 * @brief Convert 1D to ND
 */
#define at_array_index_to_nd(array, s, s_nd) at_index_to_nd(array->h.dim, array->h.step,s,s_nd)
/**
 * @brief Convert ND to 1D
 */
#define at_array_index_to_1d(array, s_nd, s) at_index_to_1d(array->h.dim, array->h.step,s_nd,s)
/**
 * @brief File max of array
 */
#define at_array_max(array) _Generic((array), \
  AtArrayU8*:at_arrayu8_max)(array)

/**
 * @brief at_array_header_init
 * @param header
 */
void
at_array_header_init(AtArrayHeader* header);

/**
 * @brief at_array_header_set
 * @param header
 * @param dim
 * @param shape
 */
void
at_array_header_set(AtArrayHeader* header, uint8_t dim, uint64_t* shape);

/**
 * @brief at_array_header_dispose
 * @param header
 */
void
at_array_header_dispose(AtArrayHeader* header);

/**
 * @brief Delete array
 */
#define at_array_destroy(array_ptr) _Generic((array_ptr), \
  AtArrayU8**: at_arrayu8_destroy             \
)(array_ptr)

// uint8_t
// --------
/**
 * @brief at_arrayu8_create
 * @return
 */
AtArrayU8*
at_arrayu8_create();
/**
 * @brief at_arrayu8_new
 * @param dim
 * @param shape
 * @return
 */
AtArrayU8*
at_arrayu8_new(uint8_t dim, uint64_t* shape);
/**
 * @brief at_arrayu8_new_with_data
 * @param dim
 * @param shape
 * @param data
 * @param copy
 * @return
 */
AtArrayU8*
at_arrayu8_new_with_data(uint8_t dim, uint64_t* shape, uint8_t* data, bool copy);
/**
 * @brief at_arrayu8_zeros
 * @param dim
 * @param shape
 * @return
 */
AtArrayU8*
at_arrayu8_zeros(uint8_t dim, uint64_t* shape);
/**
 * @brief at_arrayu8_ones
 * @param dim
 * @param shape
 * @return
 */
AtArrayU8*
at_arrayu8_ones(uint8_t dim, uint64_t* shape);
/**
 * @brief at_arrayu8_fill
 * @param array
 * @param value
 */
void
at_arrayu8_fill(AtArrayU8* array, uint8_t value);
/**
 * @brief at_arrayu8_max
 * @param array
 * @return
 */
uint8_t
at_arrayu8_max(AtArrayU8* array);
/**
 * @brief at_arrayu8_destroy
 * @param array
 */
void
at_arrayu8_destroy(AtArrayU8** array);

// uint16_t
// --------
/**
 * @brief at_arrayu16_new
 * @param dim
 * @param shape
 * @return
 */
AtArrayU16*
at_arrayu16_new(uint8_t dim, uint64_t* shape);
/**
 * @brief Save array to file
 */
#define at_array_save(arrays,names,num,filename) at_arrayu8_save((AtArrayU8**)arrays,names,num,filename);
/**
 * @brief at_arrayu8_save
 * @param arrays
 * @param names
 * @param num
 * @param filename
 */
void
at_arrayu8_save(AtArrayU8** arrays, char** names, uint8_t num, const char* filename);
/**
 * @brief at_array_load
 * @param namesp
 * @param nump
 * @param filename
 * @return
 */
AtArrayU8*
at_array_load(char*** namesp, uint8_t *nump, const char* filename);
/**
 * @brief at_arrayu16_destroy
 * @param arp
 */
void
at_arrayu16_destroy(AtArrayU16** arp);

// uint64_t
// --------
/**
 * @brief at_arrayu64_create
 * @return
 */
AtArrayU64*
at_arrayu64_create();
/**
 * @brief at_arrayu64_new_with_data
 * @param dim
 * @param shape
 * @param data
 * @param copy
 * @return
 */
AtArrayU64*
at_arrayu64_new_with_data(uint8_t dim, uint64_t* shape, uint64_t* data, bool copy);
/**
 * @brief at_arrayu64_fill
 * @param array
 * @param value
 */
void
at_arrayu64_fill(AtArrayU64* array, uint64_t value);
/**
 * @brief at_arrayu8_set_nd
 * @param ar
 * @param coords
 * @param value
 */
void
at_arrayu8_set_nd(AtArrayU8* ar, uint64_t* coords, uint8_t value);
/**
 * @brief at_arrayu8_set_nd_many
 * @param ar
 * @param coords
 * @param value
 */
void
at_arrayu8_set_nd_many(AtArrayU8* ar, uint64_t *coords, uint8_t* value);

/**
 * @brief at_arrayu64_destroy
 * @param array
 */
void
at_arrayu64_destroy(AtArrayU64** array);
/**
 * @brief at_arrayu32_new
 * @param dim
 * @param shape
 * @return
 */
AtArrayU32*
at_arrayu32_new(uint8_t dim, uint64_t* shape);
/**
 * @brief at_arrayu64_new_with_data
 * @param dim
 * @param shape
 * @param data
 * @param copy
 * @return
 */
AtArrayU32*
at_arrayu32_new_with_data(uint8_t dim, uint64_t* shape, uint32_t* data, bool copy);

/**
 * @brief at_arrayu32_destroy
 * @param array_ptr
 */
void
at_arrayu32_destroy(AtArrayU32** array_ptr);

AT_END_DECLS
#endif
