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

void
at_scatterplot_init(AtScatterPlot* plot){
  at_lineplot_init(&plot->l);
  plot->x = NULL;
  plot->l.type = AT_PLOT_SCATTER;
}

AtScatterPlot*
at_scatterplot_new(){
  AtScatterPlot* plot = malloc(sizeof(AtScatterPlot));
  at_scatterplot_init(plot);
}

void
at_scatterplot_fill_d64(AtScatterPlot* sp, double* x, double* y, uint64_t num){
  at_lineplot_fill_d64(&sp->l,y,num);
  if(!sp->x) sp->x = malloc(num<<3);
  uint64_t i;
  for(i = 0; i < num;i++)
    sp->x[i] = x[i];
}
