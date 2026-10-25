// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Candidate assembly for the operator syntax (unicode-operators.md,
// candidate-assembly): member candidates, non-member candidates from
// unqualified lookup and ADL, ranked as one set, and no built-in candidates.
// For every case x ⊞ y selects the same overload as operator⊞(x, y), asserted
// as type identity over a tag-returning overload set, so "same overload" is
// checkable by __is_same and not merely by two compilations both succeeding.

template <int N> struct Tag { static constexpr int value = N; };

// 1. The motivating line, and the equivalence in miniature.
constexpr int operator⊞(int a, int b) { return a + b; }
static_assert(5 ⊞ 7 == 12);
static_assert(operator⊞(5, 7) == 12);
static_assert(__is_same(decltype(5 ⊞ 7), decltype(operator⊞(5, 7))));
static_assert(__is_same(decltype(5 ⊞ 7), int));

// 2. ADL -- the normative case.
namespace Adl {
struct A {};
enum E { e1 };
struct Ptr {};
constexpr Tag<41> operator⊘(A, A) { return {}; }
constexpr Tag<43> operator⊘(E, E) { return {}; }
constexpr Tag<44> operator⊚(Ptr *, Ptr *) { return {}; }
// A hidden friend: reachable by ADL and by nothing else at all.
struct Hidden {
  friend constexpr Tag<45> operator⊛(Hidden, Hidden) { return {}; }
};
}
// Nothing named operator⊘ / operator⊚ / operator⊛ is visible here by ordinary
// unqualified lookup.
static_assert((Adl::A{} ⊘ Adl::A{}).value == 41);
static_assert(__is_same(decltype(Adl::A{} ⊘ Adl::A{}),
                        decltype(operator⊘(Adl::A{}, Adl::A{}))));
static_assert((Adl::e1 ⊘ Adl::e1).value == 43);  // enumeration's namespace
static_assert((Adl::Hidden{} ⊛ Adl::Hidden{}).value == 45);  // hidden friend
constexpr auto adl_pointer() {
  Adl::Ptr p;
  return &p ⊚ &p;  // pointee's namespace is associated
}
static_assert(adl_pointer().value == 44);

// A class template's argument contributes its namespace, as for any call.
template <class T> struct Wrap {};
namespace Adl2 {
struct B {};
constexpr Tag<46> operator⊜(Wrap<B>, Wrap<B>) { return {}; }
}
static_assert((Wrap<Adl2::B>{} ⊜ Wrap<Adl2::B>{}).value == 46);

// ADL augments the ordinary-lookup set rather than being masked by it: a
// visible, non-viable ordinary-lookup candidate does not stop the ADL one from
// being found.  (This is the shape the backtick track's gcc-slot-adl defect
// got wrong.)
struct Local {};
constexpr Tag<47> operator⊝(Local, Local) { return {}; }
namespace Adl3 {
struct C {};
constexpr Tag<48> operator⊝(C, C) { return {}; }
}
static_assert((Local{} ⊝ Local{}).value == 47);
static_assert((Adl3::C{} ⊝ Adl3::C{}).value == 48);

// A using-declaration brings the name in by ordinary lookup.
namespace Adl4 {
constexpr Tag<49> operator⊨(int, int) { return {}; }
}
using Adl4::operator⊨;
static_assert((1 ⊨ 2).value == 49);

// The member IS found by the operator syntax, because D is the left operand's
// class, even though ADL finds no members for the explicit call (call.C).
namespace Adl5 {
struct D {
  constexpr int operator⊡(D) const { return 51; }
};
}
static_assert((Adl5::D{} ⊡ Adl5::D{}) == 51);

// 3. Member candidates.
struct Mem {
  int v;
  // Non-commutative on purpose: a wrong operand order is a wrong value.
  constexpr int operator⊕(Mem o) const { return v * 10 + o.v; }
};
static_assert((Mem{1} ⊕ Mem{2}) == 12);
// The equivalence for the member form: x ⊕ y is x.operator⊕(y).
static_assert(__is_same(decltype(Mem{1} ⊕ Mem{2}),
                        decltype(Mem{1}.operator⊕(Mem{2}))));
static_assert((Mem{1} ⊕ Mem{2}) == Mem{1}.operator⊕(Mem{2}));

// Inherited members are found: the lookup is into the left operand's class
// and its bases, exactly as [over.match.oper]p3 says.
struct MemBase {
  constexpr Tag<52> operator⊗(int) const { return {}; }
};
struct MemDerived : MemBase {};
static_assert((MemDerived{} ⊗ 0).value == 52);

