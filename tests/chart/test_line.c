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

#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <at/chart.h>
#include <math.h>

static void
test_line(void** state){
  double     data[100];
  uint64_t   i;
  for(i = 0; i < 100; i++)
    data[i] = sin(M_PI/25*i) + 1;
  AtChart  * chart = at_chart_new();
  at_chart_plot_d64 (chart, data, 100);
  assert_int_equal  (chart->nplots, 1);
  assert_non_null   (chart->lineplots);
  assert_int_equal  (chart->lineplots->nelem,100);
  assert_non_null   (chart->lineplots->values);
  at_chart_write_pdf(chart, "line.pdf",640,480);
  at_chart_destroy  (&chart);
}

int main(void){
  const struct CMUnitTest tests[1] = {
    cmocka_unit_test(test_line),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
