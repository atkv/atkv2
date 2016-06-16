#include <ift.h>

AtArray_uint64_t*
at_seeds_new(uint64_t num_seeds, uint64_t* seeds_data){
  uint64_t seeds_shape[2] = {num_seeds,2};
  return at_array_uint64_t_new_with_data(2,seeds_shape, seeds_data);
}

int main(int argc, char const *argv[])
{
  uint64_t  seeds_data[4]  = {0,0,  15,1}; // 32 bytes - offset  0
  uint64_t  array_shape[2] = {4,4};        // 16 bytes - offset 32
  uint8_t   data[16]       = {1,1,1,1,     // 16 bytes - offset 48
                              0,0,0,0,
                              0,0,0,0,
                              0,0,0,0};
  AtArray * array;                         //  8 bytes - offset 64
  IFT     * ift;                           //  8 bytes - offset 72
  AtArray * seeds;                         //  8 bytes - offset 80
                                           // Total: 88 bytes
  // Creating data
  array = at_array_new_with_data(2, array_shape, data);
  seeds = at_seeds_new(2,seeds_data);

  // Applying IFT
  ift   = at_ift_apply_array(array,
                             AT_ADJACENCY_4,
                             AT_MINIMIZATION,
                             at_path_max,
                             at_weight_abs_diff,
                             seeds);
  // Output

  // Clearing memory
  at_array_destroy(array);
  at_ift_destroy(ift);
  free(seeds);

  return 0;
}
