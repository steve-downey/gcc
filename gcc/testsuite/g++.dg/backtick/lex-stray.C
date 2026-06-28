// { dg-do compile }
// Without -fbacktick, backtick is a stray character (CPP_OTHER).
void f() {
  `				// { dg-error "stray" }
}
