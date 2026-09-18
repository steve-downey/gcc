// { dg-do compile { target c++11 } }
// { dg-options "-fbacktick" }
// escape-content: the word between the backticks does not have to be a
// keyword.  Anything spelled as an identifier may stand there, and what comes
// out is that identifier and nothing more specific -- `foobar` *is* foobar.
// Mirrors Clang's backtick-escape-identifier.cpp.  No -std is given, so the
// harness runs this under every dialect in its list, which is the point of
// the `requires` case below.

// --- Identity: one name, two spellings ---
int `foobar` = 0;
int read_bare() { return foobar; }        // the same variable
int read_escaped() { return `foobar`; }   // and so is this

int fn(int x);                            // declared bare
int `fn`(int x) { return x; }             // defined escaped: one function
int call_both() { return fn(1) + `fn`(2); }

// --- The same word, every dialect ---
// `requires` is a keyword from C++20 and an ordinary identifier before it,
// and the escaped declaration means the same thing in all of them -- which
// is what lets a name be escaped *before* the committee takes the word.
// Under the keyword-only rule this was rejected below C++20.
bool `requires`(int);
bool call_requires(int x) { return `requires`(x); }

// --- Alternative tokens ([lex.digraph]) ---
int `and` = 0;
int read_and() { return `and`; }
int `bitor` = 0;

// --- Reserved names stay reserved: the escape only lets one be written ---
int `__foo` = 0;
int read_reserved() { return __foo; }

// --- Positions other than a declarator-id ---
struct Holder { int `member` = 0; };
int read_member(Holder h) { return h.`member` + h.member; }

namespace `ns` { int `inner` = 0; }
int read_qualified() { return `ns`::`inner` + ns::inner; }

enum class `Color` { `red`, green };
Color pick() { return `Color`::`red`; }

template <class `T`> struct Box { `T` value; };
int read_box() { Box<int> b{7}; return b.value; }

// --- The infix operator is untouched ---
int add(int a, int b) { return a + b; }
int infix_still_works() { int x = 1, y = 2; return x `add` y; }

// --- Printing: the spelling is put back only where it has to be ---
// A name spelled as a keyword or an alternative token prints escaped,
// because printing it bare would not re-parse; a name spelled as an ordinary
// identifier prints bare, because it is one.
void printing() {
  `foobar`();   // { dg-error "'foobar' cannot be used as a function" }
  `and`();      // { dg-error "'`and`' cannot be used as a function" }
}
