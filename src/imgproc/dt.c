#define __STDC_FORMAT_MACROS
#include <at/imgproc.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
//#define INF 1E20
#define UP_S(f, q, v, k) (float)((f[q] + q*q) - (f[v[k]]+v[k]*v[k]))/(float)((q-v[k])<<1);
static void at_float_dt_1D(float* f, uint64_t n, float* d, uint64_t* v, float* z){
  uint64_t k = 0, q;
  float s;
  v[0] = 0;
  z[0] = -INFINITY;
  z[1] = INFINITY;
  for(q = 1; q <= n-1; q++){
    s = UP_S(f, q, v, k);
    while(s <= z[k]){
      k--;
      s = UP_S(f, q, v, k);
    }
    k++;
    v[k] = q;
    z[k] = s;
    z[k+1] = INFINITY;
  }
  k = 0;
  for(q = 0; q <= n-1; q++){
    while(z[k+1] < q) k++;
    d[q] = (q-v[k])*(q-v[k]) + f[v[k]];
  }
}

static void at_arrayu16_dt_2D(AtArrayU16* array){
  uint64_t width   = array->h.shape[1];
  uint64_t height  = array->h.shape[0];
  uint64_t nelem   = array->h.nelem;
  uint64_t maxsize = max(width, height);
  uint64_t x, y, offset;
  uint8_t *data    = malloc((maxsize << 4) + ((maxsize + 1) << 2));
  uint64_t *v      = (uint64_t*) data;
  float    *f      = (float*)&v[maxsize];
  float    *d      = &f[maxsize];
  float    *z      = &d[maxsize];

  // Transform along columns
  for(x = 0; x < width; x++){
    for(y = 0, offset = x; y < height; y++, offset += width)
      f[y] = array->data[offset];
    at_float_dt_1D(f, height, d, v, z);
    for(y = 0, offset = x; y < height; y++, offset += width)
      array->data[offset] = d[y];
  }

  // Transform along rows
  uint16_t *doffset;
  for(y = 0, doffset = array->data; y < height; y++, doffset = &doffset[width]){
    for(x = 0; x < width; x++) f[x] = doffset[x];
    at_float_dt_1D(f, width, d, v, z);
    for(x = 0; x < width; x++) doffset[x] = d[x];
  }

  free(data);
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
void
at_arrayu8_distance_transform(AtArrayU8* array, uint8_t on, AtArrayU16* out){
  uint64_t i;
  for(i = 0; i < array->h.nelem; i++){
    if(array->data[i] == on) out->data[i] = 0;
    else                     out->data[i] = UINT16_MAX;
  }
  at_arrayu16_dt_2D(out);
}
