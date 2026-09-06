// { dg-do compile }
// { dg-options "-fbacktick -std=c++17 -fdump-tree-original" }
// ADL for a bare *template-id* infix backtick slot (design doc §17.4).
//
// The bare-name slot got full ADL in G10, but its two-token lookahead
// (CPP_NAME + CPP_BACKTICK) does not see a template-id, so `x `f<T>` y` took
// the resolve-at-parse-time path and pure ADL failed with
// "'f' was not declared in this scope".  §17.4 is normative for the slot as a
// whole, not for bare names only.

// ---------------------------------------------------------------------------
// 1. Pure ADL: g visible only in ns:: (not at file scope).
// ---------------------------------------------------------------------------
namespace ns {
  struct A {};
  template<class T> int g(A, A) { return 1; }
}

ns::A ax, ay;
int r_pure = ax `g<int>` ay;   // g found via ADL on ns::A args

// { dg-final { scan-tree-dump "r_pure = ns::g<int>" "original" } }

// ---------------------------------------------------------------------------
// 2. ADL augmentation: h visible at file scope AND via ADL in ns2::.
// ---------------------------------------------------------------------------
namespace ns2 {
  struct B {};
  template<class T> double h(B, B);   // ADL overload
}
template<class T> int h(int, int);    // ordinary-scope overload

ns2::B bx, by;
double r_augment = bx `h<int>` by;

// { dg-final { scan-tree-dump "r_augment = ns2::h<int>" "original" } }

// ---------------------------------------------------------------------------
// 3. Nested template arguments, and the RHS-lookahead handler that gives the
//    operator its highest binary precedence.
// ---------------------------------------------------------------------------
namespace ns3 {
  struct C {};
  template<class T> struct W {};
  template<class T> int k(C, C) { return 3; }
}

ns3::C cx, cy;
int r_nested = cx `k<ns3::W<int>>` cy;
int r_rhs    = 3 * cx `k<int>` cy;    // parses as 3 * k<int>(cx, cy)

// { dg-final { scan-tree-dump "r_nested = ns3::k<ns3::W<int> >" "original" } }
// { dg-final { scan-tree-dump "r_rhs = ns3::k<int>" "original" } }

// ---------------------------------------------------------------------------
// 4. A slot that is a relational expression is still a relational expression:
//    `f<b` is not a template-id, so the template-id path must roll back and
//    leave the old parse in place.  A parenthesised slot is unaffected.
// ---------------------------------------------------------------------------
struct S {};
S sx, sy;
int fsel(S, S);
int b = 0;
int r_paren = sx `(b < 1 ? fsel : fsel)` sy;

// { dg-final { scan-tree-dump "r_paren = " "original" } }
