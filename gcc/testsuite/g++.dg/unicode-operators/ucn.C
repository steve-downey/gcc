// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// A universal-character-name designating an operator code point is that
// operator token (unicode-operators.md, ucn-spellings), and the equivalence is
// one of *entities*, not of token streams: a declaration spelled one way and a
// definition spelled another are one function with one symbol, and a use
// spelled a third way calls it.  Each case is a constant evaluation, so a
// wrong answer is a wrong number and not merely a missing diagnostic.

// (a) Declared with the glyph, called with each UCN form.
constexpr int operator⊦(int a, int b) { return 10 * a + b; }
static_assert(operator⊦(1, 2) == 12);
static_assert(operator\U000022A6(1, 2) == 12);
static_assert(operator\u{22A6}(1, 2) == 12);
static_assert(operator\N{ASSERTION}(1, 2) == 12);

// (b) Declared with a named UCN, called with the glyph.  The declaration and
// the definition are also spelled differently from each other, so this is
// simultaneously a redeclaration-merge assertion.
constexpr int operator\N{MODELS}(int a, int b);
constexpr int operator⊧(int a, int b) { return 100 * a + b; }
static_assert(operator⊧(1, 2) == 102);

// (c) The overload set is one set however its members are spelled.
constexpr int operator\N{ASSERTION}(double, double) { return 7; }
static_assert(operator⊦(1.0, 2.0) == 7);
static_assert(operator⊦(1, 2) == 12);

// (d) A UCN-spelled operator-function-id names the overload set in every
// position an unqualified-id does.
constexpr int (*pfn_ucn)(int, int) = &operator\N{ASSERTION};
constexpr int (*pfn_glyph)(int, int) = &operator⊦;
static_assert(pfn_ucn == pfn_glyph);
static_assert(pfn_ucn(3, 4) == 34);

// (e) The operator token itself may be a UCN, in infix and prefix position,
// with and without surrounding whitespace, and chains like the glyph does.
constexpr int operator⊖(int a) { return 3 * a + 1; }
static_assert(1 ⊦ 2 == 12);
static_assert(1⊦ 2 == 12);
static_assert(1 \N{ASSERTION} 2 == 12);
static_assert(⊖ 2 == 7);
static_assert(⊖⊖ 1 == 13);
static_assert(1 ⊦ 2 \U000022A6 3 == 123);   // left-associative: (12) ⊦ 3

// (f) A UCN at the start of an identifier that designates an operator code
// point does not get absorbed into the identifier: a⊦b is three tokens.
constexpr int a = 1, b = 2;
static_assert(a⊦b == 12);
