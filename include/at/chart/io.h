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

#ifndef AT_CHART_IO_H
#define AT_CHART_IO_H
#include <at/chart.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
AT_BEGIN_DECLS
/**
 * @brief at_chart_write_pdf
 * @param chart
 * @param filename
 * @param width
 * @param height
 */
void
at_chart_write_pdf(AtChart *chart, const char *filename, double width, double height);
/**
 * @brief at_chart_write_png
 * @param chart
 * @param filename
 * @param width
 * @param height
 */
void
at_chart_write_png(AtChart *chart, const char *filename, double width, double height);
/**
 * @brief at_chart_write_svg
 * @param chart
 * @param filename
 * @param width
 * @param height
 */
void
at_chart_write_svg(AtChart *chart, const char *filename, double width, double height);
/**
 * @brief at_chart_write
 * @param chart
 * @param filename
 * @param width
 * @param height
 */
void
at_chart_write(AtChart* chart, const char* filename, double width, double height);

AT_END_DECLS
#endif
