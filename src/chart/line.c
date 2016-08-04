#include <at/chart.h>

AtLinePlot*
at_lineplot_new(){
  return malloc(sizeof(AtLinePlot));
}

void
at_lineplot_destroy(AtLinePlot** selfp){
  if(*selfp){
    if((*selfp)->values) free((*selfp)->values);
    free(*selfp);
    *selfp = NULL;
  }
}
