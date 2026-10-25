// { dg-additional-options "-fmodules-ts -funicode-operators" }
// A Unicode user-defined operator crossing a module boundary.  The
// operator-function-id is an identifier spelled from the code point, so the
// exported functions stream through the CMI as any other name does, and a
// template whose body uses the operator on dependent operands streams the
// USER_OPERATOR_EXPR that holds the use until instantiation.

export module uniop;
// { dg-module-cmi uniop }

export struct W { int v; };

export constexpr W operator⊞ (W a, W b) { return {10 * a.v + b.v}; }
export constexpr W operator⊖ (W a) { return {-a.v}; }

export struct M
{
  int v;
  constexpr int operator⊗ (M o) const { return 100 * v + o.v; }
};

// Dependent uses, infix, prefix and member, to be instantiated by the
// importer.
export template <class T> constexpr auto combine (T a, T b) { return a ⊞ b; }
export template <class T> constexpr auto negate (T a) { return ⊖a; }
export template <class T> constexpr auto mem (T a, T b) { return a ⊗ b; }
