#include <at/chart.h>
#define NUM 100
int main(int argc, char* argv[]){
  double data[NUM];
  AtSeriesScatter* series = at_series_scatter_new();
  at_series_scatter_fill_1D(series,data,NUM);
  AtChart* chart = at_chart_new();
  at_chart_add_series(chart, series);
  at_chart_default_axis();
  at_chart_show("name",chart);
}
