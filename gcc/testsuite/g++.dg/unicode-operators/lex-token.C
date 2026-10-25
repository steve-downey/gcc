// { dg-do preprocess { target c++23 } }
// { dg-options "-funicode-operators" }
// A code point in the frozen operator set lexes as one token under
// -funicode-operators (unicode-operators.md, lexing-and-declarations), whatever
// its spelling and whatever is declared, and -E writes it back as a
// universal-character-name the way it writes every extended character, so
// the output re-lexes to the same tokens.  Literals, comments and macro
// bodies behave as for any other token.

int a, b;
int c = a ⊞ b;
// { dg-final { scan-file lex-token.i "int c = a \\\\U0000229e b;" } }

// Adjacency: no whitespace needed.  No operator code point is an identifier
// character, so the identifier lexer stops in front of it and resumes after.
int d = a⊞b;
// { dg-final { scan-file lex-token.i "int d = a\\\\U0000229eb;" } }

// Two operators in a row are two tokens: every operator is one code point and
// none is a prefix of another.
int e = a ⊞⊗ b;
// { dg-final { scan-file lex-token.i "int e = a \\\\U0000229e\\\\U00002297 b;" } }

// Every spelling is the one token: \u, \U, \u{} and \N{}.
int f = a ⊞ b \U0000229E a \u{229E} b \N{SQUARED PLUS} a;
// { dg-final { scan-file lex-token.i "int f = a \\\\U0000229e b \\\\U0000229e a \\\\U0000229e b \\\\U0000229e a;" } }

// A macro may carry an operator, and stringification spells it as written.
#define OP ⊞
#define STR(x) #x
int g = a OP b;
const char *h = STR(⊞) STR(\u229E);
// { dg-final { scan-file lex-token.i "int g = a \\\\U0000229e b;" } }
// { dg-final { scan-file lex-token.i "char .h = \"⊞\" \"\\\\u229E\";" } }

// Literals and comments are untouched: their bodies never reach the
// classification.  /* ⊞ ⊗ ↦ */
const char *s = "⊞ ⊗ ↦";
const char *r = R"(⊞ ⊗ ↦)";
char k = '⊞';
// { dg-final { scan-file lex-token.i "char .s = \"⊞ ⊗ ↦\";" } }
// { dg-final { scan-file lex-token.i "char k = '⊞';" } }
