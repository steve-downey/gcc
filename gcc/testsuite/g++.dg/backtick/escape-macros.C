// { dg-do compile }
// { dg-options "-fbacktick" }
// escape-content, the preprocessor half: the escape is a phase 7 construct
// built out of three preprocessing tokens, and phase 4 has never heard of it.
// Mirrors Clang's backtick-escape-macros.cpp.

// An object-like macro name is replaced, escaped or not: by the time the
// parser sees the escape, what stands between the backticks is the
// replacement list.  GCC places the error at the replacement list, in the
// #define, and notes the expansion at the escape; it stops at the one error.
#define OBJECT_MACRO 3 // { dg-error "backtick escape requires an identifier" }
int `OBJECT_MACRO` = 0; // { dg-message "in expansion of macro 'OBJECT_MACRO'" }

// A function-like macro name is *not* replaced when escaped, because it is
// replaced only when followed by '(' and here the next preprocessing token is
// the closing backtick.  The ordinary invocation still expands.
#define FUNC_MACRO(x) ((x) + 1)
int `FUNC_MACRO` = 7;
int read_it() { return `FUNC_MACRO`; }
int still_expands() { return FUNC_MACRO(1); }
