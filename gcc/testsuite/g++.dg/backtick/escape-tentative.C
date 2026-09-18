// { dg-do compile }
// { dg-options "-fbacktick" }
// G08: escaped names survive GCC tentative parse in both declaration and
//      expression-statement contexts.

struct T { int x; };
int `new`(int, int);          // function named 'new'

// ---------------------------------------------------------------------------
// 1. Tentative-parse: declaration preferred when T is a known type
//    T `new`;  ->  variable of type T named 'new'  (not an expression)
// ---------------------------------------------------------------------------
void test_declaration() {
    T `new`;                  // VarDecl: type=T, name='new'
    (void)`new`.x;            // member-access proves 'new' is a T, not a call
}

// ---------------------------------------------------------------------------
// 2. Expression-statement: `new`(1,2) is a call, not a declaration
//    ('new' does not name a type here, so no declaration reading is possible)
// ---------------------------------------------------------------------------
void test_expression_statement() {
    `new`(1, 2);              // expression-statement: calls ::new(int,int)
}

// ---------------------------------------------------------------------------
// 3. Both forms in one scope: prefer-declaration rule holds
// ---------------------------------------------------------------------------
void test_both() {
    T `new` = {};             // declaration (prefer-declaration; T names a type)
    (void)`new`.x;            // use the declared variable
}

// ---------------------------------------------------------------------------
// 4. The same shapes with a name that is not a keyword (escape-content).
//    The tentative parse reaches the same verdicts on the wider set, and the
//    infix operator still wins in post-operand position.
// ---------------------------------------------------------------------------
int `ordinary`(int, int);

void test_declaration_ordinary() {
    T `v`;                    // declaration
    (void)v.x;                // bare use of the escaped name
}

void test_expression_ordinary() {
    `ordinary`(1, 2);         // call
}

int test_infix_ordinary(int a, int b) {
    return a `ordinary` b;    // operator, by position
}
