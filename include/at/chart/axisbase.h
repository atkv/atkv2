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

#ifndef AT_AXISBASE_H
#define AT_AXISBASE_H

#include <at/chart.h>

AT_BEGIN_DECLS
/*=============================================================================
 STRUCTURE
 ============================================================================*/

typedef enum{
  AT_AXIS_TOP,
  AT_AXIS_BOTTOM,
  AT_AXIS_LEFT,
  AT_AXIS_RIGHT
}AtAxisAlignment;

typedef enum{
  AT_CAP_FLAT,
  AT_CAP_SQUARE,
  AT_CAP_ROUND
}AtPenCapStyle;

typedef enum{
  AT_JOIN_MITER,
  AT_JOIN_BEVEL,
  AT_JOIN_ROUND,
}AtPenJoinStyle;

typedef struct AtPen{
  double*        dashpattern;
  double         dashoffset;
  AtColor        color;
  AtPenCapStyle  capstyle;
  AtPenJoinStyle joinstyle;

}AtPen;

typedef struct AtLabelConfig{

}AtLabelConfig;

typedef struct AtShadesConfig{

}AtShadesConfig;

typedef struct AtOrientation{

}AtOrientation;


typedef struct AtAxisBase{
  AtAxisAlignment alignment;
  AtColor         color;
  AtPen           gridline;
  AtPen           minorgridline;
  AtLabelConfig   labels;
  AtShadesConfig  shades;
  AtOrientation   orientation;
  char*           title;
  bool            reverse;
  bool            visible;
}AtAxisBase;

AT_END_DECLS
#endif
