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
#include <at/gui.h>
#include <math.h>

int
main(int argc, char** argv){
  AtChart      * chart;
  double         data[100];
  uint64_t       i;

  // Fill data
  for(i = 0; i < 100; i++) data[i] = i*i+1;

  // Creating our chart
  chart = at_chart_new();
  at_chart_plot_d64(chart,data,100);
  chart->title = "Line Plot";
  at_chart_show("chart", chart);

  return 0;
}
