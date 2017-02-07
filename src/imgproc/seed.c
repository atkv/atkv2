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

#include <at/imgproc.h>

/*=============================================================================
 PUBLIC API
 ============================================================================*/

AtSeeds*
at_seeds_from_list(uint64_t num_seeds, uint64_t* slist, uint8_t* labels){
  AtSeeds* seeds = malloc(sizeof(AtSeeds));
  seeds->s = slist;
  seeds->l = labels;
  seeds->n = num_seeds;
  seeds->m = NULL;
  return seeds;
}

AtSeeds*
at_seeds_from_mask(AtArrayU8* mask){
  AtSeeds* seeds = malloc(sizeof(AtSeeds));
  seeds->l = malloc(mask->h.nelem);
  seeds->s = malloc(mask->h.nelem<<3);
  seeds->n = 0;
  seeds->m = mask;

  uint64_t  * seeds_data = malloc(mask->h.nelem << 4); // numelem x 2^1 pairs x 2^3 bytes
  uint64_t    k = 0, i;
  for(i = 0; i < mask->h.nelem; i++){
    if(mask->data[i] != UINT8_MAX){
      seeds->s[k] = i;
      seeds->l[k] = mask->data[i];
      k++;
    }
  }
  seeds->n = k;
  seeds->l = realloc(seeds->l,k);
  seeds->s = realloc(seeds->s,k<<3);

//  uint64_t shape[2]   = {k>>1,2};
//  seeds_data = realloc(seeds_data,k << 3);
//  //AtArrayU64* seeds   = at_arrayu64_new_with_data(2,shape,seeds_data,false);
//  seeds->h.owns_data  = true;
  return seeds;
}

void
at_seeds_mask_generate(AtSeeds* seeds, uint8_t dim, uint64_t* shape){
  uint64_t i;
  seeds->m = at_arrayu8_new(dim,shape);                      // creating a mask
  memset(seeds->m->data,UINT8_MAX,seeds->m->h.nelem); // UINT8_MAX means "not a seed"
  for(i = 0; i < seeds->n; i++){                             // mark all seeds with their labels
    seeds->m->data[seeds->s[i]] = seeds->l[i];
  }
}

void
at_seeds_split(AtSeeds* seeds, AtSeeds** sbackp, AtSeeds** sobjp, uint64_t lblback){
  AtSeeds* s[2]={0,0};
  uint64_t n[2]={0,0};
  uint64_t k[2]={0,0};
  uint64_t i;
  uint8_t  ind;
  // Get number of seeds for each label
  for(i = 0; i < seeds->n; i++)
    n[seeds->l[i] == lblback]++;

  // Allocate them
  for(i = 0; i < 2; i++){
    s[i]    = malloc(sizeof(AtSeeds));
    s[i]->l = malloc(n[i]);
    s[i]->s = malloc(n[i]<<3);
    s[i]->n = n[i];
    s[i]->m = NULL;
    if(seeds->m){
      s[i]->m = at_arrayu8_new(seeds->m->h.dim, seeds->m->h.shape);
      memset(s[i]->m->data,UINT8_MAX,s[i]->m->h.nelem);
    }
  }

  // Fill them
  for(i = 0; i < seeds->n; i++){
    ind = seeds->l[i] == lblback;
    s[ind]->l[k[ind]] = seeds->l[i];
    s[ind]->s[k[ind]] = seeds->s[i];
    if(s[ind]->m)
      s[ind]->m->data[seeds->s[i]] = seeds->l[i];
    k[ind]++;
  }

  *sbackp = s[1];
  *sobjp  = s[0];


//  sback->s = malloc(n[0]);
//  sback->l = malloc(n[0]);
//  sback->n = n[0];
//  sobj->s  = malloc(n[0]);
//  sobj->l  = malloc(n[1]);
//  sobj->n  = n[1];

//  for(i = 0; i < seeds->n; i++)
//    n[seeds->l[i] == lblback]++;


//  uint64_t a,k,i, size;
//  size = seeds->h.nelem << 1;
//  uint64_t* sbackdata = malloc(seeds->h.nelem << 3);
//  uint64_t* sobjdata  = malloc(seeds->h.nelem << 3);
//  k = 0;
//  a = 0;
//  for(i = 0; i < seeds->h.nelem; i+=2) if(seeds->data[i+1] == lblback){
//    sbackdata[k++] = seeds->data[i];
//    sbackdata[k++] = lblback;
//  }else{
//    sobjdata[a++] = seeds->data[i];
//    sobjdata[a++] = seeds->data[i+1];
//  }
//  sbackdata = realloc(sbackdata,k<<3);
//  sobjdata  = realloc(sobjdata,a<<3);
//  uint64_t sbackshape[2] = {k>>1,2};
//  uint64_t sobjshape[2]  = {a>>1,2};
//  *sback = at_seeds_from_list(at_arrayu64_new_with_data(2,sbackshape,sbackdata,false);
//  *sobj  = at_arrayu64_new_with_data(2,sobjshape,sobjdata,false);
//  (*sback)->h.owns_data = true;
//  (*sobj)->h.owns_data  = true;
}

AtSeeds*
at_seeds_new(uint64_t n, uint64_t* seeds, uint8_t* labels){
  AtSeeds* s = malloc(sizeof(AtSeeds));
  s->s = seeds;
  s->l = labels;
  s->n = n;
  s->m = NULL;
  return s;
}

void
at_seeds_destroy(AtSeeds** seeds){
  if((*seeds)->l)free((*seeds)->l);
  if((*seeds)->s)free((*seeds)->s);
  free(*seeds);
  *seeds = NULL;
}
