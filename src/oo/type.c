#include <at/oo.h>

//-----------------------------------------------------
// PRIVATE API
//-----------------------------------------------------
typedef struct _AtTypeNode AtTypeNode;
struct _AtTypeNode{
  AtTypeClass*       klass;
  AtInstanceCtorFunc instctor;
  AtClassCtorFunc    classctor;
  uint32_t           instsize;
  uint16_t           classsize;
  uint16_t           n_supers;
  AtType             supers[1];

};
static AtTypeNode primitive_nodes[256];

static inline AtTypeNode*
at_type_node_from_id(AtType type){
  return (AtTypeNode*) type;
}

//-----------------------------------------------------
// PUBLIC API
//-----------------------------------------------------
#define NODE_IS_ANCESTOR(n, a) (n == a || (a->n_supers <= n->n_supers && n->supers[a->n_supers] == (AtType)a))

bool
at_type_instance_is(AtTypeInstance* i, AtType t){
  if(!i)                                   return false;
  else if(!i->klass)                       return false;
  else if(i->klass->type == t)             return true;
  else{
    AtTypeNode* ni = (AtTypeNode*)i->klass->type;
    AtTypeNode* nt = (AtTypeNode*)t;
    return NODE_IS_ANCESTOR(ni,nt);
  }
}

bool
at_type_instance_base_is(AtTypeInstance* i, AtType t){
  if(!i)                                   return false;
  else if(!i->klass)                       return false;
  else if(i->klass->type == t)             return true;
  else return ((AtTypeNode*)i->klass->type)->supers[0] == t;
}

void
at_type_free_instance(AtTypeInstance* inst){
  free(inst);
}

AtTypeInstance*
at_type_instance_new(AtType type){
  AtTypeNode    * node = at_type_node_from_id(type);
  AtTypeInstance* obj  = malloc(node->instsize);
  AtTypeClass   * klass;
  AtTypeNode    * pnode;
  uint16_t        i;
  obj->klass           = node->klass;

  // Calling constructors
  primitive_nodes[node->supers[0]].instctor(obj,NULL);
  for(i = 1; i < node->n_supers; i++){
    klass = (AtTypeClass*)node->supers[i];
    pnode = at_type_node_from_id(klass->type);
    pnode->instctor(obj, klass);
  }
  node->instctor(obj,node->klass);

  // Returning new object
  return obj;
}

AtTypeInstance*
at_type_instance_cast(AtTypeInstance* i, AtType t){
  if(i){
    if(i->klass){
      AtTypeNode* ni,* nt;
      ni = (AtTypeNode*)i->klass->type;
      nt = t < 255?&primitive_nodes[t]:(AtTypeNode*)t;
      if(ni == nt || (nt->n_supers <= ni->n_supers && ni->supers[nt->n_supers] == t))
        return i;

      // warn here because they are not relatives
    }
  }
  return i;
}

AtType
at_type_register(AtType             parent_type,
                 uint16_t           classsize,
                 uint32_t           instsize,
                 AtInstanceCtorFunc instctor,
                 AtClassCtorFunc    classctor){
  size_t superssize = 0;
  uint8_t nnsupers = 1;
  AtTypeNode* pnode;
  if(parent_type > 255){
    pnode      = (AtTypeNode*) parent_type;
    nnsupers  += pnode->n_supers;
    superssize = nnsupers * sizeof(AtType);
  }else{
    pnode      = &primitive_nodes[parent_type];
  }
  // Create the singleton node and fill info
  AtTypeNode* node  = malloc(sizeof(AtTypeNode)+superssize);
  node->instctor    = instctor;
  node->classctor   = classctor;
  node->n_supers    = nnsupers;
  node->classsize   = classsize;
  node->instsize    = instsize;
  node->klass       = malloc(classsize);
  memcpy(node->klass,pnode->klass,pnode->classsize);
  node->klass->type = (AtType)node;

  // Fill inheritance
  if(parent_type > 255) memcpy(node->supers,pnode->supers,superssize);
  node->supers[node->n_supers-1] = parent_type;

  // Call class constructor
  node->classctor(node->klass, NULL);

  // Return the id
  return (AtType)node;
}

AtType
at_type_register_primitive(AtType             type,
                           uint16_t           classsize,
                           uint32_t           instsize,
                           AtInstanceCtorFunc instctor,
                           AtClassCtorFunc    classctor){
  primitive_nodes[type].classctor   = classctor;
  primitive_nodes[type].classsize   = classsize;
  primitive_nodes[type].instctor    = instctor;
  primitive_nodes[type].instsize    = instsize;
  primitive_nodes[type].klass       = malloc(classsize);
  primitive_nodes[type].klass->type = type;
  primitive_nodes[type].n_supers    = 0;

  // Call class constructor
  classctor(primitive_nodes[type].klass,NULL);

  // Return the id
  return type;
}
