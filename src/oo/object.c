#include <at/oo.h>

//-----------------------------------------------------
// PRIVATE API
//-----------------------------------------------------

static void at_object_ctor(AtObject* self){
  self->refcount = 1;
}

static void at_object_dispose(AtObject* self){

}

static void at_object_finalize(AtObject* self){

}

static void at_object_class_ctor(AtObjectClass* klass){
  klass->dispose  = at_object_dispose;
  klass->finalize = at_object_finalize;
}

__attribute__((constructor))
static void init(){
  at_type_register_primitive(AT_TYPE_OBJECT,
                             sizeof(AtObjectClass),
                             sizeof(AtObject),
                             (AtInstanceCtorFunc)at_object_ctor,
                             (AtClassCtorFunc)   at_object_class_ctor);
}

//-----------------------------------------------------
// PUBLIC API
//-----------------------------------------------------

void*
at_new(AtType type, ...){
  AtObject* object = (AtObject*)at_type_instance_new(type);
  return object;
}

void
at_object_ref(void* _obj){
  AtObject* obj = _obj;
  obj->refcount++;
}

void
at_object_unref(void* _obj){
  AtObject* obj = _obj;
  if(--obj->refcount == 0){
    // delete object
    AtObjectClass* klass = (AtObjectClass*)obj->super.klass;
    klass->dispose(obj);
    klass->finalize(obj);
    at_type_free_instance((AtTypeInstance*)obj);
  }
}

