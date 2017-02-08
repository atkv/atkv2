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
#if !defined(AT_IMGPROC_H_INSIDE)
#error "Only <at/imgproc.h> can be included directly."
#endif
#ifndef AT_ROBOTUSER_H
#define AT_ROBOTUSER_H
#include <at/core.h>
#include <at/imgproc.h>
AT_BEGIN_DECLS

typedef struct AtRobotUserU8 {
  AtArrayU16  ** dt;
  AtArrayU8    * array;
  AtArrayU8    * mask;
  AtArrayU8    * diff;
  AtSeeds      * seeds;
  AtIFT        * ift;
  AtGraphArray * g;
  uint64_t       step;
  uint64_t       curseed;
  AtPolicy       policy;
  AtConnectivity conn;
  uint8_t        nclasses;
  uint8_t        finished;
}AtRobotUserU8;

AtRobotUserU8*
at_robotuseru8_new_from_array(AtArrayU8* array, AtArrayU8 *mask, uint64_t maxseeds, AtAdjacency adjacency, AtWeighting weighting, AtPolicy policy, AtConnectivity conn);

void
at_robotuseru8_next(AtRobotUserU8* robotuser);

void
at_robotuseru8_destroy(AtRobotUserU8** robotuserp);

AT_END_DECLS
#endif
