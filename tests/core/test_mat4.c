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

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cmocka.h>
#include <at/core.h>
#include <math.h>

static void
test_mat4(void** state){
  AtMat4D64 mat;
  AtVec3D64 scale  = {1,2,3};
  AtVec3D64 rotate = {0,1,0};
  at_mat4d64_eye(&mat);
  assert_int_equal(mat.d[0] ,1);
  assert_int_equal(mat.d[5] ,1);
  assert_int_equal(mat.d[10],1);

  at_mat4d64_scale(&mat, scale);
  assert_int_equal(mat.d[0] ,1);
  assert_int_equal(mat.d[5] ,2);
  assert_int_equal(mat.d[10],3);

  at_mat4d64_translate(&mat, scale);
  assert_int_equal(mat.d[12] ,1);
  assert_int_equal(mat.d[13] ,2);
  assert_int_equal(mat.d[14] ,3);

  AtVec4D64 vec={1,2,3,1}, res;
  res = at_mat4d64_dot(&mat,vec);
  assert_int_equal(res.x,2);
  assert_int_equal(res.y,6);
  assert_int_equal(res.z,12);
  assert_int_equal(res.w,1);

  at_mat4d64_eye(&mat);
  at_mat4d64_rotate(&mat, 90, rotate);
  double rot_data[16] = {0,0,-1,0, 0,1,0,0, 1,0,0,0, 0,0,0,1};
  uint8_t i;
  for(i = 0; i < 16; i++)
    assert_true(fabs(rot_data[i]-mat.d[i])<1e-5);

  mat = at_mat4d64_ortho(-100,100,-100,100,100,-100);
  assert_true(fabs(mat.d[0] -1.0/100)<1e-5);
  assert_true(fabs(mat.d[5] -1.0/100)<1e-5);
  assert_true(fabs(mat.d[10]-1.0/100)<1e-5);
  assert_true(fabs(mat.d[12])<1e-5);
  assert_true(fabs(mat.d[13])<1e-5);
  assert_true(fabs(mat.d[14])<1e-5);
  assert_true(fabs(mat.d[1])<1e-5);
  assert_true(fabs(mat.d[2])<1e-5);
  assert_true(fabs(mat.d[3])<1e-5);
  assert_true(fabs(mat.d[4])<1e-5);
  assert_true(fabs(mat.d[6])<1e-5);
  assert_true(fabs(mat.d[7])<1e-5);
  assert_true(fabs(mat.d[8])<1e-5);
  assert_true(fabs(mat.d[9])<1e-5);
  assert_true(fabs(mat.d[11])<1e-5);
  assert_true(fabs(mat.d[15]-1)<1e-5);
}


int main(){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_mat4)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
