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
#if !defined(AT_OO_H_INSIDE)
#error "Only <at/oo.h> can be included directly."
#endif
#ifndef AT_OO_TYPE_H
#define AT_OO_TYPE_H

#include <at/oo.h>
AT_BEGIN_DECLS

//------------------------------------------------------------------------
// SOME ALIAS
//------------------------------------------------------------------------
typedef uint64_t               AtType;
typedef struct _AtTypeInstance AtTypeInstance;
typedef struct _AtTypeClass    AtTypeClass;

#define AT_TYPE_PRIMITIVE_MAX 255
#define AT_TYPE_INVALID  0
#define AT_TYPE_VOID     1
#define AT_TYPE_U8       2
#define AT_TYPE_U6       3
#define AT_TYPE_U2       4
#define AT_TYPE_U64      5
#define AT_TYPE_I8       6
#define AT_TYPE_I16      7
#define AT_TYPE_I32      8
#define AT_TYPE_I64      9
#define AT_TYPE_F32      10
#define AT_TYPE_D64      11
#define AT_TYPE_BOOL     12
#define AT_TYPE_STRING   13
#define AT_TYPE_OBJECT   14

//------------------------------------------------------------------------
// STRUCTURES
//------------------------------------------------------------------------
/**
 * @brief The _AtTypeClass struct
 */
struct _AtTypeClass{
  AtType       type;
};

/**
 * @brief The _AtTypeInstance struct
 */
struct _AtTypeInstance{
  AtTypeClass* klass;
};

//------------------------------------------------------------------------
// PUBLIC API
//------------------------------------------------------------------------

/**
 * @brief Prototype for instance constructors
 */
typedef void
(*AtInstanceCtorFunc)(AtTypeInstance* instance, void* atclass);

/**
 * @brief Prototype for class constructors
 */
typedef void
(*AtClassCtorFunc)(AtTypeClass* atclass, void* classdata);

/**
 * @brief Declares a new class
 */
#define AT_DECLARE_CLASS(ObjName, funcname, PREFIX, OBJNAME, ParentObj) \
  AtType funcname##_get_type();                                         \
  typedef struct _##ObjName ObjName;                                    \
  typedef struct {ParentObj##Class super;} ObjName##Class;              \
  _AT_DEFINE_AUTOPTR_CHAINUP (ObjName, ParentObj)                       \
  static inline ObjName* PREFIX##_##OBJNAME(void* obj){                 \
    return AT_TYPE_INSTANCE_CAST(obj, funcname##_get_type(),ObjName);}  \
  static inline bool PREFIX##_IS_##OBJNAME(void* obj){                  \
    return AT_TYPE_INSTANCE_IS(obj, funcname##_get_type());}            \

/**
 * @brief Defines a new class
 */
#define AT_DEFINE_CLASS(ObjName, funcname, TYPE_PARENT)      \
  static void funcname##_ctor(ObjName* self);                \
  static void funcname##_class_ctor(ObjName##Class *klass);  \
  AtType funcname##_get_type(){                              \
    static uint64_t id = 0;                                  \
    if(id == 0){                                             \
      id = at_type_register(TYPE_PARENT,                     \
                            sizeof(ObjName##Class),          \
                            sizeof(ObjName),                 \
                            (AtInstanceCtorFunc)funcname##_ctor,   \
                            (AtClassCtorFunc)funcname##_class_ctor \
      );                                                     \
    }                                                        \
    return id;                                               \
                                                             \
  }                                                          \
                                                             \
                                                             \

#define AT_DEFINE_CLASS_CTOR(ObjName, funcname, TYPE_PARENT)       \
  AT_DEFINE_CLASS(ObjName, funcname, TYPE_PARENT)                  \
  static void funcname##_class_ctor(ObjName##Class *klass){}

#define AT_DEFINE_CLASS_SIMPLE(ObjName, funcname, TYPE_PARENT, ParentObj)   \
  struct _##ObjName{ ParentObj super; };                                    \
  AT_DEFINE_CLASS_CTOR(ObjName, funcname, TYPE_PARENT)                      \
  static void funcname##_ctor(ObjName *self){}                              \

#define AT_TYPE_INSTANCE_CAST(inst, type, ObjName) ((ObjName*) at_type_instance_cast((AtTypeInstance*) inst, (AtType) type))
#define AT_TYPE_INSTANCE_IS(inst, type)                        at_type_instance_is((AtTypeInstance*) inst, (AtType) type)
#define AT_TYPE_INSTANCE_BASE_IS(inst, type)                   at_type_instance_base_is((AtTypeInstance*) inst, (AtType) type)

/**
 * @brief at_type_instance_cast
 * @param i
 * @param t
 * @return
 */
AtTypeInstance*
at_type_instance_cast(AtTypeInstance* i, AtType t);

/**
 * @brief Verifies if instance class is or inherits a type
 * @param i
 * @param t
 * @return
 */
bool
at_type_instance_is(AtTypeInstance* i, AtType t);

/**
 * @brief Verifies if base instance class is of specific type
 * @param i
 * @param t
 * @return
 */
bool
at_type_instance_base_is(AtTypeInstance* i, AtType t);

/**
 * @brief at_type_free_instance
 * @param inst
 */
void
at_type_free_instance(AtTypeInstance* inst);
/**
 * @brief at_type_instance_new
 * @param type
 * @return
 */
AtTypeInstance*
at_type_instance_new(AtType type);

/**
 * @brief at_type_register
 * @param parent_type
 * @param classsize
 * @param instsize
 * @param instctor
 * @param classctor
 * @return
 */
AtType
at_type_register(AtType             parent_type,
                 uint16_t           classsize,
                 uint32_t           instsize,
                 AtInstanceCtorFunc instctor,
                 AtClassCtorFunc    classctor);

/**
 * @brief at_type_register_primitive
 * @param type
 * @param classsize
 * @param instsize
 * @param instctor
 * @param classctor
 * @return
 */
AtType
at_type_register_primitive(AtType             type,
                           uint16_t           classsize,
                           uint32_t           instsize,
                           AtInstanceCtorFunc instctor,
                           AtClassCtorFunc    classctor);

AT_END_DECLS
#endif
