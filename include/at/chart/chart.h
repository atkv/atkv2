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

#ifndef AT_CHART_CHART_H
#define AT_CHART_CHART_H

#include <at/chart.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct AtChart{
  const char * title;
  AtSList    * subcharts;
  uint8_t      gridsize[2];
  uint8_t      nsubs;
}AtChart;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_chart_new
 * @return
 */
AtChart*
at_chart_new();

///**
// * @brief at_chart_plot
// * @param chart
// * @param values
// * @param num
// * @return
// */
//AtLinePlot*
//at_chart_plot_u8(AtChart* chart, uint8_t* values, uint64_t num);

AtSubchart*
at_chart_add(AtChart* chart);
/**
 * @brief Creates a line plot in the first subchart
 * @param chart
 * @param values
 * @param num
 * @return
 */
AtLinePlot*
at_chart_plot_d64(AtChart* chart, double* values, uint64_t num);

/**
 * @brief Creates an scatter plot in the first subchart
 * @param chart
 * @param x
 * @param y
 * @param num
 * @return
 */
AtScatterPlot*
at_chart_scatter_d64(AtChart* chart, double* x, double* y, uint64_t num);

///**
// * @brief at_chart_write_pdf
// * @param chart
// * @param filename
// */
//void
//at_chart_write_pdf(AtChart* chart, const char* filename, double width, double height);

/**
 * @brief at_chart_destroy
 * @param chart
 */
void
at_chart_destroy(AtChart** chart);
AT_END_DECLS
#endif
