// { dg-do compile }
// { dg-options "-fbacktick" }
// G07: keyword-escape `kw` identifiers — name position (declarator-id,
//      primary-expression, member access).

// ---------------------------------------------------------------------------
// 1. Declarator-id: `new` names a function (§12 escape rule)
// ---------------------------------------------------------------------------
void `new`(int, int);

// ---------------------------------------------------------------------------
// 2. Primary-expression call to escaped name
// ---------------------------------------------------------------------------
void test_call(int a, int b) { `new`(a, b); }

// ---------------------------------------------------------------------------
// 3. Member with escaped name
// ---------------------------------------------------------------------------
struct S {
  int `delete`;
  void `new`(int);
};

void test_member(S &s) {
  (void)s.`delete`;   // member access after '.'
  s.`new`(42);        // member-function call
}

// ---------------------------------------------------------------------------
// 4. Infix regression: infix backtick still works after G07
// ---------------------------------------------------------------------------
int add(int, int);
int a, b;
int r_infix = a `add` b;

// ---------------------------------------------------------------------------
// 5. D3 interaction: escaped callee inside infix paren slot
//    x `(`new`)` y -> new(x, y)
// ---------------------------------------------------------------------------
void test_d3(int x, int y) { x `(`new`)` y; }
