// { dg-do compile }
// { dg-options "-fbacktick" }
// With -fbacktick, backtick is lexed as CPP_BACKTICK (not a stray character).
// The parser gives a parse error (infix parsing not yet in G03), but NOT stray.
void f() {
  `
}
// { dg-bogus "stray" "" { target *-*-* } 6 }
// { dg-error "expected primary-expression" "" { target *-*-* } 6 }
