#include <at/core.h>
#include <math.h>
void
at_mat4d64_eye(AtMat4D64* mat){
  memset(mat->d,0,sizeof(AtMat4D64));
  mat->d[0]  = 1;
  mat->d[5]  = 1;
  mat->d[10] = 1;
  mat->d[15] = 1;
}

void
at_mat4d64_scale(AtMat4D64* mat, AtVec3D64 scale){
  mat->d[0]  *= scale.x;
  mat->d[5]  *= scale.y;
  mat->d[10] *= scale.z;
}

void
at_mat4d64_translate(AtMat4D64* mat, AtVec3D64 translation){
  mat->d[12] += translation.x;
  mat->d[13] += translation.y;
  mat->d[14] += translation.z;
}

void
at_mat4d64_mult(AtMat4D64* mat1, AtMat4D64* mat2){
  AtMat4D64 res;
  memset(res.d,0,sizeof(AtMat4D64));
  uint8_t i, j, k;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      for(k = 0; k < 4; k++)
        res.e[j].d[i] += mat1->e[k].d[i] * mat2->e[j].d[k];
  *mat1 = res;
}


void
at_mat4d64_rotate(AtMat4D64* mat, double angle, AtVec3D64 axis){
  AtMat4D64 r;
  AtVec3D64 n = at_vec3d64_normalize(axis);
  // Some shortcuts
  AtVec3D64 a2;
  a2.v = n.v*n.v;

  double uv = n.x * n.y;
  double uw = n.x * n.z;
  double vw = n.y * n.z;

  double cosTheta = cos(angle / 180.0 * M_PI);
  double sinTheta = sin(angle / 180.0 * M_PI);
  double     cosM = 1 - cosTheta;

  // Calculation of rotation matrix
  r.d[ 0] = cosTheta + a2.x * cosM;
  r.d[ 1] = uv*cosM  + n.z*sinTheta;
  r.d[ 2] = uw*cosM  - n.y * sinTheta;
  r.d[ 3] = 0;
  r.d[ 4] = uv*cosM  - n.z * sinTheta;
  r.d[ 5] = cosTheta + a2.y * cosM;
  r.d[ 6] = vw*cosM  + n.z * sinTheta;
  r.d[ 7] = 0;
  r.d[ 8] = uw*cosM  + n.y*sinTheta;
  r.d[ 9] = vw*cosM  - n.x * sinTheta;
  r.d[10] = cosTheta + a2.z *cosM;
  r.d[11] = 0;
  r.d[12] = 0;
  r.d[13] = 0;
  r.d[14] = 0;
  r.d[15] = 1;

  at_mat4d64_mult(mat, &r);
}

AtMat4D64
at_mat4d64_ortho(double l, double r, double b, double t, double n, double f){
  AtMat4D64 m = {0};
  m.d[0]  =  2.0/(r-l);
  m.d[5]  =  2.0/(t-b);
  m.d[10] = -2.0/(f-n);
  m.d[12] = -(r+l)/(r-l);
  m.d[13] = -(t+b)/(t-b);
  m.d[14] = -(f+n)/(f-n);
  m.d[15] = 1;
  return m;
}

AtMat4D64
at_mat4d64_perspective(double fov, double aspect, double near, double far){
  AtMat4D64 m  = {0};
  double    fn = 1.0/(far-near);
  m.d[5 ] = 1.0/tan(fov/2.0);
  m.d[0 ] = m.d[5]/aspect;
  m.d[10] = -(far+near)*fn;
  m.d[14] = -1.0;
  m.d[11] = -(2.0*far*near)*fn;
  return m;
}

AtVec4D64
at_mat4d64_dot(AtMat4D64* m, AtVec4D64 v){
  AtVec4D64 r;
  r.x = v.x*m->e[0].x + v.y*m->e[1].x + v.z*m->e[2].x + v.w*m->e[3].x;
  r.y = v.x*m->e[0].y + v.y*m->e[1].y + v.z*m->e[2].y + v.w*m->e[3].y;
  r.z = v.x*m->e[0].z + v.y*m->e[1].z + v.z*m->e[2].z + v.w*m->e[3].z;
  r.w = v.x*m->e[0].w + v.y*m->e[1].w + v.z*m->e[2].w + v.w*m->e[3].w;
  return r;
}
