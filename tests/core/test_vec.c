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
#include <math.h>
#include <at/core.h>
static void
test_vec(void** state){
  AtVec2U8 v1u, v2u, v3u;
  v1u.x = 3;  v1u.y = 100;
  v2u.x = 24; v2u.y = 30;

  v3u.v = v1u.v + v2u.v;
  assert_int_equal(v3u.x, 27);
  assert_int_equal(v3u.y, 130);

  AtVec3I64 v1i, v2i, v3i;
  v1i.x = 24;  v1i.y = 26;  v1i.z = 1024;
  v2i.x = 431; v2i.y = 123; v2i.z = 512;

  v3i.v = v1i.v * v2i.v;
  assert_int_equal(v3i.r, 24*431);
  assert_int_equal(v3i.g, 26*123);
  assert_int_equal(v3i.b, 1024*512);

  AtVec4F32 v1f, v2f, v3f;
  v1f.x = 24;  v1f.y = 26;  v1f.z = 1024;v1f.w = 2;
  v2f.x = 431; v2f.y = 123; v2f.z = 512; v2f.w = 6;

  v3f.v = v1f.v - v2f.v;
  assert_true(fabs(v3f.x-(24.0f-431.0f)) < .1e-10);
  assert_true(fabs(v3f.y-(26.0f-123.0f)) < .1e-10);
  assert_true(fabs(v3f.z-(1024.0f-512.0f)) < .1e-10);
  assert_true(fabs(v3f.w-(2.0f-6.0f)) < .1e-10);


}

int main(){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_vec)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
