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
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <math.h>

/*=============================================================================
 TEST CASES
 ============================================================================*/

static void
test_jaccard_dice(void** state){
  AtArrayU8* a1;
  AtArrayU8* a2;
  uint64_t   shape[2]= {4,4};
  uint8_t    d1[16]  = {0,0,0,0, 0,1,1,0, 0,1,1,0, 1,1,1,1};
  uint8_t    d2[16]  = {0,0,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1};
  a1 = at_arrayu8_new_with_data(2,shape,d1,false);
  a2 = at_arrayu8_new_with_data(2,shape,d2,false);

  double jaccard = at_arrayu8_jaccard(a1,a2);
  double dice    = at_arrayu8_dice(a1,a2);
  assert_true(fabs(jaccard-0.6) < 1e-5);
  assert_true(fabs(dice-0.75) < 1e-5);

  d1[0] = 1;d1[1] = 1;

  jaccard = at_arrayu8_jaccard(a1,a2);
  dice    = at_arrayu8_dice(a1,a2);
  assert_true(fabs(jaccard-0.5) < 1e-5);
  assert_true(fabs(dice-0.6666666) < 1e-5);

  at_arrayu8_destroy(&a1);
  at_arrayu8_destroy(&a2);

}

static void
test_jaccard_dice16(void** state){
  AtArrayU16* a1;
  AtArrayU16* a2;
  uint64_t   shape[2]= {4,4};
  uint16_t    d1[16]  = {0,0,0,0, 0,1,1,0, 0,1,1,0, 1,1,1,1};
  uint16_t    d2[16]  = {0,0,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1};
  a1 = at_arrayu16_new_with_data(2,shape,d1,false);
  a2 = at_arrayu16_new_with_data(2,shape,d2,false);

  double jaccard = at_arrayu16_jaccard(a1,a2);
  double dice    = at_arrayu16_dice(a1,a2);
  assert_true(fabs(jaccard-0.6) < 1e-5);
  assert_true(fabs(dice-0.75) < 1e-5);

  d1[0] = 1;d1[1] = 1;

  jaccard = at_arrayu16_jaccard(a1,a2);
  dice    = at_arrayu16_dice(a1,a2);
  assert_true(fabs(jaccard-0.5) < 1e-5);
  assert_true(fabs(dice-0.6666666) < 1e-5);

  at_arrayu16_destroy(&a1);
  at_arrayu16_destroy(&a2);

}

int main(){
  const struct CMUnitTest tests[2] = {
    cmocka_unit_test(test_jaccard_dice),
    cmocka_unit_test(test_jaccard_dice16),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
