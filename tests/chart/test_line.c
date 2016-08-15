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
test_linechart(void** state){
  double     data[100];
  uint64_t   i;
  for(i = 0; i < 100; i++)
    data[i] = sin(M_PI/25*i) + 1;

  AtSeriesScatter* scatter = at_series_scatter_new();
  assert_null     (scatter->base.name);
  assert_int_equal(scatter->base.opacity, 1);
  assert_int_equal(scatter->base.type, AT_SERIES_SCATTER);
  assert_int_equal(scatter->base.visible, true);
  assert_int_equal(scatter->linecfg.capstyle, AT_CAP_ROUND);
  assert_int_equal(scatter->num, 100);
  for(i = 0; i < 100; i++) assert_int_equal(scatter->x[i],i);
  at_series_scatter_fill_1D(scatter,data,100);
  assert_ptr_equal(scatter->y,data);

}

static void
test_line(void** state){
  double     data[100];
  uint64_t   i;
  for(i = 0; i < 100; i++)
    data[i] = sin(M_PI/25*i) + 1;
  AtChart  * chart = at_chart_new();
  AtSubchart* sc   = (AtSubchart*)chart->subcharts->value;
  assert_non_null(sc);
  at_chart_plot_d64 (chart, data, 100);
  assert_int_equal  (sc->nplots, 1);
  assert_non_null   (sc->axis);
  assert_null       (chart->title);
  assert_null       (sc->title);
  assert_non_null   (sc->plotlist);
  AtLinePlot* lineplot = (AtLinePlot*)sc->plotlist->value;
  assert_non_null(lineplot);
  assert_int_equal  (lineplot->nelem,100);
  assert_non_null   (lineplot->y);
  at_chart_plot_d64 (chart, data, 100);
  assert_int_equal  (sc->nplots, 2);
  at_chart_destroy  (&chart);
}

int main(void){
  const struct CMUnitTest tests[2] = {
    cmocka_unit_test(test_line),
    cmocka_unit_test(test_linechart),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
