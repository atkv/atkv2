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

#if !defined(AT_CHART_H_INSIDE)
#error "Only <at/chart.h> can be included directly."
#endif

#ifndef AT_CHART_LINEPLOT_H
#define AT_CHART_LINEPLOT_H

#include <at/chart.h>
AT_BEGIN_DECLS

typedef struct AtLinePlot{
  double*      y;
  const char*  name;
  uint64_t     nelem;
  double       linewidth;
  double       markersize;
  double       markeredgewidth;
  AtColor      markeredgecolor;
  AtColor      markerfacecolor;
  AtColor      linecolor;
  AtMarkerType marker;
  AtLineStyle  linestyle;
  AtSeriesType type;
}AtLinePlot;

/**
 * @brief at_lineplot_new
 * @return
 */
AtLinePlot*
at_lineplot_new();

void
at_lineplot_init(AtLinePlot* lp);

/**
 * @brief at_lineplot_fill_u8
 * @param lineplot
 * @param values
 * @param num
 */
void
at_lineplot_fill_u8(AtLinePlot* lineplot, uint8_t *values, uint64_t num);
/**
 * @brief at_lineplot_fill_d64
 * @param lineplot
 * @param values
 * @param num
 */
void
at_lineplot_fill_d64(AtLinePlot* lineplot, double *values, uint64_t num);

/**
 * @brief at_lineplot_destroy
 * @param selfp
 */
void
at_lineplot_destroy(AtLinePlot** selfp);

AT_END_DECLS

#endif
