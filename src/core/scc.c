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
#include <at/core/scc.h>
/*=============================================================================
 PRIVATE API
 ============================================================================*/
/**
 * @brief structure for simulate recursive function stack
 */
typedef struct{
  uint64_t i;
  uint64_t v;
}StackFuncItem;

/**
 * @brief Applies Tarjan algorithm to find SCC in a graph array
 *
 * Pseudocode
 * ~~~c
 * v.index   = index
 * v.lowlink = lowlink;
 * index++
 * s.push(v) //add to the stack
 * for each Node n in Adj(v) do //for all descendants
 *   if n.index == -1 //if the node was not discovered yet
 *     tarjanAlgorithm(n, scc, s, index) //search
 *     v.lowlink = min(v.lowlink, n.lowlink) //modify parent's lowlink
 *   else if stack.contains(n) //if the component was not closed yet
 *     v.lowlink = min(v.lowlink, n.index) //modify parents lowlink
 *
 * if v.lowlink == v.index //if we are in the root of the component
 *   Node n = null
 *   List component //list of nodes contained in the component
 *   do
 *     n = stack.pop() //pop a node from the stack
 *     component.add(n) //and add it to the component
 *   while(n != v) //while we are not in the root
 *   scc.add(component) //add the compoennt to the SCC list
 * ~~~
 *
 * @param g
 * @param scc
 * @param v
 * @param index
 * @param lowlink
 * @param curindex
 * @param s
 * @param si
 * @param sp
 * @return
 *
 *
 */
static uint32_t
at_grapharrayu8_scc_tarjan(AtGraphArray* g, AtSCC* scc, uint64_t v,
                           uint32_t* index, uint32_t curindex,
                           uint64_t* s, uint64_t* si, uint64_t* sp, StackFuncItem* sf){
  uint64_t off, i, n, start;
  uint64_t sfi = 0;
  uint8_t  recursing = true, returning = false;

  do{
    // Setting index and lowlink for v
    if(!returning && recursing){
      index[v]   = curindex;
      scc->l[v]  = curindex++;

      s[*si] = v;
      sp[v]  = (*si)++;
      start  = 0;
      recursing = false;
    }else
      start = sf[sfi].i;
    off = v*g->adjacency;
    for(i = start; i < g->adjacency; i++){
      if(g->active[off+i]){
        n = g->neighbors[off+i];
        if(returning){
          scc->l[v] = min(scc->l[v],scc->l[n]);
          returning = false;
        }else if(index[n] == UINT32_MAX){
          sf[sfi].v          = v;
          sf[sfi++].i        = i;
          recursing = true;
          v = n;
          break;
        }else if(sp[n]!=UINT64_MAX)
          scc->l[v] = min(scc->l[v],index[n]);
      }
    }
    if(!recursing){
      if(scc->l[v] == index[v]){
        do{
          n = s[--(*si)];
          scc->l[n] = scc->l[v];
          sp[n] = UINT64_MAX;
        }while(n != v);
      }
      returning = true;
      if(sfi > 0)
        v = sf[sfi-1].v;
      sfi--;
    }
  }while(sfi < (uint64_t)-1);
  return curindex;
}

AtSCC*
at_scc_new(AtGraphArray* g){
  AtSCC* scc = malloc(sizeof(AtSCC));
  scc->l     = malloc(g->h->num_elements << 2);
  memset(scc->l,UINT32_MAX,g->h->num_elements << 2);
  return scc;
}

/*=============================================================================
 PUBLIC API
 ============================================================================*/
AtSCC*
at_grapharrayu8_scc(AtGraphArray* g, AtSCCAlgorithm a){
  AtSCC* scc = at_scc_new(g);
  // Tarjan variables
  uint64_t *s, *sp;        // stack and stack positions
  uint32_t *index, *indseq;// index map and consecutive labels
  uint64_t i, si;          // counter and stack index
  uint32_t curindex;       // current index
  StackFuncItem* sf;
  switch(a){
    case AT_SCC_TARJAN:
      // Create a stack
      // For each node in G
      // if (node is undefined)
      //   apply tarjan algorithm
      //
      index   = malloc(g->h->num_elements << 2);
      indseq  = malloc(g->h->num_elements << 2);
      s       = malloc(g->h->num_elements << 4);
      sp      = malloc(g->h->num_elements << 4);
      sf      = malloc(g->h->num_elements * sizeof(StackFuncItem));
      si      = 0;
      curindex= 0;
      memset(index ,UINT32_MAX,g->h->num_elements << 2);
      memset(indseq,UINT32_MAX,g->h->num_elements << 2);
      memset(sp    ,UINT32_MAX,g->h->num_elements << 4);

      // Calculate tarjan
      for(i = 0; i < g->h->num_elements; i++)
        if(scc->l[i] == UINT32_MAX)
          curindex = at_grapharrayu8_scc_tarjan(g, scc, i, index, curindex, s, &si, sp, sf);

      // Consecutive labels
      si = 0;
      for(i = 0; i < g->h->num_elements; i++){
        if(indseq[scc->l[i]] == UINT32_MAX)
          indseq[scc->l[i]] = si++;
        scc->l[i] = indseq[scc->l[i]];
      }
      scc->n = si;

      // Memory management
      free(index);
      free(indseq);
      free(s);
      free(sp);
      free(sf);
      break;
  }
  return scc;


}

void
at_scc_destroy(AtSCC** scc){
  if((*scc)->l) free((*scc)->l);
  free(*scc);
  *scc = NULL;
}

