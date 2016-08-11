#include <at/chart.h>
#include <math.h>
#define NUM 100
int main(int argc, char* argv[]){
  double data[NUM];
  for(i = 0; i < NUM; i++) data[i] = sin(i/2.0);
  AtSeriesScatter* series = at_series_scatter_new();
  at_series_scatter_fill_1D(series,data,NUM);
  AtChart* chart = at_chart_new();
  at_chart_add_series(chart, series);
  at_chart_default_axis(chart);
  AtAxisBase* axis_x = at_chart_get_axis_x();
  AtAxisBase* axis_y = at_chart_get_axis_y();
  axis_x->title = "Title X";
  axis_y->title = "Title Y";
  at_chart_show(chart,"Line Chart");
}
