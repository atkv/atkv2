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
  uint8_t *data    = malloc((maxsize << 4) + (maxsize << 2) + 1);
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

///* dt of 1d function using squared distance */
//static float *dt(float *f, int n) {
//  float *d = new float[n];
//  int *v = new int[n];
//  float *z = new float[n+1];
//  int k = 0;
//  v[0] = 0;
//  z[0] = -INF;
//  z[1] = +INF;
//  for (int q = 1; q <= n-1; q++) {
//    float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
//    while (s <= z[k]) {
//      k--;
//      s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
//    }
//    k++;
//    v[k] = q;
//    z[k] = s;
//    z[k+1] = +INF;
//  }

//  k = 0;
//  for (int q = 0; q <= n-1; q++) {
//    while (z[k+1] < q)
//      k++;
//    d[q] = square(q-v[k]) + f[v[k]];
//  }

//  delete [] v;
//  delete [] z;
//  return d;
//}

///* dt of 2d function using squared distance */
//static void dt(image<float> *im) {
//  int width = im->width();
//  int height = im->height();
//  float *f = new float[std::max(width,height)];

//  // transform along columns
//  for (int x = 0; x < width; x++) {
//    for (int y = 0; y < height; y++) {
//      f[y] = imRef(im, x, y);
//    }
//    float *d = dt(f, height);
//    for (int y = 0; y < height; y++) {
//      imRef(im, x, y) = d[y];
//    }
//    delete [] d;
//  }

//  // transform along rows
//  for (int y = 0; y < height; y++) {
//    for (int x = 0; x < width; x++) {
//      f[x] = imRef(im, x, y);
//    }
//    float *d = dt(f, width);
//    for (int x = 0; x < width; x++) {
//      imRef(im, x, y) = d[x];
//    }
//    delete [] d;
//  }

//  delete f;
//}


///* dt of binary image using squared distance */
//static image<float> *dt(image<uchar> *im, uchar on = 1) {
//  int width = im->width();
//  int height = im->height();

//  image<float> *out = new image<float>(width, height, false);
//  for (int y = 0; y < height; y++) {
//    for (int x = 0; x < width; x++) {
//      if (imRef(im, x, y) == on)
//	imRef(out, x, y) = 0;
//      else
//	imRef(out, x, y) = INF;
//    }
//  }

//  dt(out);
//  return out;
//}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtArrayU16*
at_arrayu8_distance_transform(AtArrayU8* array, uint8_t on){
  AtArrayU16* out = at_arrayu16_zeros(array->h.dim, array->h.shape);
  uint64_t i;
  for(i = 0; i < array->h.nelem; i++){
    if(array->data[i] == on) out->data[i] = 0;
    else                     out->data[i] = UINT16_MAX;
  }
  at_arrayu16_dt_2D(out);
  return out;
}
