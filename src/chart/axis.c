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
at_axis_fill(AtAxis* axis, double* values, uint64_t num){
  uint64_t i;
  for(i = 0; i < num;i++){
    axis->vmin = min(axis->vmin,values[i]);
    axis->vmax = max(axis->vmax,values[i]);
  }
}
