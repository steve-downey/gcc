// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// An excluded code point says why it is excluded (unicode-operators.md
// section 5 predicate 5, section 8), and is never aliased to the token it
// apes: a program containing a character that looks like '-' but is not must
// fail to lex, not quietly mean something else.  The three exclusion classes
// do not have the same scope.  Confusable and emoji-presentation code points
// are not identifier characters and never were, so nothing but an operator
// could have been meant: the lexer diagnoses them where it classifies them,
// for the glyph and for every universal-character-name spelling alike, and
// the code point becomes one stray token whose only further consequence is
// the ordinary follow-on parse error.  The identifier-profile exclusions
// (∂ ∇ ∞) ARE identifier characters, so a lexer diagnostic would fire on
// every legitimate use of one as a name; they are noted only in
// operator-name position, where an operator was unambiguously meant.  (The
// testsuite runs in the C locale, where the front end prints a non-ASCII
// character in a name as a UCN; the lexer prints the source bytes.)

int a, b;

// 1. Confusable, all spellings of one code point.
int c1 = a − b;   // { dg-error "− U\\+2212 is not a user-defined operator: it is confusable with '-'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int c2 = a − b;   // { dg-error "U\\+2212 is not a user-defined operator: it is confusable with '-'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int c3 = a \N{MINUS SIGN} b;   // { dg-error "U\\+2212 is not a user-defined operator: it is confusable with '-'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
// Adjacency: an excluded code point ends an identifier rather than being
// absorbed into it, exactly as an operator code point does.  Without that the
// only thing reported would be "not valid in an identifier", which is true and
// says nothing about why U+2212 in particular is out.
int c4 = a−b;   // { dg-error "− U\\+2212 is not a user-defined operator: it is confusable with '-'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }

// 2. Confusable, the whole table.  The ASCII token in each message comes from
// the generated table (libcpp/unicode-operators.h), not from a switch in the
// lexer, so the derivation and the diagnostic cannot drift apart.
int d1 = a ⁄ b;   // { dg-error "U\\+2044 is not a user-defined operator: it is confusable with '/'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d2 = a ⇐ b;   // { dg-error "U\\+21D0 is not a user-defined operator: it is confusable with '<='" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d3 = a ⇒ b;   // { dg-error "U\\+21D2 is not a user-defined operator: it is confusable with '=>'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d4 = a ⇔ b;   // { dg-error "U\\+21D4 is not a user-defined operator: it is confusable with '<=>'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d5 = a ∕ b;   // { dg-error "U\\+2215 is not a user-defined operator: it is confusable with '/'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d6 = a ∗ b;   // { dg-error "U\\+2217 is not a user-defined operator: it is confusable with '\\*'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d7 = a ∙ b;   // { dg-error "U\\+2219 is not a user-defined operator: it is confusable with '.'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d8 = a ∣ b;   // { dg-error "U\\+2223 is not a user-defined operator: it is confusable with '\\|'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d9 = a ∶ b;   // { dg-error "U\\+2236 is not a user-defined operator: it is confusable with ':'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d10 = a ≤ b;   // { dg-error "U\\+2264 is not a user-defined operator: it is confusable with '<='" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d11 = a ≥ b;   // { dg-error "U\\+2265 is not a user-defined operator: it is confusable with '>='" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int d12 = a ⋅ b;   // { dg-error "U\\+22C5 is not a user-defined operator: it is confusable with '.'" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }

// 3. Emoji presentation.
int e1 = a ⌚ b;   // { dg-error "U\\+231A is not a user-defined operator: characters with emoji presentation are excluded from the operator set" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }
int e2 = a ⭐ b;   // { dg-error "U\\+2B50 is not a user-defined operator: characters with emoji presentation are excluded" }
// { dg-error "expected ',' or ';' before 'b'" "" { target *-*-* } .-1 }

// 4. A code point that is neither in the set nor a named exclusion nor an
// identifier character keeps the diagnostic it gets without the flag.
int f1 = a ¬ b;   // { dg-error "extended character ¬ is not valid in an identifier" }
// { dg-error "expected ',' or ';' before '.U000000ac'" "" { target *-*-* } .-1 }

// 5. The identifier profile: ∂ is an identifier character, so a function may
// be named with it, and the exclusion is a note in operator-name position
// only, on a conversion-function-id parse that has already failed.
int ∂(int x) { return x; }
int use_partial() { return ∂(1); }
struct S {};
int operator ∂(S);   // { dg-error "expected type-specifier before '.U00002202'" }
// { dg-message "'.U00002202' U\\+2202 is not a user-defined operator: the mathematical notation profile of UAX #31 reserves it as an identifier character" "" { target *-*-* } .-1 }

// 6. Literals and comments are untouched: an excluded code point in a string
// or a comment is not classified at all.
const char *s = "− ≤ ⌚";   // −
