// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Precedence and associativity of Unicode user-defined operators
// (unicode-operators.md, user-infix-precedence, user-declared-fixity): one
// precedence level for all user-introduced infix, left associative, operands
// are cast-expressions, prefix binds tighter than any binary operator.
//
// Two oracles: values (every operator is constexpr and neither commutative
// nor associative, so a wrong grouping is a wrong number) and types (the same
// operators overloaded on Tag<N>, so a grouping claim is
// __is_same(decltype(EXPR), Tag<N>) and every chain can be asserted equal to
// its explicitly parenthesized left-nested form).  No declaration below says
// anything about binding; the declaration order of ⊞ and ⊗ is arbitrary, and
// every mixed chain groups strictly left-to-right regardless.

template <int N> struct Tag { static constexpr int value = N; };

// The value oracle.
constexpr int operator⊞(int a, int b) { return 2 * a + b; }
constexpr int operator⊗(int a, int b) { return 3 * a + b; }
constexpr int operator⊖(int a) { return 3 * a + 1; }

// The type oracle: the same arithmetic, one level up.
template <int A, int B> constexpr Tag<2 * A + B> operator⊞(Tag<A>, Tag<B>) { return {}; }
template <int A, int B> constexpr Tag<3 * A + B> operator⊗(Tag<A>, Tag<B>) { return {}; }
template <int A> constexpr Tag<3 * A + 1> operator⊖(Tag<A>) { return {}; }

// A redeclaration of the same operator, spelled as a universal-character-name.
// The grammar level is a property of the token kind and not of how the token
// was written.
constexpr int operator\U0000229E(int a, int b);
static_assert(1 ⊞ 2 == 4);
static_assert(1 \N{SQUARED PLUS} 2 ⊞ 3 == 11);   // still left-associative
static_assert(⊖ 1 ⊞ 2 == 10);               // still prefix-tighter

constexpr Tag<1> a{};
constexpr Tag<2> b{};
constexpr Tag<3> c{};
constexpr Tag<4> d{};

// 1. Left-associative.
static_assert(1 ⊞ 2 ⊞ 3 == 11);
static_assert(__is_same(decltype(a ⊞ b ⊞ c), Tag<11>));
static_assert(__is_same(decltype(a ⊞ b ⊞ c), decltype((a ⊞ b) ⊞ c)));
static_assert(!__is_same(decltype(a ⊞ b ⊞ c), decltype(a ⊞ (b ⊞ c))));
static_assert(1 ⊞ 2 ⊞ 3 ⊞ 4 == 26);
static_assert(__is_same(decltype(a ⊞ b ⊞ c ⊞ d), decltype(((a ⊞ b) ⊞ c) ⊞ d)));

// 2. One level for all user-introduced infix.
static_assert(1 ⊞ 2 ⊗ 3 == 15);   // (1 ⊞ 2) ⊗ 3 == 3*4 + 3
static_assert(1 ⊗ 2 ⊞ 3 == 13);   // (1 ⊗ 2) ⊞ 3 == 2*5 + 3
static_assert(__is_same(decltype(a ⊞ b ⊗ c), Tag<15>));
static_assert(__is_same(decltype(a ⊗ b ⊞ c), Tag<13>));
static_assert(__is_same(decltype(a ⊞ b ⊗ c), decltype((a ⊞ b) ⊗ c)));
static_assert(__is_same(decltype(a ⊗ b ⊞ c), decltype((a ⊗ b) ⊞ c)));
static_assert(1 ⊞ 2 ⊗ 3 ⊞ 4 == 34);
static_assert(1 ⊗ 2 ⊞ 3 ⊗ 4 == 43);
static_assert(__is_same(decltype(a ⊞ b ⊗ c ⊞ d), decltype(((a ⊞ b) ⊗ c) ⊞ d)));
static_assert(__is_same(decltype(a ⊗ b ⊞ c ⊗ d), decltype(((a ⊗ b) ⊞ c) ⊗ d)));
// The level does not depend on which operator was declared first.
constexpr int operator⊚(int x, int y) { return 7 * x + y; }
template <int A, int B> constexpr Tag<7 * A + B> operator⊚(Tag<A>, Tag<B>) { return {}; }
static_assert(1 ⊞ 2 ⊚ 3 ⊗ 4 == 97);   // ((1 ⊞ 2) ⊚ 3) ⊗ 4 == 3*31 + 4
static_assert(__is_same(decltype(a ⊞ b ⊚ c ⊗ d), decltype(((a ⊞ b) ⊚ c) ⊗ d)));

