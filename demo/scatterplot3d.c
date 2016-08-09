#include <at/chart.h>

int main(){
  double x[100], y[100], z[100];
  AtChart* chart = at_chart_new();
  at_chart_scatter_d64(x,y,z);
  at_chart_show(chart);
  return 0;
}
