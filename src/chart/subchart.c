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
  at_axis_fill(&sc->axis[1],lineplot->values,num);
  sc->axis[0].vmin = 0;
  sc->axis[0].vmax = num;
  sc->nplots++;
  return lineplot;
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
