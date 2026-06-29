// { dg-do compile }
// { dg-options "-fbacktick -fno-exceptions" }
// G09: keyword-escaped identifier ABI — §12.
// The escape yields an ordinary identifier; mangling is unchanged from a
// plain identifier with the same spelling (Itanium ABI, x86-64 Linux).
// Cross-compiler claim: mangling is identical to Clang S09.

// --- 1. Free function: declaration, definition, call ---
int `new`(int, int);
int `new`(int x, int y) { return x + y; }
void test_call(int a, int b) { (void)`new`(a, b); }

// --- 2. Member function: definition, . access, -> access ---
struct Widget { void `delete`(); };
void Widget::`delete`() {}
void test_dot()   { Widget w;       w.`delete`(); }
void test_arrow() { Widget *pw = 0; pw->`delete`(); }

// --- 3. Mangling: external symbol is the plain identifier ---
// _Z3newii         == new(int, int)   (matches Clang S09 backtick-escape-abi.cpp)
// _ZN6Widget6deleteEv == Widget::delete() (matches Clang S09)
// { dg-final { scan-assembler "_Z3newii" } }
// { dg-final { scan-assembler "_ZN6Widget6deleteEv" } }
