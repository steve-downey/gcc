// { dg-do compile }
// { dg-options "-fbacktick -std=c++20" }
// escape-module-names: without -fmodules GCC recognizes no module
// directives, so a line beginning module `f` y is still the infix call.
// Under -fmodules it is a directive (modules/backtick-module-name-2.C).

int fn (int a, int b) { return a + b; }

int t ()
{
  int module = 1, y = 2;
module `fn` y;
  return module `fn` y;
}
