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


#include<at/chart.h>
static void
at_pieplot_init(AtPiePlot* pp){
  at_lineplot_init(&pp->l);
  pp->l.type      = AT_SERIES_PIE;
  pp->colors      = NULL;
}

AtPiePlot*
at_pieplot_new(){
  AtPiePlot* bp = malloc(sizeof(AtPiePlot));
  at_pieplot_init(bp);
}
void
at_pieplot_fill_d64(AtPiePlot* pieplot, char** categories, double* values, uint64_t num){
  at_lineplot_fill_d64(&pieplot->l,values,num);
  pieplot->categories = categories;
  pieplot->colors = malloc(sizeof(AtColor)*num);
  uint64_t i;
  for(i = 0; i < num; i++){
    pieplot->colors[i].x = (rand()%100)/100.0;
    pieplot->colors[i].g = (rand()%100)/100.0;
    pieplot->colors[i].b = (rand()%100)/100.0;
    pieplot->colors[i].a = 1;
  }
}
