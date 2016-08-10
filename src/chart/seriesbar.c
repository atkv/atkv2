#include <at/chart.h>

AtSeriesBar*
at_series_bar_new(){
  AtSeriesBar *series = malloc(sizeof(AtSeriesBar));
  return series;
}

void
at_series_bar_append(AtSeriesBar* series, AtBarSet* barset){

}

void
at_series_bar_destroy(AtSeriesBar** seriesp){

}
