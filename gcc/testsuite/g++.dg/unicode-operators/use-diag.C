// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Diagnostics for ill-formed uses.  A use is never a lexing or parsing error
// in a translation unit with the feature on (unicode-operators.md,
// lexing-and-declarations): when nothing is viable the diagnostic is the
// ordinary no-viable-overload error naming operator⊞, the same category of
// error as std::cout << my_type{} without the << overload.  And there are no
// built-in candidates: a fundamental-type pair finds the user's operator or
// nothing, never an arithmetic or pointer meaning.  (The testsuite runs in
// the C locale, where a name with a non-ASCII character is printed as a UCN.)

constexpr int operator⊞(int a, int b) { return 2 * a + b; }   // { dg-note "initializing argument" }
constexpr int operator⊖(int a) { return 3 * a + 1; }

// Nothing named operator⊠ is declared anywhere in this translation unit.
int no_operator_at_all(int a, int b) {
  return a ⊠ b;   // { dg-error "no match for 'operator.U000022a0' \\(operand types are 'int' and 'int'\\)" }
}
int no_prefix_at_all(int a) {
  return ⊠a;   // { dg-error "no match for 'operator.U000022a0' \\(operand type is 'int'\\)" }
}

// An operator⊟ exists but does not accept ints.  If built-in candidates were
// in the set, a ⊟ b would quietly become integer arithmetic.
struct Base {};
int operator⊟(Base, Base);   // { dg-note "candidate" }
// { dg-note "no known conversion" "" { target *-*-* } .-1 }
int no_arithmetic_fallback(int a, int b) {
  return a ⊟ b;   // { dg-error "no match for 'operator.U0000229f' \\(operand types are 'int' and 'int'\\)" }
  // { dg-note "there is 1 candidate" "" { target *-*-* } .-1 }
}

// The same for pointers: no built-in pointer arithmetic is synthesized even
// though operator⊞(int, int) is visible; what is diagnosed is the ordinary
// call's bad conversion to that candidate, exactly as for operator⊞(p, n).
int *no_pointer_arithmetic(int *p, long n) {
  return p ⊞ n;   // { dg-error "invalid conversion from 'int\\*' to 'int'" }
  // { dg-error "invalid conversion from 'int' to 'int\\*'" "" { target *-*-* } .-1 }
  // { dg-note "dereference" "" { target *-*-* } .-2 }
}

// ADL must not fire when nothing associates the namespace: fundamental
// operands only, operator in an unrelated namespace.
namespace Unrelated {
constexpr int operator⊧(int, int) { return 50; }
}
int adl_does_not_reach_here(int a, int b) {
  return a ⊧ b;   // { dg-error "no match for 'operator.U000022a7'" }
}

// ADL does not find member operators for the explicit call.
namespace Adl5 {
struct D {
  constexpr int operator⊡(D) const { return 51; }
};
}
int adl_finds_no_members(Adl5::D d) {
  return operator⊡(d, d);   // { dg-error "'operator.U000022a1' was not declared in this scope|not defined" }
}

// Ordinary member diagnostics, naming the operator.
struct MemBad {
  int operator⊣(MemBad) const;   // { dg-note "candidate" }
  // { dg-note "no known conversion" "" { target *-*-* } .-1 }
};
int member_not_viable(MemBad m, Base b) {
  return m ⊣ b;   // { dg-error "no match for 'operator.U000022a3' \\(operand types are 'MemBad' and 'Base'\\)" }
  // { dg-note "there is 1 candidate" "" { target *-*-* } .-1 }
}

// GCC's tie-breaker between a member and a non-member whose conversion
// sequences are incomparable is the one it applies to operator+ in the same
// situation: a warning rather than the standard's ambiguity error.
struct AmbMN {
  int operator⊢(int) const;   // { dg-note "candidate" }
};
int operator⊢(AmbMN, int);   // { dg-note "candidate" }
int member_nonmember_ambiguous(AmbMN a) {
  return a ⊢ 1;   // { dg-warning "ISO C\\+\\+ says that these are ambiguous" }
}

class PrivateOp {
  int operator⊪(PrivateOp) const;   // { dg-note "declared private here" }
};
int member_access(PrivateOp p) {
  return p ⊪ p;   // { dg-error "is private within this context" }
}

struct DeletedOp {
  int operator⊩(DeletedOp) const = delete;   // { dg-note "declared here" }
};
int member_deleted(DeletedOp d) {
  return d ⊩ d;   // { dg-error "use of deleted function" }
}

// A prefix overload and a defaulted-second-parameter infix overload of the
// same code point make a prefix use ambiguous -- the ordinary ambiguity for
// f(x) given f(int) and f(int, int = 1), reached through operator syntax.
constexpr int operator⊡(int a) { return 1000 + a; }   // { dg-note "candidate" }
constexpr int operator⊡(int a, int b = 1) { return 7 * a + b; }   // { dg-note "candidate" }
constexpr int ambiguous = ⊡5;   // { dg-error "ambiguous overload for 'operator.U000022a1' \\(operand type is 'int'\\)" }
// { dg-note "there are 2 candidates" "" { target *-*-* } .-1 }

// No braced-init-list operand: the operands are cast-expressions.
int braced(int a) { return a ⊞ {1, 2}; }   // { dg-error "expected primary-expression before '.' token" }
// { dg-error "expected ';' before" "" { target *-*-* } .-1 }
int braced_prefix() { return ⊖{1, 2}; }   // { dg-error "expected primary-expression before '.' token" }
// { dg-error "expected ';' before" "" { target *-*-* } .-1 }

// Still a binary operator, so a missing right operand is the ordinary
// missing-operand diagnostic -- and there is no postfix form, so a postfix
// attempt is that same diagnostic, with or without a space.
int missing(int a) { return a ⊞ ; }   // { dg-error "expected primary-expression before ';' token" }
int postfix(int a) { return a⊖; }   // { dg-error "expected primary-expression before ';' token" }
int no_operand() { return ⊖; }   // { dg-error "expected primary-expression before ';' token" }

// Not a fold operator.
template <int... N> constexpr int lfold() { return (... ⊞ N); }   // { dg-error "expected binary operator before '.U0000229e'" }
// { dg-error "expected '\\)' before '.U0000229e'" "" { target *-*-* } .-1 }
template <int... N> constexpr int rfold() { return (N ⊞ ...); }   // { dg-error "expected primary-expression before '...' token" }
// { dg-error "expected '\\)' before '...' token" "" { target *-*-* } .-1 }
