// { dg-do compile { target c++23 } }
// { dg-options "-funicode-operators" }
// Which operator⊞ declarations are ill-formed (unicode-operators.md section 7,
// "Declaring"): arity, and a static member, and nothing else.  The second
// half is the control: the existing operators must behave exactly as before,
// which is what shows the relaxation was scoped to the new name and not to a
// shared checker.  (The testsuite runs in the C locale, where a name with a
// non-ASCII character is printed as a UCN.)

int operator⊡();   // { dg-error "user-defined operator 'int operator.U000022a1\\(\\)' must have one parameter \\(prefix\\) or two parameters \\(infix\\); it has 0 parameters" }

int operator⋄(int, int, int);   // { dg-error "user-defined operator 'int operator.U000022c4\\(int, int, int\\)' must have one parameter \\(prefix\\) or two parameters \\(infix\\); it has 3 parameters" }

struct T {
  // The free infix form written as a member: two declared parameters plus the
  // implicit object parameter is three operands.
  int operator⊗(T, T);   // { dg-error "user-defined operator 'int T::operator.U00002297\\(T, T\\)' must have no parameters \\(prefix\\) or one parameter \\(infix\\); it has 2 parameters" }

  // A static member is rejected by choice: x ⊞ y means operator⊞(x, y) or
  // x.operator⊞(y), and a static member is named by neither.
  static int operator⊢(T, T);   // { dg-error "'static int T::operator.U000022a2\\(T, T\\)' must be either a non-static member function or a non-member function" }
};

namespace N {
int operator⊡(int, int, int, int);   // { dg-error "it has 4 parameters" }
}

template <class U> U operator⋄();   // { dg-error "it has 0 parameters" }

// Only functions may be declared using an operator-function-id.
int operator⊞;   // { dg-error "declaration of 'operator.U0000229e' as non-function" }
typedef int operator⊠(int, int);   // { dg-error "declaration of 'operator.U000022a0' as 'typedef'" }

// Control: no existing operator's rules moved.
int operator+(int, int);   // { dg-error "'int operator\\+\\(int, int\\)' must have an argument of class or enumerated type" }
struct P {};
P operator*(P, P, P);      // { dg-error "must have either one or two arguments" }
P operator&(P, P, ...);    // { dg-error "must not have variable number of arguments" }
P operator-(P a, P b = P());   // { dg-error "cannot have default arguments" }
struct Q {
  static P operator%(Q, Q);   // { dg-error "must be either a non-static member function or a non-member function" }
};
