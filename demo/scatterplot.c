/**
 ** This file is part of the atkv project.
 ** Copyright 2016-2017 Anderson Tavares <nocturne.pe@gmail.com>.
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
  plot2->l.name      = "Scatter Plot with Line";
  sc2->legend        = true;
  sc2->legendspos    = AT_BOTTOM;
  chart->title       = "Scatter Plot";
  at_chart_show("Chart",chart);
  return 0;
}
