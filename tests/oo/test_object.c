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

#include <at/oo.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

// Header
#define AT_TYPE_FOO at_foo_get_type()
AT_DECLARE_CLASS(AtFoo, at_foo, AT, FOO, AtObject)

// Source
struct _AtFoo{
  AtObject super;
};
AT_DEFINE_CLASS(AtFoo , at_foo, AT_TYPE_OBJECT)
static void
at_foo_ctor(AtFoo *self){
}
static void
at_foo_class_ctor(AtFooClass *klass){
}

#define AT_TYPE_BAR at_bar_get_type()
AT_DECLARE_CLASS(AtBar, at_bar, AT, BAR, AtObject)
AT_DEFINE_CLASS_SIMPLE(AtBar, at_bar, AT_TYPE_OBJECT, AtObject)

#define AT_TYPE_ALICE at_alice_get_type()
AT_DECLARE_CLASS(AtAlice, at_alice, AT, ALICE, AtBar)
struct _AtAlice{
  AtBar super;
  int   variable;
};
AT_DEFINE_CLASS_CTOR(AtAlice, at_alice, AT_TYPE_BAR)

static void
at_alice_ctor(AtAlice *self){
  self->variable = 5;
}

// Tests
static void
test_object(void** state){
  // Creating an object
  AtFoo* foo = at_new(AT_TYPE_FOO, NULL);
  assert_non_null(foo);
  assert_int_equal(foo->super.refcount, 1);
  assert_true(AT_IS_OBJECT(foo));
  assert_true(AT_IS_FOO(foo));

  // Referencing the same object by another variable
  AtFoo* foo2 = NULL;
  at_set(&foo2,foo);
  assert_non_null(foo2);
  assert_int_equal(AT_OBJECT(foo2)->refcount, 2);
  assert_true(AT_IS_OBJECT(foo2));
  assert_true(AT_IS_FOO(foo2));
  at_clear(&foo);
  at_clear(&foo2);
  assert_null(foo);
  assert_null(foo2);

  // Testing automatic cleaning
  at_autoptr(AtFoo) foo3 = at_new(AT_TYPE_FOO, NULL);
  assert_int_equal(AT_OBJECT(foo3)->refcount, 1);

  at_autoptr(AtAlice) alice = at_new(AT_TYPE_ALICE,NULL);
  assert_int_equal(AT_OBJECT(alice)->refcount, 1);
  assert_int_equal(alice->variable,5);
}

int
main(int argc, char** argv){
  (void)argc;
  (void)argv;
  const struct CMUnitTest tests[1]={
    cmocka_unit_test(test_object),
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}