// 3. Tighter than every built-in binary operator.
static_assert(2 * 3 ⊞ 4 == 20);       // 2 * (3 ⊞ 4)
static_assert(2 ⊞ 3 * 4 == 28);       // (2 ⊞ 3) * 4
static_assert(12 / 3 ⊞ 4 == 1);       // 12 / (3 ⊞ 4) == 12 / 10
static_assert(13 % 3 ⊞ 4 == 3);       // 13 % 10
static_assert(1 + 2 ⊞ 3 == 8);        // 1 + (2 ⊞ 3)
static_assert(10 - 2 ⊞ 3 == 3);       // 10 - (2 ⊞ 3)
static_assert((1 << 1 ⊞ 2 ) == 16);   // 1 << (1 ⊞ 2) == 1 << 4
static_assert((256 >> 1 ⊞ 2) == 16);  // 256 >> 4
static_assert((1 ⊞ 2 < 5) == true);   // (1 ⊞ 2) < 5
static_assert((1 ⊞ 2 > 5) == false);
static_assert(2 ⊞ 3 == 7);            // (2 ⊞ 3) == 7
static_assert((12 & 1 ⊞ 2) == 4);     // 12 & (1 ⊞ 2)
static_assert((12 ^ 1 ⊞ 2) == 8);
static_assert((8 | 1 ⊞ 2) == 12);
constexpr bool land(int x, int y) { return x ⊞ y && y; }
constexpr bool lor(int x, int y) { return x ⊞ y || y; }
static_assert(land(1, 0) == false);   // (1 ⊞ 0) && 0
static_assert(lor(0, 0) == false);    // (0 ⊞ 0) || 0
static_assert(lor(1, 0) == true);     // (1 ⊞ 0) || 0 == 2 || 0

// 4. Looser than unary: both operands are cast-expressions.
static_assert(-1 ⊞ -2 == -4);
static_assert(-1 ⊞ 2 == 0);
static_assert(1 ⊞ -2 == 0);
static_assert(+1 ⊞ +2 == 4);
static_assert(!0 ⊞ 1 == 3);
static_assert(~0 ⊞ 1 == -1);
static_assert(sizeof(int) ⊞ 0 == 8);
static_assert(__is_same(decltype(-1 ⊞ -2), decltype((-1) ⊞ (-2))));

// 5. A prefix user operator binds tighter than any binary.
static_assert(⊖1 ⊞ 2 == 10);          // operator⊞(⊖1, 2) == 2*4 + 2
static_assert(1 ⊞ ⊖2 == 9);           // operator⊞(1, ⊖2) == 2 + 7
static_assert(⊖1 ⊞ ⊖2 == 15);
static_assert(__is_same(decltype(⊖a ⊞ b), decltype((⊖a) ⊞ b)));
static_assert(__is_same(decltype(a ⊞ ⊖b), decltype(a ⊞ (⊖b))));
// Three binding strengths in one expression: ⊖a ⊞ 2 * ⊖b is
// ((⊖a) ⊞ 2) * (⊖b) -- the '*' is the outermost operator.
static_assert(⊖1 ⊞ 2 * ⊖2 == 70);     // ((3*1+1) ⊞ 2) * (3*2+1) == 10 * 7
static_assert(⊖1 ⊞ (2 * ⊖2) == 22);   // the reading most people expect: 2*4 + 14
static_assert(⊖2 * 3 ⊞ 4 == 7 * 10);  // (⊖2) * (3 ⊞ 4)
static_assert(⊖⊖1 ⊞ 2 == 28);         // 2*13 + 2

