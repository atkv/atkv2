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
#ifndef AT_SCC_H
#define AT_SCC_H

#include <at/core/grapharray.h>
#include <at/core/type.h>
AT_BEGIN_DECLS
typedef struct{
  uint32_t *l;    // 00+8
  uint32_t  n;    // 08+4
  uint8_t   pd[4];// 12+4
}AtSCC;           // 16

AtSCC*
at_grapharrayu8_scc(AtGraphArray* grapharray, AtSCCAlgorithm algorithm);
AT_END_DECLS
#endif
