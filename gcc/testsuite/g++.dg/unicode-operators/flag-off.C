// { dg-do compile { target c++23 } }
// Without -funicode-operators nothing changes: an operator code point reaches
// the identifier lexer exactly as upstream, and the diagnostics are upstream's
// (unicode-operators.md, unicode-feature-gating).  The messages pinned here
// are what a pristine trunk cc1plus says.  (The testsuite runs in the C
// locale, where a name with a non-ASCII character is printed as a UCN.)

int a, b;
int c = a ⊞ b;   // { dg-error "extended character ⊞ is not valid in an identifier" }
// { dg-error "expected ',' or ';' before" "" { target *-*-* } .-1 }

int operator⊞(int, int);   // { dg-error "extended character ⊞ is not valid in an identifier" }

int d = \u2296 a;   // { dg-error "universal character .u2296 is not valid in an identifier" }
// { dg-error "was not declared in this scope" "" { target *-*-* } .-1 }
