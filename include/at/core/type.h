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
#ifndef AT_TYPE_H
#define AT_TYPE_H
#include <stdint.h>
typedef enum{
  AT_SCC_TARJAN,
  AT_SCC_KOSARAJU,
  AT_SCC_DIJKSTRA,
}AtSCCAlgorithm;
typedef int8_t (*AtCompareFunc)    (void* data1, void* data2);
typedef int8_t (*AtCompareDataFunc)(void* data1, void* data2, void* user_data);
typedef void   (*AtDataFunc)       (void* data1, void* user_data);

#define INT8_TO_POINTER(i)   ((void*)(int64_t)(i))
#define INT16_TO_POINTER(i)  ((void*)(int64_t)(i))
#define INT32_TO_POINTER(i)  ((void*)(int64_t)(i))
#define INT64_TO_POINTER(i)  ((void*)(int64_t)(i))
#define UINT8_TO_POINTER(i)  ((void*)(uint64_t)(i))
#define UINT16_TO_POINTER(i) ((void*)(uint64_t)(i))
#define UINT32_TO_POINTER(i) ((void*)(uint64_t)(i))
#define UINT64_TO_POINTER(i) ((void*)(uint64_t)(i))

#define POINTER_TO_INT8(i)   ((int8_t)  (int64_t)(i))
#define POINTER_TO_INT16(i)  ((int16_t) (int64_t)(i))
#define POINTER_TO_INT32(i)  ((int32_t) (int64_t)(i))
#define POINTER_TO_INT64(i)  ((int64_t) (int64_t)(i))
#define POINTER_TO_UINT8(i)  ((uint8_t) (uint64_t)(i))
#define POINTER_TO_UINT16(i) ((uint16_t)(uint64_t)(i))
#define POINTER_TO_UINT32(i) ((uint32_t)(uint64_t)(i))
#define POINTER_TO_UINT64(i) ((uint64_t)(uint64_t)(i))

#endif
