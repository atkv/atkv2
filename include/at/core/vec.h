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
#ifndef AT_VEC_H
#define AT_VEC_H
#include <stdint.h>

typedef uint8_t __attribute__((vector_size(2)))   v2u8;
typedef uint8_t __attribute__((vector_size(4)))   v4u8;

typedef uint16_t __attribute__((vector_size(4)))  v2u16;
typedef uint16_t __attribute__((vector_size(8)))  v4u16;

typedef uint32_t __attribute__((vector_size(8)))  v2u32;
typedef uint32_t __attribute__((vector_size(16))) v4u32;

typedef uint64_t __attribute__((vector_size(16)))  v2u64;
typedef uint64_t __attribute__((vector_size(32)))  v4u64;

typedef int8_t __attribute__((vector_size(2)))    v2i8;
typedef int8_t __attribute__((vector_size(4)))    v4i8;

typedef int16_t __attribute__((vector_size(4)))   v2i16;
typedef int16_t __attribute__((vector_size(8)))   v4i16;

typedef int32_t __attribute__((vector_size(8)))   v2i32;
typedef int32_t __attribute__((vector_size(16)))   v4i32;

typedef int64_t __attribute__((vector_size(16)))   v2i64;
typedef int64_t __attribute__((vector_size(32)))   v4i64;

typedef float __attribute__((vector_size(8))) v2f32;
typedef float __attribute__((vector_size(16))) v4f32;

typedef double __attribute__((vector_size(16))) v2d64;
typedef double __attribute__((vector_size(32))) v4d64;

#define AT_DECLARE_VEC2(vectype, uniontype, type) \
typedef union{                                 \
  v2##vectype  v;                              \
  type d[2];                                   \
  struct{ type x; type y;};                    \
  struct{ type width; type height;};           \
}AtVec2##uniontype;

#define AT_DECLARE_VEC3(vectype, uniontype, type) \
typedef union{                                    \
  v4##vectype  v;                                 \
  type d[4];                                      \
  struct{ type x; type y; type z;};               \
  struct{ type r; type g; type b;};               \
}AtVec3##uniontype;

#define AT_DECLARE_VEC4(vectype, uniontype, type)   \
typedef union{                                      \
  v4##vectype  v;                                   \
  type d[4];                                        \
  struct{ type x; type y; union{struct{type z; type w;};struct{type width; type height;};};};        \
  struct{ type x0; type y0; type x1; type y1;};     \
  struct{ type r; type g; type b; type a;};         \
}AtVec4##uniontype;

AT_DECLARE_VEC2(u8, U8, uint8_t)
AT_DECLARE_VEC2(u16,U16,uint16_t)
AT_DECLARE_VEC2(u32,U32,uint32_t)
AT_DECLARE_VEC2(u64,U64,uint64_t)
AT_DECLARE_VEC2(i8, I8, int8_t)
AT_DECLARE_VEC2(i16,I16,int16_t)
AT_DECLARE_VEC2(i32,I32,int32_t)
AT_DECLARE_VEC2(i64,I64,int64_t)
AT_DECLARE_VEC2(f32,F32,float)
AT_DECLARE_VEC2(d64,D64,double)

AT_DECLARE_VEC3(u8, U8, uint8_t)
AT_DECLARE_VEC3(u16,U16,uint16_t)
AT_DECLARE_VEC3(u32,U32,uint32_t)
AT_DECLARE_VEC3(u64,U64,uint64_t)
AT_DECLARE_VEC3(i8, I8, int8_t)
AT_DECLARE_VEC3(i16,I16,int16_t)
AT_DECLARE_VEC3(i32,I32,int32_t)
AT_DECLARE_VEC3(i64,I64,int64_t)
AT_DECLARE_VEC3(f32,F32,float)
AT_DECLARE_VEC3(d64,D64,double)

AT_DECLARE_VEC4(u8, U8, uint8_t)
AT_DECLARE_VEC4(u16,U16,uint16_t)
AT_DECLARE_VEC4(u32,U32,uint32_t)
AT_DECLARE_VEC4(u64,U64,uint64_t)
AT_DECLARE_VEC4(i8, I8, int8_t)
AT_DECLARE_VEC4(i16,I16,int16_t)
AT_DECLARE_VEC4(i32,I32,int32_t)
AT_DECLARE_VEC4(i64,I64,int64_t)
AT_DECLARE_VEC4(f32,F32,float)
AT_DECLARE_VEC4(d64,D64,double)

AtVec3D64
at_vec3d64_normalize(AtVec3D64 vec);
#endif
