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

#if !defined(AT_CHART_H_INSIDE)
#error "Only <at/chart.h> can be included directly."
#endif

#ifndef AT_CHART_PIEPLOT_H
#define AT_CHART_PIEPLOT_H

#include <at/chart.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct _AtPiePlot{
  AtLinePlot   l;
  char      ** categories;
  AtColor    * colors;
}AtPiePlot;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtPiePlot*
at_pieplot_new();

void
at_pieplot_fill_d64(AtPiePlot* pieplot, char** categories, double* values, uint64_t num);

AT_END_DECLS
#endif
