// { dg-do compile }
// { dg-options "-fbacktick -fdump-tree-original" }
// G06: Precedence and associativity sweep — §4 Option A (D2).
// Left-associativity and unary-prefix (D2) are already confirmed in
// infix-basic.C; this file adds multiplicative precedence, member
// access, and ternary, plus an explicit left-assoc cross-check.

int f(int, int);
int g(int, int);

// -----------------------------------------------------------------------
// 1. Multiplicative precedence: a * b `f` c  ->  a * f(b, c)
//    backtick binds tighter than *, so * is the outer node.
// -----------------------------------------------------------------------
int a, b, c;
int r_mul = a * b `f` c;
// GCC canonicalises commutative MULT_EXPR with the call on the left.
// { dg-final { scan-tree-dump "f \\(b, c\\) \\* a" "original" } }

// -----------------------------------------------------------------------
// 2. Member-access operands: s1.x `f` s2.x  ->  f(s1.x, s2.x)
// -----------------------------------------------------------------------
struct S { int x; };
S s1, s2;
int r_mem = s1.x `f` s2.x;
// { dg-final { scan-tree-dump "f \\(s1\\.x, s2\\.x\\)" "original" } }

// -----------------------------------------------------------------------
// 3. Ternary: p ? q : r `f` d  ->  p ? q : f(r, d)
//    backtick is tighter than ?:, so the else-branch is f(r, d).
// -----------------------------------------------------------------------
int p, q, r, d;
int r_tern = p ? q : r `f` d;
// GCC normalises `p != 0 ? q : f(r,d)` to `p == 0 ? f(r,d) : q`.
// { dg-final { scan-tree-dump "p == 0 \\? f \\(r, d\\) : q" "original" } }

// -----------------------------------------------------------------------
// 4. Left-associativity: a `f` b `g` c  ->  g(f(a, b), c)
//    (confirms D1; already in infix-basic.C with the same variables)
// -----------------------------------------------------------------------
int r_chain = a `f` b `g` c;
// { dg-final { scan-tree-dump "g \\(f \\(a, b\\), c\\)" "original" } }
