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

#include <at/imgproc/nifti.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <time.h>
static void
test_at_nifti_read(void** state){
  AtError     * error = NULL;
  AtNiftiImage* nifti;
  nifti         = at_niftiimage_read("MRHead.nii.gz",true,&error);
  assert_int_equal(nifti->nifti_type,AT_NIFTI_TYPE_NIFTI1_1);
  assert_int_equal(nifti->ndim,3);
  assert_int_equal(nifti->dim[0],256);
  assert_int_equal(nifti->dim[1],256);
  assert_int_equal(nifti->dim[2],130);
  at_niftiimage_destroy(&nifti);
}

static void
test_at_nifti_write(void** state){

}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_at_nifti_read),
    cmocka_unit_test(test_at_nifti_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
