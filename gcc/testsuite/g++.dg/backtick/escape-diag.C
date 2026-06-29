// { dg-do compile }
// { dg-options "-fbacktick" }
// G07: diagnostics for malformed keyword-escape uses.

// Non-keyword inside escape: error.
void `x`();   // { dg-error "backtick keyword-escape requires a C\\+\\+ keyword" }
