#include <at/chart.h>
#include <math.h>
int main(int argc, char** argv){
  double  data[100],data2[100];
  AtColor red  = {1,0,0,1};
  AtColor blue = {0,0,1,1};
  uint8_t i;
  for(i = 0; i < 100; i++){ 
    data[i]  = sin(i/10.0);
    data2[i] = cos(i/10.0);
  }
  AtChart   * chart  = at_chart_new();
  chart->title       = "Chart Title";
  chart->gridsize[0] = 2;
  AtSubchart* sc2    = at_chart_add(chart);
  AtSubchart* sc3    = at_chart_add(chart);
  AtSubchart* sc     = (AtSubchart*)chart->subcharts->value;
  AtLinePlot* plot   = at_subchart_plot_d64(sc,data,100);
  AtLinePlot* plot2  = at_subchart_plot_d64(sc2,data2,100);
  AtLinePlot* plot3  = at_subchart_plot_d64(sc,data2,100);
  AtLinePlot* plot4  = at_subchart_plot_d64(sc3,data2,100);
  sc->legend         = true;
  sc->legendin       = true;
  sc->legendspos     = AT_BOTTOM;
  sc->title          = "A subchart";
  sc3->title         = "Another subchart";
  plot->marker       = AT_MARKER_SQUARE;
  plot->linestyle    = AT_LINESTYLE_SOLID;
  plot->linecolor    = red;
  plot->linewidth    = 2;
  plot->name         = "Plot1";
  plot2->marker      = AT_MARKER_NONE;
  plot2->linestyle   = AT_LINESTYLE_DASHED;
  plot2->linecolor   = blue;
  plot2->linewidth   = 1;
  plot2->name         = "Plot2alsdf";
  plot3->name         = "Plot3xx";
  plot4->name         = "Plot4++++++";
  at_chart_show("plot",chart);
  return 0;
}
