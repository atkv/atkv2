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

#ifndef AT_CHART_SUBCHART_H
#define AT_CHART_SUBCHART_H

#include <at/chart.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct AtSubchart{
  AtAxis    * axis;
  AtSList   * plotlist;
  const char* title;
  uint8_t     nplots;
}AtSubchart;

#define AT_DECLARE_SUBCHART_PLOT(lower, UPPER, type) \
AtLinePlot* at_subchart_plot_##lower(AtSubchart* sc, type* values, uint64_t num);

AT_DECLARE_SUBCHART_PLOT(u8 , U8 , uint8_t)
AT_DECLARE_SUBCHART_PLOT(u16, U16, uint16_t)
AT_DECLARE_SUBCHART_PLOT(u32, U32, uint32_t)
AT_DECLARE_SUBCHART_PLOT(u64, U64, uint64_t)
AT_DECLARE_SUBCHART_PLOT(i8 , I8 , int8_t)
AT_DECLARE_SUBCHART_PLOT(i16, I16, int16_t)
AT_DECLARE_SUBCHART_PLOT(i32, I32, int32_t)
AT_DECLARE_SUBCHART_PLOT(i64, I64, int64_t)
AT_DECLARE_SUBCHART_PLOT(f32, F32, float)
AT_DECLARE_SUBCHART_PLOT(d64, D64, double)


AtSubchart*
at_subchart_new();

void
at_subchart_destroy(AtSubchart** scp);

AT_END_DECLS
#endif
