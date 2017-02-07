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
#include <at/imgproc.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include <setjmp.h>
#include <cmocka.h>

static
void test_png(void** state){
  AtArrayU8* image;
  AtError         * error = NULL;

  at_array_read_png(image,"nao_existe.png",&error);
  assert_non_null(error);
  at_error_destroy(&error);

  at_array_read_png(image,"teste_img_io",&error);
  assert_non_null(error);
  at_error_destroy(&error);

  at_array_read_png(image,"teste.png",&error);

  assert_null(error);
  assert_non_null(image);
  assert_int_equal(image->h.dim,3);
  assert_int_equal(image->h.shape[0], 965);//1150);
  assert_int_equal(image->h.shape[1], 800);//906);
  at_error_destroy(&error);

  at_array_write_png(image,"teste_w.png",&error);
  assert_null(error);

  at_array_destroy(&image);

  at_array_read_jpg(image,"teste.jpg",&error);

  assert_null(error);
  assert_non_null(image);
  assert_int_equal(image->h.dim,3);
  assert_int_equal(image->h.shape[0], 160);
  assert_int_equal(image->h.shape[1], 160);
  at_array_write_jpg(image,"teste_w.jpg",&error);
  at_array_write_ppm(image,"teste_w.ppm",&error);
  assert_null(error);

  at_array_destroy(&image);
  at_error_destroy(&error);


}

int main(void){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_png)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
