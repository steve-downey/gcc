// { dg-do compile { target { c++23 && { x86_64-*-linux* } } } }
// { dg-options "-funicode-operators -fno-exceptions" }
// operator⊞ mangles through the Itanium vendor-extended operator production
// (unicode-operators.md section 9, mangling-derivation-rule):
//
//     <operator-name> ::= v <digit> <source-name>
//
// with the <digit> the operator's arity (1 prefix, 2 infix, counting a
// member's implicit object parameter) and the <source-name> derived from the
// code point as "op_u" + uppercase hex, no "U+" prefix, at least four digits.
// The symbols pinned here are the ones Clang's prototype emits for the same
// declarations, so the two implementations agree on the ABI.

struct S {};

// free infix: two parameters, arity 2
int operator⊞(S, S) { return 1; }
// { dg-final { scan-assembler "_Zv28op_u229E1SS_" } }

// free prefix: one parameter, arity 1
int operator⊖(S) { return 2; }
// { dg-final { scan-assembler "_Zv18op_u22961S" } }

// member infix: ONE declared parameter, arity 2; member prefix: none, arity 1
struct T {
  int operator⊞(T) const;
  int operator⊖() const;
};
int T::operator⊞(T) const { return 3; }
int T::operator⊖() const { return 4; }
// { dg-final { scan-assembler "_ZNK1Tv28op_u229EES_" } }
// { dg-final { scan-assembler "_ZNK1Tv18op_u2296Ev" } }

// explicit object parameter: declared parameters are the operands
struct E {
  int operator⊗(this E, E);
  int operator⊘(this E);
};
int E::operator⊗(this E, E) { return 5; }
int E::operator⊘(this E) { return 6; }
// { dg-final { scan-assembler "_ZNH1Ev28op_u2297ES_S_" } }
// { dg-final { scan-assembler "_ZNH1Ev18op_u2298ES_" } }

// template instantiation
template <class X> int operator⊠(X, X) { return 7; }
template int operator⊠<int>(int, int);
// { dg-final { scan-assembler "_Zv28op_u22A0IiEiT_S0_" } }

// namespace scope, and a nested-name substitution
namespace N {
int operator⊟(S, S) { return 8; }
}
// { dg-final { scan-assembler "_ZN1Nv28op_u229FE1SS0_" } }

// the hex-derivation rule across the range: both ends, and a code point whose
// hex has no letters
int operator←(S, S) { return 9; }
int operator⯿(S, S) { return 10; }
int operator≨(S, S) { return 11; }
// { dg-final { scan-assembler "_Zv28op_u21901SS_" } }
// { dg-final { scan-assembler "_Zv28op_u2BFF1SS_" } }
// { dg-final { scan-assembler "_Zv28op_u22681SS_" } }

// spelling independence: the derivation reads the code point, never a
// spelling.  Declared with a named UCN, defined with the glyph above: one
// symbol, and a call written with a UCN spelling targets it.
int operator\N{SQUARED PLUS}(S, S);
int call_all() {
  S s;
  return operator⊞(s, s) + operator⊖(s) + operator\N{SQUARED PLUS}(s, s)
	 + (s ⊞ s) + ⊖s;
}
// { dg-final { scan-assembler-not "op_u0000" } }