// 6. Relation to the levels below.
static_assert((1 ⊞ 2 == 2 ⊞ 2) == false);   // 4 == 6
static_assert((1 ⊞ 2 == 1 ⊞ 2) == true);
constexpr int assigns() {
  int x = 0, y = 0;
  x = 1 ⊞ 2 ⊞ 3;       // x = ((1 ⊞ 2) ⊞ 3)
  y = x ⊞ 1;
  x += 1 ⊞ 0;          // compound assignment
  return 100 * x + y;
}
static_assert(assigns() == 1323);   // x == 13, y == 23
constexpr int assign_chain() {
  int x = 0, y = 0;
  x = y = 1 ⊞ 2;       // x = (y = (1 ⊞ 2))
  return x + y;
}
static_assert(assign_chain() == 8);
constexpr int comma_op() {
  int x = 0;
  int y = (x = 1 ⊞ 2, 3 ⊞ 4);   // both sides are complete chains
  return 100 * y + x;
}
static_assert(comma_op() == 1004);  // y == 10, x == 4
static_assert((1 ⊞ 2 ? 3 ⊞ 4 : 5 ⊞ 6) == 10);
static_assert((0 ⊞ 0 ? 3 ⊞ 4 : 5 ⊞ 6) == 16);
static_assert((true ? 1 : 2) ⊞ 3 == 5);
static_assert(__is_same(decltype((true ? a : a) ⊞ b), Tag<4>));

// 7. Postfix and cast operands.
constexpr int v1 = 3, v2 = 4;
constexpr const int *p = &v1, *q = &v2;
static_assert(*p ⊞ *q == 10);                 // unary '*' on both operands
struct S {
  int m;
  constexpr int mf() const { return m + 1; }
};
constexpr S s1{3}, s2{4};
constexpr const S *ps = &s1;
static_assert(s1.m ⊞ s2.m == 10);             // member access
static_assert(ps->m ⊞ s2.m == 10);            // arrow
static_assert(s1.mf() ⊞ s2.mf() == 13);       // member call
constexpr int g(int x) { return x + 1; }
static_assert(g(2) ⊞ g(3) == 10);             // call
constexpr int arr[] = {1, 2, 3};
static_assert(arr[1] ⊞ arr[2] == 7);          // subscript
static_assert((int)1.9 ⊞ (int)2.9 == 4);      // C-style cast
static_assert(static_cast<int>(1.9) ⊞ 2 == 4);// named cast
constexpr int incr() {
  int i = 3, j = 4;
  int r = i++ ⊞ j++;      // (i++) ⊞ (j++) == 2*3 + 4
  return r + i + j;
}
static_assert(incr() == 19);

// 8. Parentheses regroup, everywhere.
static_assert(__is_same(decltype(a ⊞ (b ⊞ c)), Tag<9>));      // vs Tag<11>
static_assert(__is_same(decltype(a ⊞ (b ⊗ c)), Tag<11>));     // vs Tag<15>
static_assert(__is_same(decltype(a ⊗ (b ⊞ c)), Tag<10>));     // vs Tag<13>
static_assert(__is_same(decltype(a ⊞ (b ⊗ c ⊞ d)), Tag<24>));
static_assert(⊖(1 ⊞ 2) == 13);                                // vs ⊖1 ⊞ 2 == 10
static_assert((2 * 3) ⊞ 4 == 16);                             // vs 2 * 3 ⊞ 4 == 20
static_assert(2 * (3 ⊞ 4) == 20);
static_assert(-(1 ⊞ -2) == 0);                                // vs -1 ⊞ -2 == -4

// 9. A class-typed result with a non-trivial destructor, so the call's result
// is bound to a temporary: the grouping is still a decltype away, and prints.
struct Res {
  int v;
  ~Res();
};
Res operator⊢(Res, Res);
Res operator⊢(Res);
using ChainType = decltype(Res{1} ⊢ Res{2} ⊢ Res{3});
using PrefixChainType = decltype(⊢⊢Res{1} ⊢ Res{2});
static_assert(__is_same(ChainType, Res));
static_assert(__is_same(PrefixChainType, Res));
