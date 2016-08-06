#include <at/chart.h>

static void
at_lineplot_init(AtLinePlot* lp){
  AtColor edgecolor   = {0,0,0,1};
  lp->linestyle       = AT_LINESTYLE_SOLID;
  lp->linewidth       = 1;
  lp->marker          = AT_MARKER_POINT;
  lp->markeredgecolor = edgecolor;
  lp->linecolor       = edgecolor;
  lp->markeredgewidth = 1;
  lp->markerfacecolor = edgecolor;
  lp->markersize      = 1;
  lp->nelem           = 0;
  lp->values          = NULL;
}

AtLinePlot*
at_lineplot_new(){
  AtLinePlot* plot = malloc(sizeof(AtLinePlot));
  at_lineplot_init(plot);
  return plot;
}

void
at_lineplot_destroy(AtLinePlot** selfp){
  if(*selfp){
    if((*selfp)->values) free((*selfp)->values);
    free(*selfp);
    *selfp = NULL;
  }
}
