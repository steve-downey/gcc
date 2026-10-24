// { dg-do compile }
// { dg-options "-fbacktick" }
// G07: diagnostics for malformed keyword-escape uses.

// Not an identifier inside the escape.  escape-content makes the content
// rule "any word spelled as an identifier", so what is left to reject is
// everything that is not a word: a number, punctuation, a literal.  `x` used
// to be here and is now well-formed; it lives in escape-identifier.C.
void `3`();   // { dg-error "backtick escape requires an identifier" }
void `+`();   // { dg-error "backtick escape requires an identifier" }
void `&&`();  // { dg-error "backtick escape requires an identifier" }
void `"s"`(); // { dg-error "backtick escape requires an identifier" }

// The two spellings are one identifier: this conflicts only if `clash` and
// clash are the same name.
extern int `clash`;  // { dg-message "previous declaration" }
extern float clash;  // { dg-error "conflicting declaration" }

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

// ---------------------------------------------------------------------------
// An escaped name is printed escaped.  Under -fbacktick `kw` is the only
// spelling the name has -- a bare keyword declarator-id is rejected -- so a
// diagnostic naming the entity `new' spells it with something no program can
// contain, and text copied out of the diagnostic does not re-parse.
// ---------------------------------------------------------------------------

void `new` (int);      // { dg-message "initializing argument 1 of 'void `new`\\(int\\)'" }

void escaped_name_is_printed_escaped ()
{
  `new` ("x");         // { dg-error "invalid conversion from 'const char\\*' to 'int'" }
}

// ---------------------------------------------------------------------------
// A type's name, a template parameter's and a namespace's are printed by
// other routines than a declaration's, and are escaped the same way, so one
// diagnostic never names its entities two ways (gcc-type-name-printing).
// ---------------------------------------------------------------------------

struct `union` { int x; };
int class_type (`union` u) { return u.`new`; } // { dg-error "'struct `union`' has no member named '`new`'" }

enum `enum` { e1 };
`enum` enum_type = 1;	// { dg-error "invalid conversion from 'int' to '`enum`'" }

template<class T> struct `virtual` { };
int class_template (`virtual`<int> w) { return w.y; } // { dg-error "'struct `virtual`<int>' has no member named 'y'" }

struct `and` { };
int alternative_token (`and` a) { return a.q; } // { dg-error "'struct `and`' has no member named 'q'" }

struct H { struct `union` { }; };
int nested_type (H::`union` u) { return u.r; } // { dg-error "'struct H::`union`' has no member named 'r'" }

namespace `namespace` { int k; }
int namespace_name () { return `namespace`::nope; } // { dg-error "'nope' is not a member of '`namespace`'" }

template<class `int`> int template_parm (`int` x) { return x.z; } // { dg-error "request for member 'z'" }
int instantiate = template_parm (1);
// { dg-message "In instantiation of 'int template_parm\\(`int`\\) \\\[with `int` = int\\\]'" "" { target *-*-* } 0 }
