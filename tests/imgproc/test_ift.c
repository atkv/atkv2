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

#include <at/imgproc/ift.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
static void
test_ift(){
  AtArrayU8 * array;
  AtArrayU64* seeds;
  AtIFT              * ift;

  uint64_t seeds_data[4]={  0,  0,
                           15,  1};
  uint64_t shape[4]     ={  4,  4};
  uint8_t  data[16]     ={  0,  0,  0,  0,
                            0,255,255,  0,
                            0,255,255,  0,
                          255,255,255,255};
  uint8_t   ans_label[16] ={0,0,0,0, 0,1,1,0, 0,1,1,0, 1,1,1,1};
  uint64_t  ans_root [16]  ={0,0,0,0, 0,15,15,0, 0,15,15,0, 15,15,15,15};
  uint64_t  ans_pred [16]  ={0,0,1,2, 0,9,10,3, 4,13,14,7, 13,14,15,15};
  double    ans_conn [16]  ={-INFINITY,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,-INFINITY};

  array = at_arrayu8_new_with_data(2,shape,data,false);
  seeds = at_seeds_new(2,seeds_data);

  ift   = at_ift_apply_arrayu8(array, AT_ADJACENCY_4,AT_MINIMIZATION,
                                     at_conn_max,at_weighting_diff_abs, seeds,
                                     AT_FIFO);
  uint8_t i;
  for(i = 0; i < 16; i++){
    assert_int_equal(ans_label[i],ift->l[i]);
    assert_int_equal(ans_root[i],ift->r[i]);
    assert_int_equal(ans_pred[i],ift->p[i]);
    if(ans_conn[i] != -INFINITY)
      assert_true(fabs(ans_conn[i]-ift->c[i]) < 0.00001);
    else
      assert_true(ans_conn[i] == ift->c[i]);
  }

}

static void
test_orfc_core(void** state){
  uint64_t   shape[2] = {4,4};
  AtArrayU8* array;
  uint8_t    data[16] ={  0,  0,  0,  0,
                          0,255,255,  0,
                          0,255,255,  0,
                        255,255,255,255};
  array               = at_arrayu8_new_with_data(2, shape, data, false);
  uint64_t   sdata[4] = {0,0, 14,1};
  AtArrayU64*seeds    = at_seeds_new(2,sdata);
  AtSCC    * scc      = at_ift_orfc_core_arrayu8(array,AT_ADJACENCY_4, AT_MINIMIZATION, at_conn_max, at_weighting_diff_abs, seeds, 0, AT_FIFO, AT_SCC_TARJAN);
  uint8_t    i;

  assert_int_equal(scc->n, 2);
  for(i = 0; i < 16; i++){
    assert_int_equal(scc->l[i], data[i]);
  }
}

int main(void){
  const struct CMUnitTest tests[2] = {
    cmocka_unit_test(test_ift),
    cmocka_unit_test(test_orfc_core),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
