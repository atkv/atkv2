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
#if !defined(AT_CORE_H_INSIDE)
#error "Only <at/core.h> can be included directly."
#endif
#ifndef AT_OPTIMIZATION_H
#define AT_OPTIMIZATION_H
#include <at/core.h>
AT_BEGIN_DECLS
/*=============================================================================
 PUBLIC API
 ============================================================================*/

typedef enum{
  AT_MAXIMIZATION=0,
  AT_MINIMIZATION=1
}AtOptimization;

typedef enum{
  AT_FIFO=0,
  AT_LIFO=1
}AtPolicy;
AT_END_DECLS
#endif
