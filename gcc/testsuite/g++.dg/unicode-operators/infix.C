// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Infix parse of Unicode user-defined operators (unicode-operators.md,
// user-infix-precedence): one user-infix precedence level, left-associative,
// operands are cast-expressions.  This file is about grouping; which function
// is called is adl.C's job.  The grouping assertions are values, not tree
// text: each operator is constexpr and non-commutative, so a wrong grouping
// is a wrong number.

// 2a + b and 3a + b: neither is commutative and neither is associative, so
// every grouping below has a distinct value.
constexpr int operator⊞(int a, int b) { return 2 * a + b; }
constexpr int operator⊗(int a, int b) { return 3 * a + b; }

// --- the operator parses at all -------------------------------------------
static_assert((1 ⊞ 2) == 4);

// --- left-associative ------------------------------------------------------
// Same operator: right-associative would be 2*1 + (2*2+3) == 9.
static_assert(1 ⊞ 2 ⊞ 3 == 11);
// Two different operators at the same level: right-associative would be
// 2*1 + (3*2+3) == 11, and a two-level table could give anything.
static_assert(1 ⊞ 2 ⊗ 3 == 15);
static_assert(1 ⊗ 2 ⊞ 3 == 13);
// Parentheses regroup, and give the value the chain does not have.
static_assert((1 ⊞ (2 ⊗ 3)) == 11);

// --- tighter than every other binary operator ------------------------------
// Looser-or-equal to '*' would be 2*(2*3) + 4 == 16.
static_assert(2 * 3 ⊞ 4 == 20);
static_assert(2 ⊞ 3 * 4 == 28);   // (2 ⊞ 3) * 4 == 7 * 4
static_assert(1 + 2 ⊞ 3 == 8);    // 1 + (2*2+3)
static_assert(2 ⊞ 3 == 7);        // '==' is looser: (2 ⊞ 3) == 7
static_assert((1 ⊞ 2 ? 3 ⊞ 4 : 5) == 10);
static_assert((true ? 1 : 2) ⊞ 3 == 5);

// --- looser than unary: both operands are cast-expressions -----------------
// If the operator bound tighter than unary minus these would be -(1 ⊞ -2)
// == 0 and -(1 ⊞ 2) == -4 respectively.
static_assert(-1 ⊞ -2 == -4);
static_assert(-1 ⊞ 2 == 0);
static_assert(!0 ⊞ 1 == 3);
static_assert(+1 ⊞ +2 == 4);
static_assert(sizeof(int) ⊞ 0 == 8);

// A cast-expression on either side, which is exactly what the grammar says
// an operand is.
static_assert((int)1.9 ⊞ (int)2.9 == 4);

// --- the level is an ordinary level: it composes with the rest ------------
constexpr int assigns() {
  int x = 0;
  x = 1 ⊞ 2;          // assignment slot
  x += 1 ⊞ 0;         // compound assignment
  return x;
}
static_assert(assigns() == 6);

constexpr int arr[] = {1 ⊞ 2, 3 ⊞ 4};   // no comma confusion
static_assert(arr[0] == 4 && arr[1] == 10);

constexpr int call(int a, int b) { return a + b; }
static_assert(call(1 ⊞ 2, 3) == 7);      // argument position

template <int N> struct Box { static constexpr int value = N; };
static_assert(Box<(1 ⊞ 2)>::value == 4); // template-argument position

// --- templates: the parse is the same, dependent or not -------------------
template <class T> constexpr auto dep(T a, T b) { return a ⊞ b ⊗ a; }
static_assert(dep(1, 2) == 13);

// --- adjacency: no whitespace needed, and two operators in a row are two
// tokens, since no operator is a prefix of another and none is an identifier
// character.
static_assert(1⊞2 == 4);
constexpr int operator⊖(int a) { return 3 * a + 1; }
static_assert(1 ⊞⊖2 == 9);   // operator⊞(1, operator⊖(2)) == 2 + 7
