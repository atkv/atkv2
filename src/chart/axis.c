#include <at/chart.h>
#include <math.h>

static void
at_axis_init(AtAxis* axis){
  axis->vmin = INFINITY;
  axis->vmax = -INFINITY;
}

AtAxis*
at_axis_new(uint8_t num){
  AtAxis* axis = malloc(sizeof(AtAxis)*2);
  uint8_t i;
  for(i = 0; i < num; i++)
    at_axis_init(&axis[i]);
  return axis;
}

void
at_axis_fill(AtAxis* axis, uint8_t naxis, double** valuesp, uint64_t* nump){
  double* values;
  uint64_t i, k,num;
  for(i = 0; i < naxis; i++, axis++, valuesp++, nump++){
    values = *valuesp;
    num    = *nump;
    for(k = 0; k < num; k++){
      axis->vmin = min(axis->vmin,values[k]);
      axis->vmax = max(axis->vmax,values[k]);
    }
  }
}
