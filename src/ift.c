#include <at/ift.h>

IFT*
at_ift_apply_array(AtArray*             array,
                   AtAdjacency        adjacency,
                   AtOptimization     optimization, 
                   AtConnectivityFunc connectivity,
                   AtWeightingFunc    weighting,
                   AtArray*             seeds){
  IFT       * ift;         // (8 bytes - offset  0)
  AtGraphArray* graph;       // (8 bytes - offset  8)
  AtQueue   * queue;       // (8 bytes - offset 16)
  double      new_conn;    // (8 bytes - offset 24)                           
  uint64_t    s;           // (8 bytes - offset 32)
  uint64_t    t;           // (8 bytes - offset 40)
  uint64_t    offset;      // (8 bytes - offset 48)
  uint64_t    offset_next; // (8 bytes - offset 56)
  uint8_t     i;           // (1 bytes - offset 64)
  uint8_t     padding[7];  // (6 bytes - offset 65)
                           // Total: 72 bytes
                           
  // Create the auxiliary structures
  ift       = at_ift_new();
  graph     = at_grapharray_new();
  queue     = at_queue_new();
  at_grapharray_init(graph, array,    adjacency, weighting);
  at_ift_init       (  ift, array, connectivity);
  at_queue_init     (queue, seeds);

  // Main loop
  while(!at_queue_is_empty(queue)){
    // get best node
    s           = at_queue_remove(queue);

    // loop through neighbors
    offset      = s      * adjacency;
    offset_next = offset + adjacency;
    for(i = offset; i < offset_next; i++){
      if(graph->active[i]){
        t        = graph->neighbors[i];
        new_conn = connectivity(array, ift, s, t);
        if((optimization == AT_MAXIMIZATION && ift->connectivities[t] < new_conn)||
           (optimization == AT_MINIMIZATION && ift->connectivities[t] > new_conn)){
          ift->connectivities[t] = new_conn;
          ift->predecessors[t]   = s;
          ift->roots[t]          = ift->roots[s];
          ift->labels[t]         = ift->labels[s];
          at_queue_add(queue, (uint8_t)ift->connectivities[t], t);
        }
      }
    }
  }

  // Clear memory
  at_queue_destroy(queue);
  at_grapharray_destroy(graph);

  return ift;
}
