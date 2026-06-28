// { dg-do compile }
// { dg-options "-fbacktick -fdump-tree-original" }
// G03: basic infix operator x `f` y -> f(x, y); verify via tree dump.

int add(int a, int b);
int mul(int a, int b);

int a, b, c;

// Basic desugaring: a `add` b -> add(a, b)
int r1 = a `add` b;
// { dg-final { scan-tree-dump "add \\(a, b\\)" "original" } }

// Left-associativity: a `add` b `mul` c -> mul(add(a, b), c)
int r2 = a `add` b `mul` c;
// { dg-final { scan-tree-dump "mul \\(add \\(a, b\\), c\\)" "original" } }

// Unary prefix binds to operand (Option A): -a `add` -b -> add(-a, -b)
// The dump uses NON_LVALUE_EXPR for negation of lvalues.
int r3 = -a `add` -b;
// { dg-final { scan-tree-dump "add .-.*a.*-.*b" "original" } }
