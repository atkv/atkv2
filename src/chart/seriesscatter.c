#include <at/chart.h>

AtSeriesScatter*
at_series_scatter_new(){
  return malloc(sizeof(AtSeriesScatter));
}

void
at_series_scatter_fill_1D(AtSeriesScatter *ss, double *y, uint32_t num){
  ss->y   = y;
  ss->num = num;
  for(i = 0; i < num; i++)
    ss->x[i] = i;
}

void
at_series_scatter_fill_2D(AtSeriesScatter *ss, double *x, double *y, uint32_t num){
  ss->x = x;
  ss->y = y;
  ss->num = num;
}

void
at_series_scatter_destroy(AtSeriesScatter **seriesp){
  free(*seriesp);
  *seriesp = NULL;
}


