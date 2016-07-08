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
#ifndef AT_DRAWING_H
#define AT_DRAWING_H

#include <at/core/array.h>
#include <at/core/vec.h>
#include <at/core/grapharray.h>
AT_BEGIN_DECLS

/**
 * @brief at_arrayu8_draw_line
 * @param array
 * @param p0
 * @param p1
 * @param color
 * @param thickness
 * @param adjacency
 * @param shift
 */
void
at_arrayu8_draw_line(AtArrayU8*  array,
                     AtVec2I16 p0,
                     AtVec2I16 p1,
                     AtVec4U8    color,
                     uint8_t     thickness,
                     AtAdjacency adjacency,
                     uint8_t     shift);

/**
 * @brief at_arrayu8_clip_line
 * @param size
 * @param p0
 * @param p1
 */
void
at_arrayu8_clip_line(AtVec2U16   size,
                     AtVec2U16   p0,
                     AtVec2U16   p1);
/**
 * @brief at_arrayu8_draw_circle
 * @param array
 * @param center
 * @param radius
 * @param color
 * @param thickness
 * @param adjacency
 * @param shift
 */
void
at_arrayu8_draw_circle(AtArrayU8*  array,
                       AtVec2I16   center,
                       uint16_t    radius,
                       AtVec4U8    color,
                       uint8_t     thickness,
                       AtAdjacency adjacency,
                       uint8_t     shift);

/**
 * @brief at_arrayu8_draw_rectangle
 * @param array
 * @param rect
 * @param color
 * @param thickness
 * @param adjacency
 * @param shift
 */
void
at_arrayu8_draw_rectangle(AtArrayU8*  array,
                          AtVec4U16   rect,
                          AtVec4U8    color,
                          uint8_t     thickness,
                          AtAdjacency adjacency,
                          uint8_t     shift);

/**
 * @brief at_ellipse_to_poly
 * @param center
 * @param axes
 * @param angle
 * @param arc_start
 * @param arc_end
 * @param delta
 * @param pts
 * @return
 */
uint16_t
at_ellipse_to_poly(AtVec2I16 center,
                   AtVec2I16 axes,
                   uint16_t angle,
                   uint16_t arc_start,
                   uint16_t arc_end,
                   uint8_t delta,
                   AtVec4U16** pts);



AT_END_DECLS

#endif
