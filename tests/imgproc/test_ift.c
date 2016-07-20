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

#include <at/imgproc.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

static void
test_ift(){
  AtArrayU8   * array;
  AtSeeds     * seeds;
  AtIFT       * ift;
  AtGraphArray* g;

  // Our input data
  uint64_t slist[2]={  0,  15};       // seeds list
  uint64_t shape[4]={  4,  4};        // array shape
  uint8_t  slbls[2]={  0,   1};       // seeds labels
  uint8_t  data[16]={  0,  0,  0,  0, // array data
                       0,255,255,  0,
                       0,255,255,  0,
                     255,255,255,255};

  // Our output data
  double    ans_conn [16]  ={-INFINITY,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,-INFINITY};
  uint64_t  ans_root [16]  ={0, 0, 0, 0,  0,15,15, 0,  0,15,15, 0,  15,15,15,15};
  uint64_t  ans_pred [16]  ={0, 0, 1, 2,  0, 9,10, 3,  4,13,14, 7,  13,14,15,15};
  uint8_t   ans_label[16]  ={0, 0, 0, 0,  0, 1, 1, 0,  0, 1, 1, 0,   1, 1, 1, 1};
  uint8_t i;

  // The process
  array = at_arrayu8_new_with_data(2,shape,data,false);               // Creating array
  g     = at_grapharrayu8_new(array,AT_ADJACENCY_4,at_wdiffabs);      // Converting it to graph
  seeds = at_seeds_from_list(2,slist,slbls);                          // Creating our seeds
  ift   = at_ift_apply_arrayu8(array,g, at_conn_max, seeds, AT_FIFO); // Applying IFT

  // Our tests
  for(i = 0; i < 16; i++){
    assert_int_equal(ans_label[i],ift->l[i]);
    assert_int_equal(ans_root[i],ift->r[i]);
    assert_int_equal(ans_pred[i],ift->p[i]);
    if(ans_conn[i] != -INFINITY)
      assert_true(fabs(ans_conn[i]-ift->c[i]) < 0.00001);
    else
      assert_true(ans_conn[i] == ift->c[i]);
  }
  at_grapharray_destroy(&g);
  free(seeds);
  at_arrayu8_destroy(&array);
  free(ift);
}

static void
test_orfc_core(void** state){
  // Our input
  AtArrayU8   * array;
  AtSeeds     * seeds;
  AtGraphArray* g;
  AtSCC       * scc;
  double        alpha    = 0;
  uint64_t      shape[2] = { 4,  4};
  uint64_t      sindex[2]= { 0, 14};
  uint8_t       data[16] ={  0,  0,  0,  0,
                             0,255,255,  0,
                             0,255,255,  0,
                           255,255,255,255};
  uint8_t       slbl[2]  = {0,1};
  uint8_t       i;

  // Our output
  uint8_t       lbl[16]  ={  0,  0,  0,  0,
                             0,  1,  1,  0,
                             0,  1,  1,  0,
                             1,  1,  1,  1};
  array       = at_arrayu8_new_with_data(2, shape, data, false);
  at_wdiffabscalpha.params = &alpha;
  seeds       = at_seeds_new(2,sindex,slbl);
  g           = at_grapharrayu8_new(array,AT_ADJACENCY_4,at_wdiffabscalpha);

  // Our process
  scc         = at_orfc_core_arrayu8(array,g,at_conn_minr, seeds, 0, AT_FIFO, AT_SCC_TARJAN);

  // Our test
  assert_int_equal(scc->n, 2);
  for(i = 0; i < 16; i++){
    assert_int_equal(scc->l[i], lbl[i]);
  }

  // Our good practices
  at_grapharray_destroy(&g);
  free(seeds);
  at_arrayu8_destroy(&array);
  at_scc_destroy(&scc);
}

static void
test_orfc_core_multi(void** state){
  AtArrayU8   * array;
  AtSeeds     * seeds;
  AtGraphArray* g;
  AtSCC       * scc;
  double     alpha    = 0.04;
  uint64_t   shape[2] = {5,6};
  uint64_t   sidx[3]  = {0,21,10};
  uint8_t    data[30] ={  0,  0,  0,  0,  0,  0,
                          0,127,127,127, 30,  0,
                          0,127,  0,127, 30,  0,
                          0,127,127,127, 30,  0,
                          0,  0,  0,  0,  0,  0};
  uint8_t    lbls[30] ={  0,  0,  0,  0,  0,  0,  // Labels for ORFC Core
                          0,  1,  1,  1,  2,  0,
                          0,  1,  0,  1,  2,  0,
                          0,  1,  1,  1,  2,  0,
                          0,  0,  0,  0,  0,  0};
  uint8_t    lbl[30]  ={  0,  0,  0,  0,  0,  0,  // Labels for ORFC
                          0,  1,  1,  1,  1,  0,
                          0,  1,  0,  1,  1,  0,
                          0,  1,  1,  1,  1,  0,
                          0,  0,  0,  0,  0,  0};
  uint8_t    slbl[3]  = {0,1,1};
  uint8_t    i;
  at_wdiffabscalpha.params = &alpha;
  seeds    = at_seeds_new(3,sidx,slbl);
  array    = at_arrayu8_new_with_data(2, shape, data, false);
  g        = at_grapharrayu8_new(array,AT_ADJACENCY_4,at_wdiffabscalpha);
  scc      = at_orfc_core_arrayu8(array, g,at_conn_minr,seeds, 0, AT_FIFO, AT_SCC_TARJAN);

  assert_int_equal(scc->n, 3);
  for(i = 0; i < 30; i++){
    assert_int_equal(scc->l[i], lbls[i]);
  }

  at_grapharrayu8_renew_edges(g);
  AtIFT* ift = at_orfc_arrayu8(array, g, at_conn_minr, seeds, 0, AT_FIFO);
  for(i = 0; i < 30; i++){
    assert_int_equal(ift->l[i], lbl[i]);
  }
  free(seeds);
  at_scc_destroy(&scc);
  at_arrayu8_destroy(&array);
  at_grapharray_destroy(&g);
  free(ift);
}

static void
test_orfc(void** state){
  uint64_t   shape[2] = {4,4};
  AtArrayU8* array;
  uint8_t    data[16]  ={  0,  0,  0,  0,
                           0,255,255,  0,
                           0,255,255,  0,
                         255,255,255,255};
  uint8_t    lbl[16]   ={  0,  0,  0,  0,
                           0,  1,  1,  0,
                           0,  1,  1,  0,
                           1,  1,  1,  1};
  array                = at_arrayu8_new_with_data(2, shape, data, false);
  uint64_t     sidx[2] = {0,14};
  uint8_t      slbl[2] = {0, 1};
  AtSeeds     *seeds   = at_seeds_new(2,sidx,slbl);
  double alpha = 0.0;
  at_wdiffabscalpha.params = &alpha;
  AtGraphArray*g       = at_grapharrayu8_new(array,AT_ADJACENCY_4,at_wdiffabscalpha);
  AtIFT       *ift     = at_orfc_arrayu8(array, g, at_conn_minr, seeds, 0, AT_FIFO);
  uint8_t      i;

  for(i = 0; i < 16; i++){
    assert_int_equal(ift->l[i], lbl[i]);
  }
  at_grapharray_destroy(&g);
  at_arrayu8_destroy(&array);
  free(seeds);
  free(ift);
}

int main(void){
  const struct CMUnitTest tests[4] = {
    cmocka_unit_test(test_ift),
    cmocka_unit_test(test_orfc_core),
    cmocka_unit_test(test_orfc_core_multi),
    cmocka_unit_test(test_orfc),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
