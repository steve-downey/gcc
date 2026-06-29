// { dg-do compile }
// { dg-options "-fbacktick -std=c++17 -fdump-tree-original" }
// G10: ADL for bare-name infix backtick slot (§17.4; DEV-G05 fix).
// Pure ADL and ADL augmentation must work for a bare unqualified-id slot.

// ---------------------------------------------------------------------------
// 1. Pure ADL: g visible only in ns:: (not at file scope).
// ---------------------------------------------------------------------------
namespace ns {
  struct A {};
  int g(A, A) { return 1; }
}

ns::A ax, ay;
int r_pure = ax `g` ay;   // g found via ADL on ns::A args

// GCC prints the resolved (qualified) name in the original dump.
// { dg-final { scan-tree-dump "r_pure = ns::g" "original" } }

// ---------------------------------------------------------------------------
// 2. ADL augmentation: h visible at file scope AND via ADL in ns2::.
//    The overload for ns2::B must be selected when operands are ns2::B.
// ---------------------------------------------------------------------------
namespace ns2 {
  struct B {};
  double h(B, B);   // ADL overload
}
int h(int, int);    // ordinary-scope overload (for int operands, not selected here)

ns2::B bx, by;
double r_augment = bx `h` by;  // ns2::h found via ADL on ns2::B args

// GCC prints the resolved (qualified) name in the original dump.
// { dg-final { scan-tree-dump "r_augment = ns2::h" "original" } }
