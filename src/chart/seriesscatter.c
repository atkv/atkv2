#include <at/chart.h>

void
at_series_scatter_init(AtSeriesScatter* ss){
  at_series_base_init(ss->base);
  at_linecfg_init(ss->linecfg);
  at_markercfg_init(ss->markercfg);
  ss->num = 0;
  ss->x   = NULL;
  ss->y   = NULL;
}

AtSeriesScatter*
at_series_scatter_new(){
  return malloc(sizeof(AtSeriesScatter));
}

void
at_series_scatter_fill_1D(AtSeriesScatter *ss, double *y, uint32_t num){
  uint32_t i;
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


