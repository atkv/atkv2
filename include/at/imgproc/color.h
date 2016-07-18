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
#ifndef AT_COLOR_H
#define AT_COLOR_H

#include <at/core/array.h>
AT_BEGIN_DECLS

typedef enum{
  AT_GRAY,
  AT_RGB,
  AT_BGR,
  AT_RGBA,
  AT_BGRA,
  AT_ARGB,
  AT_ABGR
}AtColorType;


AtArrayU8*
at_arrayu8_cvt_color(AtArrayU8* array, AtColorType from, AtColorType to);
/**
 * @brief at_arrayu8_lut_random
 * @param array
 * @return
 */
AtArrayU8*
at_arrayu8_lut_random(AtArrayU8* array);
/**
 * @brief at_arrayu8_lut_random_multi
 * @param array
 * @param nchannels
 * @return
 */
AtArrayU8*
at_arrayu8_lut_random_multi(AtArrayU8* array, uint8_t nchannels);

AT_END_DECLS
#endif