// Member and non-member candidates are ranked in ONE set, not in two passes:
// each of these picks the other kind of candidate.
struct MN {
  constexpr Tag<53> operator⊘(long) const { return {}; }
};
constexpr Tag<54> operator⊘(MN, int) { return {}; }
static_assert((MN{} ⊘ 0).value == 54);   // non-member exact beats member conversion
static_assert((MN{} ⊘ 0L).value == 53);  // member exact beats non-member conversion

// A member candidate does not suppress ADL either.
namespace Adl6 {
struct F {
  constexpr Tag<55> operator⊜(F) const { return {}; }
};
constexpr Tag<56> operator⊜(F, int) { return {}; }
}
static_assert((Adl6::F{} ⊜ Adl6::F{}).value == 55);  // member
static_assert((Adl6::F{} ⊜ 1).value == 56);          // ADL, with members present

// Object cv- and ref-qualification select among member candidates as usual.
struct RQ {
  constexpr Tag<57> operator⊞(int) const & { return {}; }
  constexpr Tag<58> operator⊞(int) && { return {}; }
};
constexpr RQ rq{};
static_assert((rq ⊞ 0).value == 57);
static_assert((RQ{} ⊞ 0).value == 58);

// An explicit object parameter works, and so does a member template.
struct EO {
  constexpr Tag<59> operator⊗(this EO, int) { return {}; }
};
static_assert((EO{} ⊗ 0).value == 59);
struct TM {
  template <class T> constexpr Tag<60> operator⊤(T) const { return {}; }
};
static_assert((TM{} ⊤ 1).value == 60);

// A member of a class template.
template <class T> struct Boxed {
  T v;
  constexpr T operator⊥(Boxed b) const { return v * 10 + b.v; }
};
static_assert((Boxed<int>{3} ⊥ Boxed<int>{4}) == 34);

// 4. Dependent operands -- two-phase behavior, inherited.
namespace Dep {
struct E2 {};
constexpr Tag<61> operator⋀(E2, E2) { return {}; }
}
// Nothing named operator⋀ is visible where the template is defined; the
// candidate is found by ADL at instantiation.
template <class T> constexpr auto dependent_infix(T a, T b) { return a ⋀ b; }
static_assert(dependent_infix(Dep::E2{}, Dep::E2{}).value == 61);
static_assert(__is_same(decltype(dependent_infix(Dep::E2{}, Dep::E2{})),
                        decltype(operator⋀(Dep::E2{}, Dep::E2{}))));

template <class T> concept Combinable = requires(T a, T b) { a ⋀ b; };
static_assert(Combinable<Dep::E2>);
static_assert(!Combinable<int>);

template <class T>
  requires Combinable<T>
constexpr auto constrained(T a, T b) { return a ⋀ b; }
static_assert(constrained(Dep::E2{}, Dep::E2{}).value == 61);

// SFINAE: an unusable operator removes the candidate rather than erroring.
struct Base {};
template <class T>
constexpr bool has_op(int, decltype(T{} ⋀ T{}) * = nullptr) { return true; }
template <class T> constexpr bool has_op(...) { return false; }
static_assert(has_op<Dep::E2>(0));
static_assert(!has_op<Base>(0));

// A dependent operand with a member operator: the dependent expression is
// kept as the operator, not as the call it desugars to, so the member
// candidates are assembled again at instantiation.
template <class T> constexpr auto dependent_member(T a, T b) { return a ⊕ b; }
static_assert(dependent_member(Mem{1}, Mem{2}) == 12);

template <class T> concept MemberCombinable = requires(T a, T b) { a ⊕ b; };
static_assert(MemberCombinable<Mem>);
static_assert(!MemberCombinable<int>);

// Member and non-member candidates are still ranked in one set when the
// operands are dependent, both ways round.
template <class T, class U> constexpr auto dependent_ranked(T a, U b) {
  return a ⊘ b;
}
static_assert(dependent_ranked(MN{}, 0).value == 54);   // non-member
static_assert(dependent_ranked(MN{}, 0L).value == 53);  // member

// Two-phase lookup: a non-member operator visible at the definition is found
// at instantiation even for operands that associate no namespace of their
// own, and one declared only later, in no associated namespace, is not.
namespace TwoPhase {
struct X {};
}
constexpr Tag<62> operator⋁(TwoPhase::X, int) { return {}; }
template <class T> constexpr auto two_phase(T a, int b) { return a ⋁ b; }
static_assert(two_phase(TwoPhase::X{}, 0).value == 62);
