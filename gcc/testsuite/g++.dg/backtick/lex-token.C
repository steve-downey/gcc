// { dg-do compile }
// { dg-options "-fbacktick" }
// With -fbacktick, backtick is lexed as CPP_BACKTICK (not a stray character).
// Since G07, a lone backtick in name position is treated as an escape attempt;
// the parser diagnoses the missing identifier (not "stray" and not the earlier
// "expected primary-expression").
void f() {
  `
}
// { dg-bogus "stray" "" { target *-*-* } 6 }
// { dg-error "backtick escape requires an identifier" "" { target *-*-* } 9 }
