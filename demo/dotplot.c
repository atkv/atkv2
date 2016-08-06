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
  AtLinePlot* plot   = at_chart_plot_d64(chart,data,100);
  AtLinePlot* plot2  = at_chart_plot_d64(chart,data2,100);
  plot->marker       = AT_MARKER_SQUARE;
  plot->linestyle    = AT_LINESTYLE_SOLID;
  plot->linecolor    = red;
  plot->linewidth    = 2;
  plot2->marker      = AT_MARKER_NONE;
  plot2->linestyle   = AT_LINESTYLE_DASHED;
  plot2->linecolor   = blue;
  plot2->linewidth   = 1;
  at_chart_show("plot",chart);
  return 0;
}
