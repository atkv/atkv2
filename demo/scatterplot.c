#include <at/chart.h>

int main(){
  double x[100],y[100];
  uint64_t i;
  for(i = 0; i < 100; i++){
    x[i] = (rand()%10000)/100.0 - 50;
    y[i] = (rand()%10000)/100.0 - 50;
  }
  AtChart      * chart = at_chart_new();
  AtScatterPlot* plot  = at_chart_scatter_d64(chart,x,y,100);
  AtSubchart   * sc2   = at_chart_add(chart);
  AtColor        red   = {1,0,0,1};
  AtColor        blue  = {0,0,1,1};
  AtScatterPlot* plot2 = at_subchart_scatter_d64(sc2,x,y,100);
  plot->l.linecolor  = red;
  plot->l.linestyle  = AT_LINESTYLE_NONE;
  plot2->l.linecolor = blue;
  plot2->l.marker    = AT_MARKER_SQUARE;
  plot2->l.name      = "Chart";
  sc2->legend        = true;
  sc2->legendspos    = AT_BOTTOM;
  chart->title       = "Chart";
  at_chart_show("Chart",chart);
  return 0;
}
