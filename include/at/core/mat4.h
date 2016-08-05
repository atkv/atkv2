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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif

#ifndef AT_MAT4_H
#define AT_MAT4_H
#include <stdint.h>
#include <at/core.h>
/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef uint8_t  __attribute__((vector_size(16)))  m4u8;
typedef uint16_t __attribute__((vector_size(32)))  m4u16;
typedef uint32_t __attribute__((vector_size(64)))  m4u32;
typedef uint64_t __attribute__((vector_size(128))) m4u64;
typedef int8_t   __attribute__((vector_size(16)))  m4i8;
typedef int16_t  __attribute__((vector_size(32)))  m4i16;
typedef int32_t  __attribute__((vector_size(64)))  m4i32;
typedef int64_t  __attribute__((vector_size(128))) m4i64;
typedef float    __attribute__((vector_size(64)))  m4f32;
typedef double   __attribute__((vector_size(128))) m4d64;

#define AT_DECLARE_MAT(lower, UPPER, type)  \
typedef union{                              \
  m4##lower     m;                          \
  AtVec4##UPPER e[4];                       \
  type          d[16];                      \
}AtMat4##UPPER;
AT_DECLARE_MAT(u8 ,U8 ,uint8_t)
AT_DECLARE_MAT(u16,U16,uint16_t)
AT_DECLARE_MAT(u32,U32,uint32_t)
AT_DECLARE_MAT(u64,U64,uint64_t)
AT_DECLARE_MAT(i8 ,I8 ,int8_t)
AT_DECLARE_MAT(i16,I16,int16_t)
AT_DECLARE_MAT(i32,I32,int32_t)
AT_DECLARE_MAT(i64,I64,int64_t)
AT_DECLARE_MAT(f32,F32,float)
AT_DECLARE_MAT(d64,D64,double)

#undef AT_DECLARE_MAT

/*=============================================================================
 PUBLIC API
 ============================================================================*/

/**
 * @brief at_mat4d64_eye
 * @param mat
 */
void
at_mat4d64_eye(AtMat4D64* mat);

/**
 * @brief at_mat4d64_scale
 * @param mat
 * @param scale
 */
void
at_mat4d64_scale(AtMat4D64* mat, AtVec3D64 scale);

/**
 * @brief at_mat4d64_translate
 * @param mat
 * @param translate
 */
void
at_mat4d64_translate(AtMat4D64* mat, AtVec3D64 translation);
/**
 * @brief at_mat4d64_mult
 * @param mat1
 * @param mat2
 */
void
at_mat4d64_mult(AtMat4D64* mat1, AtMat4D64* mat2);
/**
 * @brief at_mat4d64_rotate
 * @param mat
 * @param angle
 * @param axis
 */
void
at_mat4d64_rotate(AtMat4D64* mat, double angle, AtVec3D64 axis);
/**
 * @brief at_mat4d64_ortho
 * @param l
 * @param r
 * @param b
 * @param t
 * @param n
 * @param f
 * @return
 */
AtMat4D64
at_mat4d64_ortho(double l, double r, double b, double t, double n, double f);
/**
 * @brief at_mat4d64_perspective
 * @param fov
 * @param aspect
 * @param near
 * @param far
 * @return
 */
AtMat4D64
at_mat4d64_perspective(double fov, double aspect, double near, double far);
/**
 * @brief at_mat4d64_dot
 * @param m
 * @param v
 * @return
 */
AtVec4D64
at_mat4d64_dot(AtMat4D64* m, AtVec4D64 v);

#endif
