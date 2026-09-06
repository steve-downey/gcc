// { dg-do compile }
// { dg-options "-fbacktick" }
// G07: diagnostics for malformed keyword-escape uses.

// Non-keyword inside escape: error.
void `x`();   // { dg-error "backtick keyword-escape requires a C\\+\\+ keyword" }

// ---------------------------------------------------------------------------
// A bare keyword is not an escape.  -fbacktick must not change what any
// program that contains no backtick diagnoses: the escape arms sit behind
// `case CPP_BACKTICK` labels that other cases fall through to, so they are
// entered on the token type, not on the flag alone.  Both of these are
// character-identical to what a build without the flag reports.
// ---------------------------------------------------------------------------

void new (int, int);   // { dg-error "expected unqualified-id before 'new'" }

int stray_xor ()
{
  return ^;            // { dg-error "expected primary-expression before '\\^' token" }
                       // { dg-error "expected primary-expression before ';' token" "" { target *-*-* } .-1 }
}
