// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Prefix parse of Unicode user-defined operators (unicode-operators.md,
// unary-forms): a user operator in operand position is the prefix form,
// binding like the other unary operators -- tighter than any binary operator,
// the user-infix level included.  The claim is not that ⊖a parses; it is that
// prefix and infix are told apart by grammatical position alone: no
// lookahead, no whitespace rule, no consultation of what has been declared.
// Section 3 uses the same code point both ways in one expression.

// ⊖ is declared BOTH ways, deliberately: one parameter is the prefix form and
// two the infix form, and both declarations are visible everywhere below.
// Nothing in the parser looks at either of them.
constexpr int operator⊖(int a) { return 3 * a + 1; }
constexpr int operator⊖(int a, int b) { return 100 * a + b; }

// An infix-only operator, to mix with.
constexpr int operator⊞(int a, int b) { return 2 * a + b; }

// 1. The prefix form parses, and stacks.
static_assert(⊖1 == 4);
static_assert(⊖⊖1 == 13);   // 3*(3*1 + 1) + 1
static_assert(⊖⊖⊖1 == 40);  // 3*13 + 1

// 2. It binds like the other unary operators.
// Tighter than the user-infix level itself. If it were looser these would be
// ⊖(1 ⊞ 2) == 13 and 2*1 + ⊖(2 ⊞ 3) == 24 respectively.
static_assert(⊖1 ⊞ 2 == 10);   // operator⊞(⊖1, 2) == 2*4 + 2
static_assert(1 ⊞ ⊖2 == 9);    // operator⊞(1, ⊖2) == 2*1 + 7
static_assert(⊖1 ⊞ ⊖2 == 15);  // 2*4 + 7
// Tighter than '*'.  If the prefix form bound looser than '*' this would be
// ⊖(2 * 3) == 19.
static_assert(⊖2 * 3 == 21);
static_assert(2 * ⊖3 == 20);
// Parentheses regroup.
static_assert(⊖(1 ⊞ 2) == 13);
// Interleaved with the built-in unary operators, either order.
static_assert(⊖-1 == -2);  // 3*(-1) + 1
static_assert(-⊖1 == -4);
static_assert(⊖!0 == 4);
static_assert(⊖~0 == -2);
static_assert(⊖sizeof(int) == 13);
// The operand is a cast-expression, so an explicit cast and every postfix
// suffix bind tighter than the operator.
static_assert(⊖(int)1.9 == 4);
constexpr int arr[] = {1, 2, 3};
static_assert(⊖arr[2] == 10);
constexpr int twice(int a) { return 2 * a; }
static_assert(⊖twice(3) == 19);
struct Field { int v; };
constexpr Field fld{5};
static_assert(⊖fld.v == 16);

// 3. The acceptance test: one code point, both forms, one expression.
// ⊖a ⊖ b is operator⊖(operator⊖(a), b): the first ⊖ opens the expression, so
// it is in operand position and is prefix; the second follows a complete
// operand, so it is infix.
static_assert(⊖1 ⊖ 2 == 402);       // 100*(3*1 + 1) + 2
static_assert(1 ⊖ ⊖2 == 107);       // 100*1 + (3*2 + 1)
static_assert(⊖1 ⊖ ⊖2 == 407);      // 100*4 + 7
static_assert(⊖⊖1 ⊖ ⊖⊖2 == 1322);   // 100*13 + 22
// The infix form is still left-associative when prefix uses are mixed in.
// Right-associative would be 100*4 + (100*7 + 3) == 1103.
static_assert(⊖1 ⊖ ⊖2 ⊖ 3 == 40703);   // 100*(100*4 + 7) + 3
// Both spellings of the level chain together, left to right, with the prefix
// form still bound tighter than either: ((⊖1) ⊖ 2) ⊞ 3 == 2*402 + 3.
static_assert(⊖1 ⊖ 2 ⊞ 3 == 807);

// 4. An ordinary operand: the prefix form composes with everything else.
constexpr int slots() {
  int x = ⊖1;      // assignment slot
  x += ⊖1;         // compound assignment
  return x;
}
static_assert(slots() == 8);
constexpr int list[] = {⊖1, ⊖2};
static_assert(list[0] == 4 && list[1] == 7);
static_assert(twice(⊖1) == 8);            // argument position
static_assert((true ? ⊖1 : ⊖2) == 4);     // conditional
template <int N> struct Box { static constexpr int value = N; };
static_assert(Box<(⊖1)>::value == 4);     // template-argument position

// 5. Member candidates, ADL, and templates.
// A member prefix operator is declared with no parameters: the arity rule
// counts the implicit object parameter.
struct Mem {
  int v;
  constexpr int operator⊕() const { return v + 9; }
};
static_assert(⊕Mem{1} == 10);

// ADL reaches an operator declared only in the operand's namespace, and a
// hidden friend reachable by nothing else.
namespace Adl {
struct A { int v; };
constexpr int operator⊘(A x) { return x.v + 100; }
struct H {
  friend constexpr int operator⊗(H) { return 45; }
};
}
static_assert(⊘Adl::A{1} == 101);
static_assert(⊗Adl::H{} == 45);

// A dependent operand: the use is rebuilt at instantiation as an operator, so
// the member candidate -- a property of the operator syntax, not of the call
// -- survives.
template <class T> constexpr int applied(T a) { return ⊕a; }
static_assert(applied(Mem{2}) == 11);
template <class T> constexpr int applied_adl(T a) { return ⊘a; }
static_assert(applied_adl(Adl::A{3}) == 103);
template <class T> constexpr int applied_twice(T a) { return ⊖⊖a; }
static_assert(applied_twice(1) == 13);

template <class T> concept Negatable = requires(T a) { ⊕a; };
static_assert(Negatable<Mem>);
static_assert(!Negatable<int>);

// 6. The default-argument corner.  [over.oper]p8 is waived for a user
// operator, so a two-parameter operator whose second parameter has a default
// argument is one-argument-viable and usable in prefix position: ⊟5 is
// operator⊟(5), which is what the desugaring promises.
constexpr int operator⊟(int a, int b = 1) { return 7 * a + b; }
static_assert(⊟5 == 36);            // prefix use of a two-parameter operator
static_assert(2 ⊟ 3 == 17);         // and the infix use is unaffected
static_assert(operator⊟(5) == 36);  // the explicit call finds the same thing
