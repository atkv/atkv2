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


#include<at/chart.h>
static void
at_barplot_init(AtBarPlot* bp){
  at_lineplot_init(&bp->l);
  bp->l.type      = AT_PLOT_BAR;
  bp->fillcolor.r = 0.6;
  bp->fillcolor.g = 0;
  bp->fillcolor.b = 0;
  bp->fillcolor.a = 1;
}

AtBarPlot*
at_barplot_new(){
  AtBarPlot* bp = malloc(sizeof(AtBarPlot));
  at_barplot_init(bp);
}
