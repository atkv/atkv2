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

#include <at/list.h>
#include <string.h>
#include <stdlib.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PRIVATE API
 ============================================================================*/

AtList_uint64_t*
at_list_uint64_t_new_array(uint64_t n_items){
  AtList_uint64_t* list = calloc(n_items, sizeof(AtList_uint64_t));
  return list;
}


AtList_uint64_t*
at_list_uint64_t_remove_first_link(){

}
