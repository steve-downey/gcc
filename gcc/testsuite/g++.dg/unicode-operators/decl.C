// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// operator⊞ parses as an operator-function-id wherever one may appear
// (unicode-operators.md, operator-function-id), and every spelling of the
// operator -- glyph, \u, \U, \u{}, \N{} -- names one entity.

// Free function: declaration, then definition.  One entity.  The
// declaration is spelled with a named universal-character-name and the
// definition with the glyph.  This constant-evaluates only if the call
// resolves to the *defined* declaration, i.e. only if the two spellings
// produced the same name and were merged into one redeclaration chain.
constexpr int operator\N{SQUARED PLUS}(int, int);
constexpr int operator⊞(int a, int b) { return a + b; }
static_assert(operator⊞(2, 3) == 5);
// ... and the call may be spelled either way too, in either direction.
static_assert(operator\U0000229E(2, 3) == 5);
static_assert(operator⊞(2, 3) == 5);
static_assert(operator\u{229E}(2, 3) == 5);
static_assert(operator\N{SQUARED PLUS}(2, 3) == 5);

// Explicit call and address-of.
int (*pfn)(int, int) = &operator⊞;
int use() { return operator⊞(1, 2); }

// Members, friend, = delete.
struct S {
  int operator⊞(S) const;
  int operator⊗() const;               // prefix form
  S operator⊟(S) const = delete;
  friend int operator⊕(S, S);
};
int S::operator⊞(S) const { return 0; }

// Qualified declarator.
namespace N {
int operator⊞(S, S);
}
int N::operator⊞(S, S) { return 1; }

// Template, explicit specialization, explicit instantiation.
template <class T> T operator⊠(T a, T) { return a; }
template <> int operator⊠(int, int b) { return b; }
template double operator⊠(double, double);
int tmpl() { return operator⊠<char>('a', 'b'); }

// using-declaration.
namespace M {
int operator⊞(S, int);
}
using M::operator⊞;

// Tentative parsing: a statement starting with a type-name is ambiguous
// between a declaration and an expression, and the declarator parse must
// accept the operator-function-id.
void tentative() {
  S operator⊛(S, S);
  S (operator⊝)(S, S);
}

// constexpr, noexcept, deleted at namespace scope.
constexpr int operator⊘(int a) noexcept { return -a; }
static_assert(operator⊘(3) == -3);
void operator⊚(S) = delete;

// Of [over.oper]'s restrictions only the arity carries over
// (unicode-operators.md section 7).  Accepted: fundamental types only;
// free prefix and infix; member infix and prefix counting the implicit object
// parameter; explicit object parameters counted as declared parameters;
// templates and specializations; consteval, deleted; variadic; defaulted.
struct Vec {};
Vec operator⊖(Vec const &);
Vec operator⊞(Vec const &, Vec const &);
struct R {
  int operator⊕(R) const;   // member infix:  1 declared + implicit object = 2
  int operator⊘() const;    // member prefix: 0 declared + implicit object = 1
  int operator⊙(this R, R); // member infix
  int operator⊚(this R);    // member prefix
  friend Vec operator⊠(R, R);
};
template <class T> T operator⊛(T a) { return a; }
consteval int operator⊜(int a) { return -a; }
static_assert(operator⊜(3) == -3);
void operator⊜(Vec) = delete;
int operator⊝(int, ...);
int operator⊟(int a, int b = 1);
// There is no postfix form and no way to spell one: a trailing int parameter
// is an ordinary second operand here, not a postfix marker.
int operator⊟(Vec, int);
