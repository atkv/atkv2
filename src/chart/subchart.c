#include <at/chart.h>

static void
at_subchart_init(AtSubchart* sc){
  memset(sc,0,sizeof(AtSubchart));
}

AtLinePlot*
at_subchart_plot_d64(AtSubchart* sc, double* values, uint64_t num){
  if(sc->axis == NULL) sc->axis = at_axis_new(2);
  AtLinePlot* lineplot = at_lineplot_new();
  sc->plotlist = at_slist_append(sc->plotlist,lineplot);
  at_lineplot_fill_d64(lineplot,values,num);
  double   ax[2]      = {0,num};
  double*  valuesp[2] = {ax,lineplot->y};
  uint64_t naxis[2]  = {2,num};
  at_axis_fill(sc->axis,2,valuesp,naxis);
  sc->nplots++;
  return lineplot;
}
AtBarPlot*
at_subchart_bar_d64(AtSubchart* sc, double* values, uint64_t num){
  if(sc->axis == NULL) sc->axis = at_axis_new(2);
  AtBarPlot* barplot = at_barplot_new();
  sc->plotlist = at_slist_append(sc->plotlist,barplot);
  at_lineplot_fill_d64(&barplot->l,values,num);
  double   ax[2]      = {0,num};
  double*  valuesp[2] = {ax,barplot->l.y};
  uint64_t naxis[2]  = {2,num};
  at_axis_fill(sc->axis,2,valuesp,naxis);
  sc->nplots++;
  return barplot;
}
AtPiePlot*
at_subchart_pie_d64(AtSubchart* sc, char** categories, double* values, uint64_t num){
  if(sc->axis == NULL) sc->axis = at_axis_new(2);
  AtPiePlot* pieplot = at_pieplot_new();
  sc->plotlist = at_slist_append(sc->plotlist,pieplot);
  at_pieplot_fill_d64(pieplot,categories,values,num);
  pieplot->categories = categories;
  sc->nplots++;
  return pieplot;
}

AtScatterPlot*
at_subchart_scatter_d64(AtSubchart *sc, double *x, double *y, uint64_t num){
  if(sc->axis == NULL) sc->axis = at_axis_new(2);
  AtScatterPlot* scatterplot = at_scatterplot_new();
  sc->plotlist = at_slist_append(sc->plotlist,scatterplot);
  at_scatterplot_fill_d64(scatterplot, x, y, num);
  double*  valuesp[2] = {scatterplot->x,scatterplot->l.y};
  uint64_t naxis[2]   = {num,num};
  at_axis_fill(sc->axis,2,valuesp,naxis);
  sc->nplots++;
  return scatterplot;
}

AtSubchart*
at_subchart_new(){
  AtSubchart* sc = malloc(sizeof(AtSubchart));
  at_subchart_init(sc);
  return sc;
}

void
at_subchart_destroy(AtSubchart** scp){
  if(*scp){
    AtSList* list = (*scp)->plotlist;
    while(list){
      AtLinePlot* lineplot = (AtLinePlot*)list->value;
      at_lineplot_destroy(&lineplot);
      list = list->next;
    }
    at_slist_free(list);
    free(*scp);
    *scp = NULL;
  }
}
