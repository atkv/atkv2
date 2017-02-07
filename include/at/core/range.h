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
#ifndef AT_RANGE_H
#define AT_RANGE_H
#include <at/core.h>
AT_BEGIN_DECLS

/*=============================================================================
 STRUCTURE
 ============================================================================*/
typedef struct _AtRange{
  int16_t from;
  int16_t to;
}AtRange;
extern const AtRange AT_RANGE_ALL;
/*=============================================================================
 PUBLIC API
 ============================================================================*/
#define at_range_from_to(from,to) ((AtRange){from,to})
#define at_range_to(to)           ((AtRange){0,to})
#define at_range_from(to)         ((AtRange){from,INT16_MAX})
#define at_range_at(at)           ((AtRange){at,at+1})
AT_END_DECLS
#endif
