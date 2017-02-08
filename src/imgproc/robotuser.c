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
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <at/imgproc.h>

/*=============================================================================
 PRIVATE API
 ============================================================================*/
static void
add_seeds(AtRobotUserU8* robotuser, uint8_t dtindex){
  // Descobrir a maior
  AtArrayU16* dt = robotuser->dt[dtindex];
  uint64_t maxindex;
  uint64_t i;
  uint16_t maxvalue = 0;
  for(i = 0; i < dt->h.nelem; i++){
    if(maxvalue < dt->data[i]) {
      maxvalue = dt->data[i];
      maxindex = i;
    }
  }
  // Adicioná-la ao conjunto de sementes
  robotuser->seeds->s[robotuser->curseed] = maxindex;
  robotuser->seeds->l[robotuser->curseed] = robotuser->mask->data[maxindex];
  robotuser->curseed++;
  robotuser->seeds->n = robotuser->curseed;
}
/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtRobotUserU8*
at_robotuseru8_new_from_array(AtArrayU8*  array,
                              AtArrayU8*  mask,
                              uint64_t    maxseeds,
                              AtAdjacency adjacency,
                              AtWeighting weighting,
                              AtPolicy    policy,
                              AtConnectivity conn){
  AtRobotUserU8* robot = malloc(sizeof(AtRobotUserU8));
  uint64_t* slist   = malloc(sizeof(uint64_t)*maxseeds);
  uint8_t * slabels = malloc(sizeof(uint8_t)*maxseeds);
  uint8_t   i;
  robot->nclasses   = at_arrayu8_max(mask);
  robot->step       = 0;
  robot->curseed    = 0;
  robot->array      = array;
  robot->mask       = mask;
  robot->diff       = at_arrayu8_new_with_data(mask->h.dim, mask->h.shape,mask->data,true);
  robot->g          = at_grapharrayu8_new(array,adjacency,weighting);
  robot->dt         = malloc(sizeof(AtArrayU16*)*(robot->nclasses+1));
  robot->ift        = NULL;
  robot->seeds      = at_seeds_new_from_list(0,slist, slabels);
  robot->conn       = conn;
  robot->policy     = policy;
  robot->finished   = false;
  for(i = 0; i < robot->nclasses+1; i++)
    robot->dt[i] = at_arrayu16_zeros(array->h.dim, array->h.shape);
  return robot;
}

void
at_robotuseru8_next(AtRobotUserU8* robotuser){
  uint64_t k;
  uint8_t i;
  if(robotuser->step == 0){
    // Get DT for each class
    for(i = 0; i < robotuser->nclasses; i++){
      at_arrayu8_distance_transform(robotuser->diff, i+1, robotuser->dt[i]);
      // Add seeds for each class
      add_seeds(robotuser, i);
    }
  }else{
    at_arrayu8_distance_transform(robotuser->diff, 0, robotuser->dt[robotuser->nclasses]);
    add_seeds(robotuser, robotuser->nclasses);
  }

  // Aplicar IFT
  robotuser->ift = at_ift_apply_arrayu8(robotuser->array, robotuser->g, robotuser->conn, robotuser->seeds, robotuser->policy, robotuser->ift);

  // Obter as diferenças
  for(k = 0; k < robotuser->array->h.nelem; k++){
    if(robotuser->mask->data[k] == robotuser->ift->l[k]) robotuser->diff->data[k] = 0;
    else                     robotuser->diff->data[k] = robotuser->mask->data[k];
  }
  robotuser->finished = !at_arrayu8_max(robotuser->diff);
  robotuser->step++;
}

void
at_robotuseru8_destroy(AtRobotUserU8** robotuserp){
  AtRobotUserU8* robot;
  uint8_t i;
  if(*robotuserp){
    robot = *robotuserp;
    at_seeds_destroy(&robot->seeds);
    free(robot->dt);
    at_arrayu8_destroy(&robot->diff);
    at_grapharray_destroy(&robot->g);
    for(i = 0; i < robot->nclasses+1; i++)
      at_arrayu16_destroy(&robot->dt[i]);
    free(robot);
  }
  *robotuserp = NULL;
}
