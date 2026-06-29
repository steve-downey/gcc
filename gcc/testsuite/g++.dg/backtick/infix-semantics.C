// { dg-do compile }
// { dg-options "-fbacktick -std=c++17 -fdump-tree-original" }
// G05: semantics test sweep for infix backtick — overload resolution, ADL,
//      templates, constexpr, value categories, lambda.
//
// Note: DEV-G05 (ADL limitation) was fixed in G10.  Section 2 below now
// uses a bare unqualified-id slot to exercise pure ADL.  The qualified-name
// workaround is retained in a comment for historical reference.

// ---------------------------------------------------------------------------
// 1. Overload resolution: selects the same overload as f(x, y)
// ---------------------------------------------------------------------------
int f_ovl(int, int);
double f_ovl(double, double);

int    a_i, b_i;
double a_d, b_d;

static_assert(__is_same(decltype(a_i `f_ovl` b_i), int),
              "int overload selected");
static_assert(__is_same(decltype(a_d `f_ovl` b_d), double),
              "double overload selected");

int    r_ovl_int = a_i `f_ovl` b_i;
double r_ovl_dbl = a_d `f_ovl` b_d;

// { dg-final { scan-tree-dump "f_ovl \\(a_i, b_i\\)" "original" } }
// { dg-final { scan-tree-dump "f_ovl \\(a_d, b_d\\)" "original" } }

// ---------------------------------------------------------------------------
// 2. Pure ADL: name visible only in argument's namespace (DEV-G05 fixed G10).
// ---------------------------------------------------------------------------
namespace ns {
  struct T {};
  int g(T, T);
}
ns::T tx, ty;
int r_adl = tx `g` ty;  // g found via ADL on ns::T (bare unqualified-id slot)

// ---------------------------------------------------------------------------
// 3. Templates: dependent operands instantiate via tsubst of CALL_EXPR.
//    No extra code expected — tsubst handles the desugared call directly.
// ---------------------------------------------------------------------------
int add_t(int a, int b) { return a + b; }

template <typename F>
int apply(int a, int b, F func) { return a `func` b; }

int r_tmpl = apply(1, 2, add_t);

// ---------------------------------------------------------------------------
// 4. constexpr: backtick expression usable in constant expression
// ---------------------------------------------------------------------------
constexpr int add_cx(int a, int b) { return a + b; }
static_assert(1 `add_cx` 2 == 3,               "constexpr basic");
static_assert((2 `add_cx` 3) `add_cx` 4 == 9, "constexpr chain");

// ---------------------------------------------------------------------------
// 5. Codegen: CALL_EXPR visible in original tree dump (desugaring confirmed)
// ---------------------------------------------------------------------------
int p, q;
int r_cg = p `add_cx` q;
// { dg-final { scan-tree-dump "add_cx \\(p, q\\)" "original" } }

// ---------------------------------------------------------------------------
// 6. Lambda in operator slot
// ---------------------------------------------------------------------------
int lambda_fn() { return 3 `[](int a, int b){ return a * b; }` 4; }
