// { dg-do compile }
// { dg-options "-fbacktick" }
// G04: diagnostics for malformed infix backtick uses.

int add(int a, int b);
int a, b;

// Empty slot: two consecutive backticks produce a specific error.
void test_empty_slot() { a ``; }     // { dg-error "expected expression between" }

// Unterminated: missing close backtick before statement end.
void test_unterminated() { a `add; } // { dg-error "expected" }

// Parenthesised slot expression is well-formed (D3 positive case: parens in slot).
void test_paren_slot() { (void)(a `(add)` b); }
