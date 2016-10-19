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

#ifndef AT_MACRO_H
#define AT_MACRO_H
#include <stdint.h>
#include <stdlib.h>
//------------------------------------------------------------------------
// PRIVATE API
//------------------------------------------------------------------------
#ifdef __GNUC__
#define _AT_AUTOPTR_FUNC_NAME(TypeName) at_autoptr_cleanup_##TypeName
#define _AT_AUTOPTR_TYPENAME(TypeName)  TypeName##_autoptr
#define _AT_AUTO_FUNC_NAME(TypeName)    at_auto_cleanup_##TypeName
#define _AT_CLEANUP(func)               __attribute__((cleanup(func)))
#define _AT_DEFINE_AUTOPTR_CHAINUP(ModuleObjName, ParentName) \
  typedef ModuleObjName *_AT_AUTOPTR_TYPENAME(ModuleObjName);                                          \
  static inline void _AT_AUTOPTR_FUNC_NAME(ModuleObjName) (ModuleObjName **_ptr) {                     \
    _AT_AUTOPTR_FUNC_NAME(ParentName) ((ParentName **) _ptr); }
#else
#define _AT_DEFINE_AUTOPTR_CHAINUP(ModuleObjName, ParentName)
#endif
static inline void
at_autoptr_cleanup_generic_free (void *p){free (*((void**)p));}
//------------------------------------------------------------------------
// PUBLIC API
//------------------------------------------------------------------------
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#ifdef __cplusplus
 #define AT_BEGIN_DECLS	extern "C" {
 #define AT_END_DECLS	}
#else
 #define AT_BEGIN_DECLS
 #define AT_END_DECLS
#endif /* __cplusplus */
typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   d64;

#ifdef __GNUC__
#define AT_DEFINE_AUTOPTR_CLEANUP_FUNC(TypeName, func)                                                    \
  typedef TypeName * _AT_AUTOPTR_TYPENAME(TypeName);                                                      \
  static inline void _AT_AUTOPTR_FUNC_NAME(TypeName) (TypeName **_ptr) { if (*_ptr) (func) (*_ptr); }
#define AT_DEFINE_AUTO_CLEANUP_CLEAR_FUNC(TypeName, func) \
  static inline void _AT_AUTO_FUNC_NAME(TypeName) (TypeName *_ptr) { (func) (_ptr); }
#define AT_DEFINE_AUTO_CLEANUP_FREE_FUNC(TypeName, func, none) \
  static inline void _AT_AUTO_FUNC_NAME(TypeName) (TypeName *_ptr) { if (*_ptr != none) (func) (*_ptr); }
#define at_autoptr(TypeName) _AT_CLEANUP(_AT_AUTOPTR_FUNC_NAME(TypeName)) _AT_AUTOPTR_TYPENAME(TypeName)
#define at_auto(TypeName)    _AT_CLEANUP(_AT_AUTO_FUNC_NAME(TypeName)) TypeName
#define at_autofree          _AT_CLEANUP(at_autoptr_cleanup_generic_free)
#else
#define AT_DEFINE_AUTOPTR_CLEANUP_FUNC(TypeName, func)
#define AT_DEFINE_AUTO_CLEANUP_CLEAR_FUNC(TypeName, func)
#define AT_DEFINE_AUTO_CLEANUP_FREE_FUNC(TypeName, func, none)
#endif


#endif
