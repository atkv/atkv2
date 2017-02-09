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

#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif

#ifndef AT_ARRAY_H
#define AT_ARRAY_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <at/core.h>
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
  uint64_t* cstep;       /*!< offsets in local (non-parent) space */                  /*16+8*/
  uint64_t  nelem;       /*!< total number of elements */                             /*24+8*/
  uint8_t   dim;         /*!< dimension */                                            /*32+1*/
  uint8_t   owns_data;   /*!< true if they can delete its own data */                 /*33+1*/
  uint8_t   elemsize;    /*!< size of element */                                      /*34+1*/
  uint8_t   alignment[5];/*!< explicit padding */                                     /*35+5*/
};          // Total: 40B

/**
 * @brief Vetor Multidimensional
 *
 * Para criar um vetor, é necessário saber a dimensão (`dim`) e o tamanho
 * (`shape`) de cada uma. Por exemplo, podemos criar uma matriz bidimensional
 * 5x4: dim = 2 e `shape={5,4}`. Note-se que o tamanho é {Y,X} e não {X,Y}, e
 * dimensões adicionais devem ser colocadas primeiro ({Z,Y,X} por exemplo).
 * dimensão é uint8 e shape é um array de uint64.
 */
#define AT_DECLARE_ARRAY(UPPER,type)              \
typedef struct AtArray##UPPER{                    \
  AtArrayHeader h;       /*!< Header *//*00-31*/  \
  type         *data;    /*!< Data */  /*32-39*/  \
}AtArray##UPPER;        // Total: 40B
AT_DECLARE_ARRAY(U8,uint8_t)
AT_DECLARE_ARRAY(U16,uint16_t)
AT_DECLARE_ARRAY(U32,uint32_t)
AT_DECLARE_ARRAY(U64,uint64_t)
AT_DECLARE_ARRAY(I8,int8_t)
AT_DECLARE_ARRAY(I16,int16_t)
AT_DECLARE_ARRAY(I32,int32_t)
AT_DECLARE_ARRAY(I64,int64_t)
AT_DECLARE_ARRAY(F32,float)
AT_DECLARE_ARRAY(D64,double)
#undef AT_DECLARE_ARRAY
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

/// @cond IGNORE
#define AT_DECLARE_ARRAY_OP(op)             \
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

#define AT_DECLARE_ARRAY_OP3(op)            \
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

// at_array_create
#define AT_DECLARE_ARRAY_CREATE(lower, UPPER)              AtArray##UPPER* at_array##lower##_create();
#define AT_DECLARE_ARRAY_NEW(lower, UPPER)                 AtArray##UPPER* at_array##lower##_new(uint8_t dim, uint64_t* shape);
#define AT_DECLARE_ARRAY_NEW_WITH_DATA(lower, UPPER, type) AtArray##UPPER* at_array##lower##_new_with_data(uint8_t dim, uint64_t* shape, type* data, bool copy);
#define AT_DECLARE_ARRAY_ZEROS(lower, UPPER)               AtArray##UPPER* at_array##lower##_zeros(uint8_t dim, uint64_t* shape);
#define AT_DECLARE_ARRAY_ONES(lower, UPPER)                AtArray##UPPER* at_array##lower##_ones(uint8_t dim, uint64_t* shape);
#define AT_DECLARE_ARRAY_FILL(lower, UPPER, type)                     void at_array##lower##_fill(AtArray##UPPER* array, type value);
#define AT_DECLARE_ARRAY_ADD_SCALAR(lower, UPPER, type)               void at_array##lower##_add_scalar(AtArray##UPPER* array, type value);
#define AT_DECLARE_ARRAY_ADD_SCALAR_CLAMPED(lower, UPPER, type)       void at_array##lower##_add_scalar_clamped(AtArray##UPPER* array, type value);
#define AT_DECLARE_ARRAY_MAX(lower, UPPER, type)                      type at_array##lower##_max(AtArray##UPPER* array);
#define AT_DECLARE_ARRAY_DESTROY(lower, UPPER)                        void at_array##lower##_destroy(AtArray##UPPER** array);
#define AT_DECLARE_ARRAY_GET_1D(lower, UPPER, type)                   type at_array##lower##_get_1d(AtArray##UPPER* array, uint64_t idx);
#define AT_DECLARE_ARRAY_GET_ND(lower, UPPER, type)                   type at_array##lower##_get_nd(AtArray##UPPER* array, uint64_t* idx);
#define AT_DECLARE_ARRAY_SET_1D(lower, UPPER, type)                   void at_array##lower##_set_1d(AtArray##UPPER* array, uint64_t idx, type value);
#define AT_DECLARE_ARRAY_SET_ND(lower, UPPER, type)                   void at_array##lower##_set_nd(AtArray##UPPER* array, uint64_t* idx, type value);
#define AT_DECLARE_ARRAY_SUB(lower, UPPER, type)                      void at_array##lower##_sub(AtArray##UPPER* array, AtRange* ranges, AtArray##UPPER** outputp, uint8_t copy);

AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_CREATE)
AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_NEW)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_NEW_WITH_DATA)
AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_ZEROS)
AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_ONES)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_FILL)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_ADD_SCALAR)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_ADD_SCALAR_CLAMPED)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_MAX)
AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_DESTROY)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_GET_1D)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_GET_ND)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_SET_1D)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_SET_ND)
AT_DECLARE_ARRAY_OP3(AT_DECLARE_ARRAY_SUB)

#undef AT_DECLARE_ARRAY_CREATE
#undef AT_DECLARE_ARRAY_NEW
#undef AT_DECLARE_ARRAY_NEW_WITH_DATA
#undef AT_DECLARE_ARRAY_ZEROS
#undef AT_DECLARE_ARRAY_ONES
#undef AT_DECLARE_ARRAY_FILL
#undef AT_DECLARE_ARRAY_ADD_SCALAR
#undef AT_DECLARE_ARRAY_ADD_SCALAR_CLAMPED
#undef AT_DECLARE_ARRAY_MAX
#undef AT_DECLARE_ARRAY_DESTROY
#undef AT_DECLARE_ARRAY_GET_1D
#undef AT_DECLARE_ARRAY_GET_ND
#undef AT_DECLARE_ARRAY_SET_1D
#undef AT_DECLARE_ARRAY_SET_ND
#undef AT_DECLARE_ARRAY_SUB

#undef AT_DECLARE_ARRAY_OP
#undef AT_DECLARE_ARRAY_OP3

#define at_arrayu8_squeeze(ar)  at_arrayheader_squeeze(&ar->h)
#define at_arrayu16_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayu32_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayu64_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayi8_squeeze(ar)  at_arrayheader_squeeze(&ar->h)
#define at_arrayi16_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayi32_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayi64_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayf32_squeeze(ar) at_arrayheader_squeeze(&ar->h)
#define at_arrayd64_squeeze(ar) at_arrayheader_squeeze(&ar->h)

/// @endcond

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
 * @brief at_arrayu8_set_1d_many
 * @param ar
 * @param index
 * @param value
 */
void
at_arrayu8_set_1d_many(AtArrayU8* ar, uint64_t index, uint8_t* value);
/**
 * @brief at_arrayu8_set_nd_many
 * @param ar
 * @param coords
 * @param value
 */
void
at_arrayu8_set_nd_many(AtArrayU8* ar, uint64_t *coords, uint8_t* value);
/**
 * @brief at_arrayu16_sub_u8
 * @param ar
 * @param ranges
 * @param output
 */
void
at_arrayu16_sub_u8(AtArrayU16* ar, AtRange* ranges, AtArrayU8 **output);
/**
 * @brief at_arrayheader_squeeze
 * @param h
 */
void
at_arrayheader_squeeze(AtArrayHeader* h);

AT_END_DECLS
#endif
