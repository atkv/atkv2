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

#ifndef AT_CHART_TYPE_H
#define AT_CHART_TYPE_H

#include <at/core.h>

typedef enum{
  AT_MARKER_POINT,
  AT_MARKER_CIRCLE,
  AT_MARKER_TRIANGLE_DOWN,
  AT_MARKER_TRIANGLE_UP,
  AT_MARKER_TRIANGLE_LEFT,
  AT_MARKER_TRIANGLE_RIGHT,
  AT_MARKER_OCTAGON,
  AT_MARKER_SQUARE,
  AT_MARKER_PENTAGON,
  AT_MARKER_STAR,
  AT_MARKER_HEXAGON1,
  AT_MARKER_HEXAGON2,
  AT_MARKER_PLUS,
  AT_MARKER_X,
  AT_MARKER_DIAMOND,
  AT_MARKER_VLINE,
  AT_MARKER_HLINE,
  AT_MARKER_NONE
}AtMarkerType;

typedef enum{
  AT_LINESTYLE_SOLID,
  AT_LINESTYLE_DASHED,
  AT_LINESTYLE_DASHDOT,
  AT_LINESTYLE_DOTTED,
  AT_LINESTYLE_NONE
}AtLineStyle;

typedef AtVec4D64 AtColor;

typedef enum{
  AT_PLOT_LINE,
  AT_PLOT_SCATTER,
  AT_PLOT_BAR,
  AT_PLOT_PIE,
}AtSeriesType;

typedef enum{
  AT_LABEL_CENTER,
  AT_LABEL_INSIDE_END,
  AT_LABEL_INSIDE_BASE,
  AT_LABEL_OUTSIDE_END
}AtLabelPosition;

#endif
