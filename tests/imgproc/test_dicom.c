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

#include <at/imgproc/dicom.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

static void
test_at_dicom_read(void** state){
  const char* filename = "MR-MONO2-16-head.dcm";
  AtDicomU16* dicom = NULL;
  AtError*          error = NULL;

  dicom = at_dicomu16_read(filename, &error);

  assert_non_null(dicom);

  assert_string_equal(dicom->base.filename, filename);
  assert_string_equal(dicom->base.modality, "MR");
  assert_true(dicom->base.pixel_data_tag_found);
  assert_string_equal(dicom->base.unit, "mm");
  assert_int_equal(dicom->image->h.dim, 3);
  assert_int_equal(dicom->base.bits_allocated, 16);
  assert_int_equal(dicom->image->h.shape[0], 256);
  assert_int_equal(dicom->image->h.shape[1], 256);
  assert_int_equal(dicom->image->h.shape[2], 1);
}

static void
test_at_dicom_write(void** state){
  (void) state;
}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[2]={
    cmocka_unit_test(test_at_dicom_read),
    cmocka_unit_test(test_at_dicom_write),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
