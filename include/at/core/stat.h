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
#ifndef AT_STAT_H
#define AT_STAT_H
#include <at/core.h>

/// @cond IGNORE
#define AT_DECLARE_ARRAY_OP(op)                    \
op(u8 ,U8 ); op(u16,U16); op(u32,U32); op(u64,U64);\
op(i8 ,I8 ); op(i16,I16); op(i32,I32); op(i64,I64);

#define AT_DECLARE_ARRAY_OP3(op)                                                    \
op(u8 ,U8 ,uint8_t );op(u16,U16,uint16_t);op(u32,U32,uint32_t);op(u64,U64,uint64_t);\
op(i8 ,I8 , int8_t );op(i16,I16, int16_t);op(i32,I32, int32_t);op(i64,I64, int64_t);\

#define AT_DECLARE_ARRAY_JACCARD(lower,UPPER)     \
double                                            \
at_array##lower##_jaccard(AtArray##UPPER* a1, AtArray##UPPER* a2);
#define AT_DECLARE_ARRAY_DICE(lower,UPPER)     \
double                                            \
at_array##lower##_dice(AtArray##UPPER* a1, AtArray##UPPER* a2);


AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_JACCARD)
AT_DECLARE_ARRAY_OP(AT_DECLARE_ARRAY_DICE)

#undef AT_DECLARE_ARRAY_JACCARD
#undef AT_DECLARE_ARRAY_DICE
#undef AT_DECLARE_ARRAY_OP
#undef AT_DECLARE_ARRAY_OP3

/// @endcond

#endif
