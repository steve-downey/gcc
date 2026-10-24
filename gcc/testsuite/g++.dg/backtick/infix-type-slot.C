// { dg-do compile }
// { dg-options "-fbacktick -std=c++20" }
// gcc-type-slot-parity: a type-name in the slot is construction,
// x `T` y == T(x, y), functional-style, CTAD applying (design doc §17.3).
// The type reading wins exactly when lookup finds a type or a class
// template.  The cases are those of Clang's Parser/backtick-infix.cpp.

template<class A, class B> struct same { static constexpr bool value = false; };
template<class A> struct same<A, A> { static constexpr bool value = true; };

struct Pt {
  int x, y;
  constexpr Pt(int a, int b) : x(a), y(b) {}
  constexpr bool operator==(const Pt&) const = default;
};

// Bare class name.
constexpr Pt type_bare = 1 `Pt` 2;
static_assert(type_bare.x == 1 && type_bare.y == 2);

// Qualified, and globally qualified, class name.
namespace geo { struct Vec { int x, y; constexpr Vec(int a, int b) : x(a), y(b) {} }; }
constexpr geo::Vec type_qual = 1 `geo::Vec` 2;
static_assert(type_qual.y == 2);
constexpr geo::Vec type_global = 3 `::geo::Vec` 4;
static_assert(type_global.x == 3);

// Class template name: CTAD, exactly as pr(1, 2) would deduce.
template<class A, class B> struct pr {
  A first; B second;
  constexpr pr(A a, B b) : first(a), second(b) {}
};
constexpr auto type_ctad = 1 `pr` 2.0;
static_assert(same<decltype(type_ctad), const pr<int, double>>::value);
namespace geo { template<class A, class B> struct qpr { constexpr qpr(A, B) {} }; }
constexpr auto type_qual_ctad = 1 `geo::qpr` 'c';
static_assert(same<decltype(type_qual_ctad), const geo::qpr<int, char>>::value);

// Template-id.
constexpr auto type_tid = 3 `pr<long, long>` 4;
static_assert(same<decltype(type_tid), const pr<long, long>>::value);

// Alias.
using PtAlias = Pt;
constexpr Pt type_alias = 5 `PtAlias` 6;
static_assert(type_alias.x == 5);

// An aggregate: parenthesized aggregate initialization, not a constructor.
struct Agg { int x, y; };
constexpr Agg type_agg = 1 `Agg` 2;
static_assert(type_agg.y == 2);
struct Agg3 { int x, y, z = 7; };
constexpr Agg3 type_agg3 = 1 `Agg3` 2;
static_assert(type_agg3.z == 7);

// Dependent types: a template parameter and a typename-specifier.
template<class T> constexpr T dep_construct(int a, int b) { return a `T` b; }
static_assert(dep_construct<Pt>(3, 4).y == 4);
template<class T> struct W { using type = Pt; };
template<class T> constexpr auto dep_typename(T a) { return a `typename W<T>::type` a; }
static_assert(same<decltype(dep_typename(1)), Pt>::value);
template<class T> struct MkBox { constexpr T make(int a, int b) { return a `T` b; } };
static_assert(MkBox<Agg>{}.make(1, 2).x == 1);

// A name that denotes both a class and a function: ordinary lookup hides the
// class, so the slot is the call both(1, 2) -- the resolution the spelled
// form gets.
struct both { both(int, int); };
long both(int, int);
static_assert(same<decltype(1 `both` 2), long>::value);
static_assert(same<decltype(both(1, 2)), long>::value);

// Statement position: still an expression, no declaration reading.
void stmt_position(int a, int b) { a `Pt` b; }

// Precedence and chaining are the operator's: -a `Pt` -b is Pt(-a, -b), and
// the result feeds the next slot.
static_assert((-1 `Pt` -2) == Pt(-1, -2));
constexpr int sum(Pt p, int k) { return p.x + p.y + k; }
static_assert((1 `Pt` 2 `sum` 3) == 6);

// The right-hand operand of another binary operator takes the slot too.
static_assert(Pt(1, 2) == 1 `Pt` 2);

// A builtin type is a functional cast with two operands, ill-formed exactly
// as int(1, 2) is.
int builtin = 1 `int` 2;	// { dg-error "compound expression in functional cast" }
