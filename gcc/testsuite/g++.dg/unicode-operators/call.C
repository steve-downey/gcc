// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// The name operator⊞ behaves as an ordinary function name in every position an
// unqualified-id can appear (unicode-operators.md, operator-function-id):
// lookup, overload sets, ADL, templates, SFINAE, constraints, address-taking.
// Nothing is reimplemented for this; the operator-function-id hands the rest
// of the front end a name it already knows how to handle.

// 1. The fundamental-only call, end to end.
constexpr int operator⊞(int a, int b) { return a + b; }
constexpr int operator⊖(int a) { return -a; }
static_assert(operator⊞(5, 7) == 12);
static_assert(operator⊖(5) == -5);
static_assert(operator⊞(operator⊞(1, 2), operator⊖(3)) == 0);
int runtime(int a, int b) { return operator⊞(a, b); }
static_assert(__is_same(decltype(operator⊞(1, 2)), int));

// 2. Overload sets and ordinary ranking.
struct Base {};
struct Derived : Base {};
constexpr int operator⊠(int, int) { return 1; }
constexpr int operator⊠(double, double) { return 2; }
constexpr int operator⊠(Base, Base) { return 3; }
constexpr int operator⊠(Derived, Derived) { return 4; }
static_assert(operator⊠(1, 2) == 1);            // exact match
static_assert(operator⊠(1.0, 2.0) == 2);        // exact match
static_assert(operator⊠(1.0f, 2.0f) == 2);      // float -> double promotion
static_assert(operator⊠(Base{}, Base{}) == 3);
static_assert(operator⊠(Derived{}, Derived{}) == 4);  // beats the Base conversion

// 3. Qualified and member calls.
namespace NS {
struct Q {};
constexpr int operator⊞(Q, Q) { return 21; }
constexpr int operator⊖(Q) { return 22; }
}
constexpr int qualified() {
  NS::Q q;
  return NS::operator⊞(q, q) + NS::operator⊖(q);
}
static_assert(qualified() == 43);

struct Mem {
  constexpr int operator⊞(Mem) const { return 31; }  // member infix
  constexpr int operator⊖() const { return 32; }     // member prefix
  // An unqualified explicit call inside the class finds the member through
  // the implicit object argument, exactly as operator+ would.
  constexpr int inside(Mem o) const { return operator⊞(o); }
};
constexpr int member_calls() {
  Mem m;
  Mem *p = &m;
  return m.operator⊞(m)         // object expression
       + p->operator⊞(m)        // through a pointer
       + m.Mem::operator⊞(m)    // qualified member call
       + m.operator⊖();         // member prefix form
}
static_assert(member_calls() == 31 * 3 + 32);
static_assert(Mem{}.inside(Mem{}) == 31);

// 4. ADL -- the normative case.
namespace Adl {
struct A {};
enum E { e1 };
struct Ptr {};
constexpr int operator⊘(A, A) { return 41; }
constexpr int operator⊙(A) { return 42; }      // prefix form
constexpr int operator⊘(E, E) { return 43; }
constexpr int operator⊚(Ptr *, Ptr *) { return 44; }
struct Hidden {
  friend constexpr int operator⊛(Hidden, Hidden) { return 45; }
};
}
static_assert(operator⊘(Adl::A{}, Adl::A{}) == 41);
static_assert(operator⊙(Adl::A{}) == 42);         // ADL for the prefix form too
static_assert(operator⊘(Adl::e1, Adl::e1) == 43); // enumeration's namespace
static_assert(operator⊛(Adl::Hidden{}, Adl::Hidden{}) == 45);  // hidden friend
constexpr int adl_pointer() {
  Adl::Ptr p;
  return operator⊚(&p, &p);  // pointee's namespace is associated
}
static_assert(adl_pointer() == 44);

template <class T> struct Wrap {};
namespace Adl2 {
struct B {};
constexpr int operator⊜(Wrap<B>, Wrap<B>) { return 46; }
}
static_assert(operator⊜(Wrap<Adl2::B>{}, Wrap<Adl2::B>{}) == 46);

// ADL augments the ordinary-lookup set rather than being masked by it.
struct Local {};
constexpr int operator⊝(Local, Local) { return 47; }
namespace Adl3 {
struct C {};
constexpr int operator⊝(C, C) { return 48; }
}
static_assert(operator⊝(Local{}, Local{}) == 47);
static_assert(operator⊝(Adl3::C{}, Adl3::C{}) == 48);

namespace Adl4 {
constexpr int operator⊟(int, int) { return 49; }
}
using Adl4::operator⊟;
static_assert(operator⊟(1, 2) == 49);

