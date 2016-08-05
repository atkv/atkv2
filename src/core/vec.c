#include <at/core.h>
#include <math.h>

AtVec3D64
at_vec3d64_normalize(AtVec3D64 vec){
  AtVec3D64 n;
  AtVec3D64 v2;
  v2.v = vec.v*vec.v;
  n.v  = vec.v/sqrt(v2.x+v2.y+v2.z);
  return n;
}
