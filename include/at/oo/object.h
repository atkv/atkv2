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
#ifndef AT_OO_OBJECT_H
#define AT_OO_OBJECT_H

#include <at/oo.h>
AT_BEGIN_DECLS

/**
 * @brief Base class for all objects
 */
typedef struct _AtObject AtObject;
typedef struct _AtObjectClass AtObjectClass;

/**
 * @brief The _AtObject struct
 */
struct _AtObject{
  AtTypeInstance super;
  uint32_t       refcount;
};

/**
 * @brief The _AtObjectClass struct
 */
struct _AtObjectClass{
  AtTypeClass   super;
  void        (*dispose)    (AtObject *object);
  void        (*finalize)   (AtObject *object);
};

/**
 * @brief True if instance is an object
 */
#define AT_IS_OBJECT(i) AT_TYPE_INSTANCE_BASE_IS((i), AT_TYPE_OBJECT)

/**
 * @brief Object casting
 */
#define AT_OBJECT(object) (AT_TYPE_INSTANCE_CAST ((object), AT_TYPE_OBJECT, AtObject))

/**
 * @brief Creates a new instance
 * @param type
 * @return
 */
void *at_new(AtType type, ...);

/**
 * @brief Removes a reference for an instance and frees it if an orphan one.
 * @param obj_ptr
 *
 * Orphan objects are the ones which doesn't have any reference.
 *
 */
void
at_object_clear(AtObject** obj_ptr);

/**
 * @brief at_object_ref
 * @param _obj
 */
void
at_object_ref(void* _obj);

/**
 * @brief at_object_unref
 * @param obj
 */
void
at_object_unref(void* _obj);

/**
 * @brief:
 */
static inline bool
(at_set) (AtObject **object_ptr,
                 AtObject  *new_object){
  AtObject *old_object = *object_ptr;
  if (old_object == new_object) return false;
  if (new_object != NULL)       at_object_ref (new_object);
  *object_ptr = new_object;
  if (old_object != NULL)       at_object_unref (old_object);
  return true;
}

/**
 * @brief Assign an object to a variable
 */
#define at_set(object_ptr, new_object) (/* Check types match. */    \
  0 ? *(object_ptr) = (new_object), false :                                \
  (at_set) ((AtObject **) (object_ptr), (AtObject *) (new_object)))

/**
 * @brief Clear variable and call destroy function
 */
#define at_clear_destroy(pp, destroy)  do{       \
  void** _pp         = (void**) pp;              \
  void* _p           = *_pp;                     \
  AtDestroyNotify _d = (AtDestroyNotify) destroy;\
  if(_p){ *_pp = NULL; _d(_p);}                  \
}while(0)

/**
 * @brief Clear object
 */
#define at_clear(obj_ptr) at_clear_destroy(obj_ptr, at_object_unref)


AT_DEFINE_AUTOPTR_CLEANUP_FUNC(AtObject, at_object_unref)

AT_END_DECLS
#endif