// Ordinary lookup finding a class member suppresses ADL, as always.
struct Suppress {
  constexpr int operator⊘(Adl::A) const { return 51; }
  constexpr int m(Adl::A a) const { return operator⊘(a); }
};
static_assert(Suppress{}.m(Adl::A{}) == 51);

// 5. Templates, SFINAE, constraints.
template <class T> constexpr T operator⊤(T a, T b) { return a + b; }
static_assert(operator⊤(2, 3) == 5);
static_assert(operator⊤<int>(2, 3) == 5);
static_assert(operator⊤(2.5, 0.5) == 3.0);

template <class T> struct Box {
  T v;
  constexpr T operator⊥(Box b) const { return v + b.v; }  // member infix
  constexpr T operator⊨() const { return -v; }            // member prefix
};
static_assert(Box<int>{3}.operator⊥(Box<int>{4}) == 7);
static_assert(Box<int>{3}.operator⊨() == -3);

namespace Dep {
struct E {};
constexpr int operator⋀(E, E) { return 61; }
constexpr int operator⋁(E) { return 62; }
}
template <class T> constexpr auto dependent_infix(T a, T b) { return operator⋀(a, b); }
template <class T> constexpr auto dependent_prefix(T a) { return operator⋁(a); }
template <class T> constexpr auto dependent_member(T a, T b) { return a.operator⊥(b); }
template <class T> constexpr auto dependent_decltype(T a, T b) -> decltype(operator⋀(a, b)) {
  return operator⋀(a, b);
}
static_assert(dependent_infix(Dep::E{}, Dep::E{}) == 61);
static_assert(dependent_prefix(Dep::E{}) == 62);
static_assert(dependent_member(Box<int>{1}, Box<int>{2}) == 3);
static_assert(dependent_decltype(Dep::E{}, Dep::E{}) == 61);

template <class T> constexpr bool has_op(int, decltype(operator⋀(T{}, T{})) * = nullptr) {
  return true;
}
template <class T> constexpr bool has_op(...) { return false; }
static_assert(has_op<Dep::E>(0));
static_assert(!has_op<Base>(0));

template <class T> concept Combinable = requires(T a, T b) { operator⋀(a, b); };
static_assert(Combinable<Dep::E>);
static_assert(!Combinable<Base>);

template <class T>
  requires Combinable<T>
constexpr int constrained(T a, T b) { return operator⋀(a, b); }
static_assert(constrained(Dep::E{}, Dep::E{}) == 61);

// 6. Address-taking, and the operator-function-id as a name.
struct Addr {};
constexpr int operator⊩(Addr, Addr) { return 71; }
constexpr int operator⊪(Addr) { return 72; }
constexpr int (*fp_amp)(Addr, Addr) = &operator⊩;
constexpr int (*fp_bare)(Addr, Addr) = operator⊩;  // without &
constexpr auto fp_prefix = &operator⊪;
static_assert(fp_amp(Addr{}, Addr{}) == 71);
static_assert(fp_bare(Addr{}, Addr{}) == 71);
static_assert(fp_prefix(Addr{}) == 72);
static_assert(__is_same(decltype(&operator⊪), int (*)(Addr)));

template <int (*F)(Addr, Addr)> struct Holder {
  static constexpr int call() { return F(Addr{}, Addr{}); }
};
static_assert(Holder<&operator⊩>::call() == 71);

constexpr int apply(int (*f)(Addr, Addr)) { return f(Addr{}, Addr{}); }
static_assert(apply(&operator⊩) == 71);
static_assert(apply(operator⊩) == 71);

constexpr int operator⊫(int, int) { return 73; }
constexpr double operator⊫(double, double) { return 74; }
constexpr int (*fp_overloaded)(int, int) = &operator⊫;  // target type selects
static_assert(fp_overloaded(0, 0) == 73);

struct PM {
  constexpr int operator⊬(PM) const { return 75; }
};
constexpr int (PM::*pmf)(PM) const = &PM::operator⊬;
constexpr int through_pmf() {
  PM p;
  return (p.*pmf)(p);
}
static_assert(through_pmf() == 75);

// 7. A member operator template is callable with explicit template arguments
// when the object expression is non-dependent, with or without the
// disambiguator.
struct TmplMem {
  template <class T> constexpr int operator⊭(T) const { return 81; }
};
static_assert(TmplMem{}.operator⊭<int>(0) == 81);
static_assert(TmplMem{}.template operator⊭<int>(0) == 81);
