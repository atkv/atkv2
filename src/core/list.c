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

#include <at/core.h>
#include <string.h>
#include <stdlib.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/*=============================================================================
 PRIVATE API
 ============================================================================*/

AtListU64*
at_listu64_new_array(uint64_t n_items){
  AtListU64* list = calloc(n_items, sizeof(AtListU64));
  return list;
}

void
at_listu64_destroy(AtListU64** list_ptr){
  if(*list_ptr){
    AtListU64* list = *list_ptr;
    AtListU64* next;
    do{
      next = list->next;
      free(list);
      list = next;
    }while(list != NULL);
    *list_ptr = NULL;
  }
}

void
at_listu64_destroy_array(AtListU64** list){
  if(*list) free(*list);
  *list = NULL;
}
