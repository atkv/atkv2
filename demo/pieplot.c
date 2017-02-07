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
#include <at/gui.h>
#include <math.h>

#define NUM 9

int
main(int argc, char** argv){
  AtChart      * chart;
  double         data[NUM];
  char         * categories[NUM] = {"IME","POLI","FEA","FFLHC","ECA","FAU","IB","EACH","CEPE"};
  uint64_t       i;

  // Fill data
  for(i = 0; i < NUM; i++) data[i] = (rand() % 10000)/100.0;

  // Creating our chart
  chart = at_chart_new();
  AtPiePlot*  pp = at_chart_pie_d64(chart,categories,data,NUM);
  chart->title   = "Pie Chart";
  AtSubchart* sc = (AtSubchart*)chart->subcharts->value;
  sc->legendspos = AT_RIGHT;
  sc->legend     = true;
  at_chart_show("chart", chart);

  return 0;
}

