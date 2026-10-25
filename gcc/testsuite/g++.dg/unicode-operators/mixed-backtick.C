// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators -fbacktick" }
// One level shared with -fbacktick (unicode-operators.md,
// user-infix-precedence; unicode-feature-gating).  a ⊞ b `f` c and
// a `f` b ⊞ c are the two orders in which the two spellings of the same level
// can meet, and both nest left -- not because the two features were made to
// agree, but because there is only one level for them to agree about.  The
// shapes are asserted as types, so a mixed chain and its left-nested
// parenthesization are the same type and there is nothing left for a second
// precedence level to be.

template <int N> struct Tag { static constexpr int value = N; };

constexpr int operator⊞(int a, int b) { return 2 * a + b; }
constexpr int operator⊗(int a, int b) { return 3 * a + b; }
constexpr int operator⊖(int a) { return 3 * a + 1; }
constexpr int f(int x, int y) { return 5 * x + y; }
template <int A, int B> constexpr Tag<2 * A + B> operator⊞(Tag<A>, Tag<B>) { return {}; }
template <int A, int B> constexpr Tag<3 * A + B> operator⊗(Tag<A>, Tag<B>) { return {}; }
template <int A> constexpr Tag<3 * A + 1> operator⊖(Tag<A>) { return {}; }
template <int A, int B> constexpr Tag<5 * A + B> f(Tag<A>, Tag<B>) { return {}; }

constexpr Tag<1> a{};
constexpr Tag<2> b{};
constexpr Tag<3> c{};
constexpr Tag<4> d{};

// Both orders, by value: right-associative would be 2*1 + f(2,3) == 15 and
// f(1, 2*2+3) == 12 respectively.
static_assert(1 ⊞ 2 `f` 3 == 23);   // f(1 ⊞ 2, 3) == 5*4 + 3
static_assert(1 `f` 2 ⊞ 3 == 17);   // (1 `f` 2) ⊞ 3 == 2*7 + 3

// Four terms, alternating spellings, both interleavings.
static_assert(1 ⊞ 2 `f` 3 ⊞ 4 == 50);
static_assert(1 `f` 2 ⊗ 3 `f` 4 == 124);

// Three spellings, three binding strengths, one expression: a prefix Unicode
// operator, a Unicode infix operator and a backtick infix operator.
// f((⊖1) ⊞ 2, 3) == 5*10 + 3.
static_assert(⊖1 ⊞ 2 `f` 3 == 53);
static_assert(⊖2 `f` 3 == 38);      // f(⊖2, 3) == 5*7 + 3
static_assert(1 `f` ⊖2 == 12);      // f(1, ⊖2) == 5 + 7
static_assert(⊖1 `f` ⊖2 == 27);     // f(4, 7)

// The same claims as shapes.
static_assert(__is_same(decltype(a ⊞ b `f` c), decltype((a ⊞ b) `f` c)));
static_assert(__is_same(decltype(a `f` b ⊞ c), decltype((a `f` b) ⊞ c)));
static_assert(__is_same(decltype(a ⊞ b `f` c), Tag<23>));
static_assert(__is_same(decltype(a `f` b ⊞ c), Tag<17>));
static_assert(__is_same(decltype(a ⊞ b `f` c ⊞ d), decltype(((a ⊞ b) `f` c) ⊞ d)));
static_assert(__is_same(decltype(a `f` b ⊗ c `f` d), decltype(((a `f` b) ⊗ c) `f` d)));
static_assert(__is_same(decltype(⊖a ⊞ b `f` c), decltype(((⊖a) ⊞ b) `f` c)));
static_assert(__is_same(decltype(⊖a ⊞ b `f` c), Tag<53>));

// The shared level is tighter than '*' whichever spelling occupies it.
static_assert(2 * 3 `f` 4 ⊞ 5 == 86);   // 2 * ((3 `f` 4) ⊞ 5)

// A user operator's overload set as the backtick slot: the slot is an
// assignment-expression and an operator-function-id is an id-expression, so
// 1 `operator⊟` 2 is a well-formed backtick use whose callee happens to be a
// Unicode operator -- and it is the same call the Unicode syntax makes.
constexpr int operator⊟(int x, int y) { return 41 * x + y; }
static_assert((1 `operator⊟` 2) == 43);
static_assert((1 ⊟ 2) == 43);
static_assert((1 `operator⊟` 2) == (1 ⊟ 2));

// Dependent operands: the mixed chain is rebuilt the same way at
// instantiation.
template <class T> constexpr auto mixed(T x, T y, T z) { return x ⊞ y `f` z; }
static_assert(mixed(1, 2, 3) == 23);
template <class T> constexpr auto mixed2(T x, T y, T z) { return x `f` y ⊞ z; }
static_assert(mixed2(1, 2, 3) == 17);
