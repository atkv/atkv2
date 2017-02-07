#include <at/imgproc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

static void test_dt(void ** state){
  uint64_t    shape[2] = {4,4};
  AtArrayU16* dt;
  AtArrayU8*  array;
  uint8_t     data[16]  ={  0,  0,  0,  0,
                            0,  0,  0,  0,
                            0,255,255,  0,
                            0,255,  0,255};

  uint8_t   gt[16]     ={  5,  4,  4,  5,
                           2,  1,  1,  2,
                           1,  0,  0,  1,
                           1,  0,  1,  0};
  uint8_t i;

  array = at_arrayu8_new_with_data(2, shape, data, false);
  dt    = at_arrayu8_distance_transform(array, 255);
  for(i = 0; i < 16; i++)
    assert_int_equal(gt[i], dt->data[i]);
  at_arrayu8_destroy(&array);
  at_arrayu16_destroy(&dt);
}

int main(int argc, char *argv[])
{
  const struct CMUnitTest tests[4] = {
    cmocka_unit_test(test_dt)
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
