// { dg-do compile { target { c++23 && { x86_64-*-linux* } } } }
// { dg-options "-funicode-operators -fno-exceptions" }
// A use with a dependent operand is kept as the operator -- a
// USER_OPERATOR_EXPR -- rather than as the call it desugars to, so that it is
// rebuilt as an operator at instantiation (unicode-operators.md section 7,
// "Desugaring").  This file exercises the places such a node has to reach:
// a late-specified return type and its mangling, decltype, SFINAE, a
// constraint, a dependent prefix use, and a non-dependent use inside a
// template that is resolved once and re-checked at instantiation.

struct S { int v; };
constexpr S operator⊞(S a, S b) { return {10 * a.v + b.v}; }
constexpr S operator⊖(S a) { return {-a.v}; }
constexpr int operator⊞(int a, int b) { return 10 * a + b; }

// Dependent operands in a trailing return type: the expression is part of
// the signature and mangles as the vendor extended operator applied to the
// parameters, v28op_u229E fp_ fp0_, then the body uses the same expression.
template <class T> auto f(T a, T b) -> decltype(a ⊞ b) { return a ⊞ b; }
template <class T> auto g(T a) -> decltype(⊖a) { return ⊖a; }
int use_f(S a, S b) { return f(a, b).v + g(a).v + f(1, 2); }
// { dg-final { scan-assembler "_Z1fI1SEDTv28op_u229Efp_fp0_ET_S2_" } }
// { dg-final { scan-assembler "_Z1gI1SEDTv18op_u2296fp_ET_" } }
// { dg-final { scan-assembler "_Z1fIiEDTv28op_u229Efp_fp0_ET_S1_" } }

// decltype on dependent operands, and the type it yields.
template <class T> using Sum = decltype(T{} ⊞ T{});
static_assert(__is_same(Sum<S>, S));
static_assert(__is_same(Sum<int>, int));

// Non-dependent use inside a template: resolved at definition time to the
// int overload and the value survives instantiation for any T.
template <class T> constexpr int fixed(T) { return 1 ⊞ 2; }
static_assert(fixed(0) == 12);
static_assert(fixed(S{}) == 12);

// A member operator reached through a dependent object: the member candidates
// are a property of the operator syntax and are assembled again at
// instantiation.
struct M {
  int v;
  constexpr int operator⊗(M o) const { return v * 100 + o.v; }
  constexpr int operator⊘() const { return v + 1; }
};
template <class T> constexpr int mem(T a, T b) { return a ⊗ b; }
template <class T> constexpr int pre(T a) { return ⊘a; }
static_assert(mem(M{1}, M{2}) == 102);
static_assert(pre(M{4}) == 5);

// Two-phase lookup: the non-member found at definition time is remembered,
// and ADL at instantiation adds to it.
namespace Later {
struct L {};
}
constexpr int operator⊛(Later::L, int) { return 1; }
template <class T> constexpr int twophase(T a) { return a ⊛ 0; }
namespace Later {
constexpr int operator⊛(L, long) { return 2; }
}
static_assert(twophase(Later::L{}) == 1);   // exact match from the definition
template <class T, class U> constexpr int twophase2(T a, U b) { return a ⊛ b; }
static_assert(twophase2(Later::L{}, 0L) == 2);   // ADL at instantiation

// A requires-expression over a dependent prefix and infix use.
template <class T> concept Ring = requires(T a, T b) { a ⊞ b; ⊖a; };
static_assert(Ring<S>);
static_assert(!Ring<M>);

// A class template whose member uses the operator on a dependent member.
template <class T> struct Acc {
  T v;
  constexpr T twice() const { return v ⊞ v; }
};
static_assert(Acc<int>{3}.twice() == 33);
static_assert(Acc<S>{S{3}}.twice().v == 33);

// Pack expansion of an operand: a user operator is not a fold operator, but
// a pack may be expanded inside its operand.
template <class... T> constexpr int sum(T... t) { return (0 + ... + (t ⊞ 0)); }
static_assert(sum(1, 2) == 30);
