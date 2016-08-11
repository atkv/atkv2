/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <at/chart.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <math.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
static void
at_chart_init(AtChart* chart){
  AtSubchart* subchart = at_subchart_new();
  chart->subcharts     = at_slist_append(NULL,subchart);
  chart->nsubs         = 1;
  chart->gridsize[0]   = 1;
  chart->gridsize[1]   = 1;
  chart->title         = NULL;
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtChart*
at_chart_new(){
  AtChart* chart = malloc(sizeof(AtChart));
  at_chart_init(chart);
  return chart;
}

AtSubchart*
at_chart_add(AtChart* chart){
  AtSubchart* subchart = at_subchart_new();
  chart->subcharts     = at_slist_append(chart->subcharts,subchart);
  chart->nsubs++;
  if(chart->gridsize[0]*chart->gridsize[1] < chart->nsubs)
    chart->gridsize[1]++;
  return subchart;
}

AtLinePlot*
at_chart_plot_d64(AtChart* chart, double* values, uint64_t num){
  return at_subchart_plot_d64((AtSubchart*)chart->subcharts->value,values,num);
}

AtBarPlot*
at_chart_bar_d64(AtChart* chart, double* values, uint64_t num){
  return at_subchart_bar_d64((AtSubchart*)chart->subcharts->value,values,num);
}

AtPiePlot*
at_chart_pie_d64(AtChart* chart, char** categories, double* values, uint64_t num){
  return at_subchart_pie_d64((AtSubchart*)chart->subcharts->value,categories,values,num);
}

AtScatterPlot*
at_chart_scatter_d64(AtChart* chart, double* x, double* y, uint64_t num){
  return at_subchart_scatter_d64((AtSubchart*)chart->subcharts->value,x,y,num);
}

void
at_chart_destroy(AtChart** chart){
  if(*chart){
    AtSList* list = (*chart)->subcharts;
    while(list){
      AtSubchart* subchart = (AtSubchart*)list->value;
      at_subchart_destroy(&subchart);
      list = list->next;
    }
    at_slist_free(list);
    free(*chart);
    *chart = NULL;
  }
}


void
at_chart_add_series(AtChart* chart, AtSeriesBase* series){
  at_slist_append(chart->series);
}
