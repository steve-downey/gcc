// { dg-do compile }
// { dg-options "-fbacktick -std=c++20" }
// gcc-dependent-slot-lookup: inside a template, an unqualified slot gets the
// lookup the call it desugars to gets -- ordinary lookup in the definition
// context, augmented by ADL at instantiation ([temp.dep.candidate]; design
// doc §17.4).  Every shape is written twice, as the call (the control) and
// as the operator, and the choice of function is made observable in the
// result type, because weaker lookup that still finds a viable candidate
// produces no diagnostic at all.

template<class A, class B> struct same { static constexpr bool value = false; };
template<class A> struct same<A, A> { static constexpr bool value = true; };

namespace lib {
  template<class T, class F> auto pipe(T t, F f) { return f(t); }
  template<class R, class T, class U> R conv(T, U) { return R(); }
}

namespace app {
  struct S { int v; };
  int inc(S s) { return s.v + 1; }
  // Reachable by ADL from S.
  long pick(S, S) { return 0; }
  char named(S, int) { return 0; }
}

namespace user {
  // 1. A using-declaration: ordinary lookup finds it, ADL cannot.
  using lib::pipe;
  template<class T> auto call_using(T t) { return pipe(t, app::inc); }
  template<class T> auto op_using(T t) { return t `pipe` app::inc; }

  // 2. A function in an enclosing namespace that ADL does not reach.
  short near(app::S, int) { return 0; }
  template<class T> auto call_near(T t) { return near(t, 1); }
  template<class T> auto op_near(T t) { return t `near` 1; }

  // 3. A variable: ordinary lookup finds a non-function, so there is no ADL.
  struct adder { double operator()(app::S, int) const { return 0; } };
  inline constexpr adder add{};
  template<class T> auto call_var(T t) { return add(t, 1); }
  template<class T> auto op_var(T t) { return t `add` 1; }

  // 5. Augmentation: a visible, viable candidate, and a better one by ADL.
  template<class U> int pick(U, U) { return 0; }
  template<class T> auto call_aug(T t) { return pick(t, t); }
  template<class T> auto op_aug(T t) { return t `pick` t; }

  // 6. A variable hides the ADL candidate of the same name ([basic.lookup.argdep]/3).
  struct namer { float operator()(app::S, int) const { return 0; } };
  inline constexpr namer named{};
  template<class T> auto call_hide(T t) { return named(t, 1); }
  template<class T> auto op_hide(T t) { return t `named` 1; }

  // 7. A template-id slot naming a template brought in by a using-declaration.
  using lib::conv;
  template<class T> auto call_tid(T t) { return conv<unsigned>(t, 1); }
  template<class T> auto op_tid(T t) { return t `conv<unsigned>` 1; }

  // 8. A block-scope function declaration suppresses ADL.
  template<class T> auto call_block(T t) { short pick(app::S, app::S); return pick(t, t); }
  template<class T> auto op_block(T t) { short pick(app::S, app::S); return t `pick` t; }

  // 9. A local callable captured by a lambda.
  template<class T> auto call_lambda(T t) {
    auto loc = [](app::S, int) { return 'x'; };
    return [&] { return loc(t, 1); }();
  }
  template<class T> auto op_lambda(T t) {
    auto loc = [](app::S, int) { return 'x'; };
    return [&] { return t `loc` 1; }();
  }

  // 10. A parenthesized or qualified slot gets no ADL, as the call gets none.
  template<class T> auto call_paren(T t) { return (pick)(t, t); }
  template<class T> auto op_paren(T t) { return t `(pick)` t; }
  template<class T> auto call_qual(T t) { return user::pick(t, t); }
  template<class T> auto op_qual(T t) { return t `user::pick` t; }

  app::S s{1};

  static_assert(same<decltype(call_using(s)), int>::value);
  static_assert(same<decltype(op_using(s)), int>::value);
  static_assert(same<decltype(call_near(s)), short>::value);
  static_assert(same<decltype(op_near(s)), short>::value);
  static_assert(same<decltype(call_var(s)), double>::value);
  static_assert(same<decltype(op_var(s)), double>::value);
  static_assert(same<decltype(call_aug(s)), long>::value);
  static_assert(same<decltype(op_aug(s)), long>::value);
  static_assert(same<decltype(call_hide(s)), float>::value);
  static_assert(same<decltype(op_hide(s)), float>::value);
  static_assert(same<decltype(call_tid(s)), unsigned>::value);
  static_assert(same<decltype(op_tid(s)), unsigned>::value);
  static_assert(same<decltype(call_block(s)), short>::value);
  static_assert(same<decltype(op_block(s)), short>::value);
  static_assert(same<decltype(call_lambda(s)), char>::value);
  static_assert(same<decltype(op_lambda(s)), char>::value);
  static_assert(same<decltype(call_paren(s)), int>::value);
  static_assert(same<decltype(op_paren(s)), int>::value);
  static_assert(same<decltype(call_qual(s)), int>::value);
  static_assert(same<decltype(op_qual(s)), int>::value);
}

// 4. Pure ADL: nothing named pick is visible here; app::pick is reached
// through S.  Kept out of user, which declares a pick of its own.
namespace pure {
  template<class T> auto call_adl(T t) { return pick(t, t); }
  template<class T> auto op_adl(T t) { return t `pick` t; }
  app::S s{1};
  static_assert(same<decltype(call_adl(s)), long>::value);
  static_assert(same<decltype(op_adl(s)), long>::value);
}

// The non-dependent forms of 3 and 6, outside any template.
namespace nondep {
  app::S s{1};
  inline constexpr user::namer named{};
  static_assert(same<decltype(named(s, 1)), float>::value);
  static_assert(same<decltype(s `named` 1), float>::value);
}
