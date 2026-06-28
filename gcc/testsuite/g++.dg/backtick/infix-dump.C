// { dg-do compile }
// { dg-options "-fbacktick -fdump-tree-original" }
// G03: infix desugars to CALL_EXPR in original tree dump.

int f(int, int);
int a, b;
int x = a `f` b;
// { dg-final { scan-tree-dump "f \\(a, b\\)" "original" } }
// { dg-bogus "stray" "" { target *-*-* } 0 }
