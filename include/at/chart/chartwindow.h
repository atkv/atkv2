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

#ifndef AT_CHARTWINDOW_H
#define AT_CHARTWINDOW_H
#include <at/chart.h>
#include <gtk/gtk.h>
AT_BEGIN_DECLS
/*=============================================================================
 STRUCTURE
 ============================================================================*/

#define AT_TYPE_CHARTWINDOW at_chartwindow_get_type()
G_DECLARE_DERIVABLE_TYPE(AtChartWindow, at_chartwindow, AT, CHARTWINDOW, GtkWindow)

typedef struct _AtChartWindowClass{
  GtkWindowClass parent_class;
}AtChartWindowClass;

/*=============================================================================
 PUBLIC API
 ============================================================================*/
/**
 * @brief at_chartwindow_new
 * @return
 */
AtChartWindow*
at_chartwindow_new();

/**
 * @brief at_chartwindow_set
 * @param window
 * @param chart
 */
void
at_chartwindow_set(AtChartWindow* window, AtChart* chart);

/**
 * @brief High level function to show a chart in a named window
 * @param name
 * @param chart
 */
AtChartWindow*
at_chart_show(const char* name, AtChart* chart);
AT_END_DECLS
#endif
