#include<at/chart.h>
static void
at_barplot_init(AtBarPlot* bp){
  at_lineplot_init(&bp->l);
  bp->l.type      = AT_PLOT_BAR;
  bp->fillcolor.r = 0.6;
  bp->fillcolor.g = 0;
  bp->fillcolor.b = 0;
  bp->fillcolor.a = 1;
}

AtBarPlot*
at_barplot_new(){
  AtBarPlot* bp = malloc(sizeof(AtBarPlot));
  at_barplot_init(bp);
}
